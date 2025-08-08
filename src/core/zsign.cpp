#include "core/bundle.h"
#include "utils/common.h"
#include "modern/callbacks.h"
#include "core/macho.h"
#include "crypto/openssl.h"
#include <dirent.h>
#include <getopt.h>
#include <libgen.h>
#include <string>
#include <vector>
#include <thread>
#include <mutex>
#include <queue>
#include <atomic>
#include <condition_variable>
#include <algorithm>
#include <chrono>

// Forward declaration - this function is defined in bundle.cpp
bool FindAppFolder(const string &strFolder, string &strAppFolder);

const struct option options[] = {
    {"debug", no_argument, NULL, 'd'},
    {"force", no_argument, NULL, 'f'},
    {"verbose", no_argument, NULL, 'v'},
    {"cert", required_argument, NULL, 'c'},
    {"pkey", required_argument, NULL, 'k'},
    {"prov", required_argument, NULL, 'm'},
    {"password", required_argument, NULL, 'p'},
    {"bundle_id", required_argument, NULL, 'b'},
    {"bundle_name", required_argument, NULL, 'n'},
    {"bundle_version", required_argument, NULL, 'r'},
    {"entitlements", required_argument, NULL, 'e'},
    {"output", required_argument, NULL, 'o'},
    {"zip_level", required_argument, NULL, 'z'},
    {"dylib", required_argument, NULL, 'l'},
    {"weak", no_argument, NULL, 'w'},
    {"install", no_argument, NULL, 'i'},
    {"info", no_argument, NULL, 'I'},
    {"quiet", no_argument, NULL, 'q'},
    {"no-embed-profile", no_argument, NULL, 'E'},
    {"help", no_argument, NULL, 'h'},
    {"bulk", no_argument, NULL, 'B'},
    {"inputfolder", required_argument, NULL, 1000},
    {"outputfolder", required_argument, NULL, 1001},
    {"parallel", optional_argument, NULL, 1002},
    {}};

int usage() {
  ZLog::Print("Usage: arksigning [-options] [-k privkey.pem] [-m dev.prov] [-o "
              "output.ipa] file|folder\n");
  ZLog::Print("options:\n");
  ZLog::Print(
      "-k, --pkey\t\tPath to private key or p12 file. (PEM or DER format)\n");
  ZLog::Print("-m, --prov\t\tPath to mobile provisioning profile.\n");
  ZLog::Print("-c, --cert\t\tPath to certificate file. (PEM or DER format)\n");
  ZLog::Print(
      "-d, --debug\t\tGenerate debug output files. (.arksigning_debug folder)\n");
  ZLog::Print("-f, --force\t\tForce sign without cache when signing folder.\n");
  ZLog::Print("-o, --output\t\tPath to output ipa file.\n");
  ZLog::Print("-p, --password\t\tPassword for private key or p12 file.\n");
  ZLog::Print("-b, --bundle_id\t\tNew bundle id to change.\n");
  ZLog::Print("-n, --bundle_name\tNew bundle name to change.\n");
  ZLog::Print("-I, --info\t\tOutput app information in JSON format, including app icon in base64.\n");
  ZLog::Print("-r, --bundle_version\tNew bundle version to change.\n");
  ZLog::Print("-e, --entitlements\tNew entitlements to change.\n");
  ZLog::Print(
      "-z, --zip_level\t\tCompressed level when output the ipa file. (0-9)\n");
  ZLog::Print("-l, --dylib\t\tPath to inject dylib file.\n");
  ZLog::Print(
      "\t\t\tUse -l multiple time to inject multiple dylib files at once.\n");
  ZLog::Print("-w, --weak\t\tInject dylib as LC_LOAD_WEAK_DYLIB.\n");
  ZLog::Print("-i, --install\t\tInstall ipa file using ideviceinstaller "
              "command for test.\n");
  ZLog::Print("-q, --quiet\t\tQuiet operation.\n");
  ZLog::Print("-E, --no-embed-profile\tDon't generate embedded mobile provision.\n");
  ZLog::Print("-v, --version\t\tShows version.\n");
  ZLog::Print("-h, --help\t\tShows help (this message).\n");
  ZLog::Print("\nBulk signing options:\n");
  ZLog::Print("-B, --bulk\t\tEnable bulk signing mode.\n");
  ZLog::Print("--inputfolder\t\tFolder containing unsigned apps to process.\n");
  ZLog::Print("--outputfolder\t\tDestination folder for signed apps.\n");
  ZLog::Print("--parallel\t\tEnable parallel processing with optional thread count.\n");

  return -1;
}

struct SigningTask {
    string inputPath;
    string outputPath;
    bool isZipFile;
};

class ThreadSafeQueue {
private:
    queue<SigningTask> taskQueue;
    mutex queueMutex;
    condition_variable cv;
    bool done;
    
public:
    ThreadSafeQueue() : done(false) {}
    
    void push(const SigningTask& task) {
        lock_guard<mutex> lock(queueMutex);
        taskQueue.push(task);
        cv.notify_one();
    }
    
    bool pop(SigningTask& task) {
        unique_lock<mutex> lock(queueMutex);
        cv.wait(lock, [this]{ return !taskQueue.empty() || done; });
        
        if(done && taskQueue.empty()) {
            return false;
        }
        
        task = taskQueue.front();
        taskQueue.pop();
        return true;
    }
    
    void setDone() {
        lock_guard<mutex> lock(queueMutex);
        done = true;
        cv.notify_all();
    }
    
    size_t size() {
        lock_guard<mutex> lock(queueMutex);
        return taskQueue.size();
    }
    
    bool empty() {
        lock_guard<mutex> lock(queueMutex);
        return taskQueue.empty();
    }
};

bool processFile(const SigningTask& task, arksigningAsset* pSignAsset, bool bForce, 
               bool bWeakInject, bool bDontEmbedProfile, vector<string> arrDyLibFiles, 
               string strBundleId, string strDisplayName, string strBundleVersion,
               uint32_t uZipLevel, atomic<int>& completedTasks, int totalTasks, mutex& printMutex) {
    ZTimer timer;
    bool bEnableCache = !task.isZipFile;
    string strFolder = task.inputPath;
    bool bRet = false;
    
    {
        lock_guard<mutex> lock(printMutex);
        ZLog::PrintV(">>> Processing [%d/%d]: %s\n", completedTasks.load()+1, totalTasks, task.inputPath.c_str());
    }
    
    if (task.isZipFile) {
        StringFormat(strFolder, "/tmp/arksigning_folder_%llu", timer.Reset());
        {
            lock_guard<mutex> lock(printMutex);
            ZLog::PrintV(">>> Unzip:\t%s (%s) -> %s ... \n", task.inputPath.c_str(),
                     GetFileSizeString(task.inputPath.c_str()).c_str(), strFolder.c_str());
        }
        RemoveFolder(strFolder.c_str());
        if (!SystemExec("unzip -qq -d '%s' '%s'", strFolder.c_str(),
                    task.inputPath.c_str())) {
            RemoveFolder(strFolder.c_str());
            lock_guard<mutex> lock(printMutex);
            ZLog::ErrorV(">>> Unzip Failed!\n");
            completedTasks++;
            return false;
        }
    }
    
    timer.Reset();
    ZAppBundle bundle;
    bRet = bundle.SignFolder(pSignAsset, strFolder, strBundleId,
                            strBundleVersion, strDisplayName, arrDyLibFiles,
                            bForce, bWeakInject, bEnableCache,
                            bDontEmbedProfile);
    
    if (bRet && !task.outputPath.empty()) {
        timer.Reset();
        size_t pos = bundle.m_strAppFolder.rfind("/Payload");
        if (string::npos == pos) {
            lock_guard<mutex> lock(printMutex);
            ZLog::Error(">>> Can't Find Payload Directory!\n");
            completedTasks++;
            return false;
        }

        lock_guard<mutex> lock(printMutex);
        ZLog::PrintV(">>> Archiving: \t%s ... \n", task.outputPath.c_str());
        string strBaseFolder = bundle.m_strAppFolder.substr(0, pos);
        char szOldFolder[PATH_MAX] = {0};
        if (NULL != getcwd(szOldFolder, PATH_MAX)) {
            if (0 == chdir(strBaseFolder.c_str())) {
                RemoveFile(task.outputPath.c_str());
                SystemExec("zip -q -%u -r '%s' Payload", uZipLevel,
                       task.outputPath.c_str());
                chdir(szOldFolder);
                if (!IsFileExists(task.outputPath.c_str())) {
                    ZLog::Error(">>> Archive Failed!\n");
                    completedTasks++;
                    return false;
                }
            }
        }
        ZLog::PrintV(">>> Archive OK! (%s)\n", GetFileSizeString(task.outputPath.c_str()).c_str());
    }
    
    // Clean up
    if (task.isZipFile && 0 == strFolder.find("/tmp/arksigning_folder_")) {
        RemoveFolder(strFolder.c_str());
    }
    
    {
        lock_guard<mutex> lock(printMutex);
        if (bRet) {
            ZLog::PrintV(">>> Successfully signed: %s\n", task.inputPath.c_str());
        } else {
            ZLog::ErrorV(">>> Failed to sign: %s\n", task.inputPath.c_str());
        }
    }
    
    completedTasks++;
    return bRet;
}

bool bulkSign(const string& inputFolder, const string& outputFolder, arksigningAsset* pSignAsset,
            bool bForce, bool bWeakInject, bool bDontEmbedProfile, vector<string> arrDyLibFiles,
            string strBundleId, string strDisplayName, string strBundleVersion,
            uint32_t uZipLevel, int threadCount) 
{
    // Create output folder if it doesn't exist
    CreateFolder(outputFolder.c_str());
    
    // Find all app files in the input folder
    vector<SigningTask> allTasks;
    DIR* dir = opendir(inputFolder.c_str());
    if (!dir) {
        ZLog::ErrorV(">>> Cannot open input folder: %s\n", inputFolder.c_str());
        return false;
    }
    
    ZLog::PrintV(">>> Scanning folder: %s\n", inputFolder.c_str());
    
    dirent* entry;
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type == DT_REG || entry->d_type == DT_DIR) {
            if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
                continue;
            }
            
            string fullPath = inputFolder + "/" + entry->d_name;
            bool isZip = IsZipFile(fullPath.c_str());
            bool isFolder = IsFolder(fullPath.c_str());
            
            if (isZip || (isFolder && FindAppFolder(fullPath.c_str(), fullPath))) {
                SigningTask task;
                task.inputPath = fullPath;
                task.isZipFile = isZip;
                
                if (isZip) {
                    string fileName = string(entry->d_name);
                    size_t extPos = fileName.rfind(".ipa");
                    if (extPos != string::npos) {
                        fileName = fileName.substr(0, extPos) + "_signed.ipa";
                    } else {
                        fileName += "_signed.ipa";
                    }
                    task.outputPath = outputFolder + "/" + fileName;
                } else {
                    // For folders, we'll create an IPA with the folder name
                    string folderName = string(entry->d_name);
                    task.outputPath = outputFolder + "/" + folderName + "_signed.ipa";
                }
                
                allTasks.push_back(task);
            }
        }
    }
    closedir(dir);
    
    if (allTasks.empty()) {
        ZLog::PrintV(">>> No valid apps found in the input folder.\n");
        return false;
    }
    
    ZLog::PrintV(">>> Found %zu apps to sign\n", allTasks.size());
    
    if (threadCount <= 0) {
        // Auto-detect optimal thread count
        threadCount = thread::hardware_concurrency();
        if (threadCount <= 0) {
            threadCount = 2; // Default fallback
        }
    }
    
    // Limit thread count to number of tasks
    threadCount = min(threadCount, (int)allTasks.size());
    
    ZLog::PrintV(">>> Using %d worker threads\n", threadCount);

    // Set up modern callback system
    ArkSigning::Callbacks::CallbackManager callbackManager;
    callbackManager.setSigningProgressCallback(ArkSigning::Callbacks::createModernSigningProgressCallback());
    callbackManager.setSigningErrorCallback(ArkSigning::Callbacks::createModernSigningErrorCallback());
    callbackManager.setSigningCompletionCallback(ArkSigning::Callbacks::createModernSigningCompletionCallback());

    // Set up threading
    ThreadSafeQueue taskQueue;
    for (const auto& task : allTasks) {
        taskQueue.push(task);
    }

    mutex printMutex;
    atomic<int> completedTasks(0);
    atomic<int> successfulTasks(0);
    vector<thread> workers;
    auto startTime = chrono::high_resolution_clock::now();
    
    // Create modern worker lambdas with callback support
    auto createWorkerLambda = [&]() {
        return [&taskQueue, pSignAsset, bForce, bWeakInject, bDontEmbedProfile,
                arrDyLibFiles, strBundleId, strDisplayName, strBundleVersion,
                uZipLevel, &completedTasks, &successfulTasks, &printMutex, &allTasks, &callbackManager]() {
            SigningTask task;
            while (taskQueue.pop(task)) {
                // Report progress using modern callback
                int current = ++completedTasks;
                callbackManager.reportSigningProgress(task.inputPath, current, static_cast<int>(allTasks.size()));

                bool success = processFile(task, pSignAsset, bForce, bWeakInject, bDontEmbedProfile,
                                      arrDyLibFiles, strBundleId, strDisplayName, strBundleVersion,
                                      uZipLevel, completedTasks, allTasks.size(), printMutex);
                if (success) {
                    successfulTasks++;
                } else {
                    callbackManager.reportSigningError(task.inputPath, "Processing failed");
                }
            }
        };
    };

    for (int i = 0; i < threadCount; i++) {
        workers.emplace_back(createWorkerLambda());
    }

    // Wait for all tasks to be processed, then signal completion
    while (completedTasks.load() < static_cast<int>(allTasks.size())) {
        this_thread::sleep_for(chrono::milliseconds(100));
    }

    // Signal that no more tasks will be added
    taskQueue.setDone();

    // Wait for all threads to complete
    for (auto& worker : workers) {
        worker.join();
    }

    // Calculate elapsed time and report completion
    auto endTime = chrono::high_resolution_clock::now();
    auto elapsedTime = chrono::duration<double>(endTime - startTime).count();

    callbackManager.reportSigningCompletion(successfulTasks.load(), static_cast<int>(allTasks.size()), elapsedTime);

    return static_cast<size_t>(successfulTasks.load()) == allTasks.size();
}

// Function already declared in bundle.cpp, removed to fix build error

int main(int argc, char *argv[]) {
  ZTimer gtimer;

  bool bForce = false;
  bool bInstall = false;
  bool bWeakInject = false;
  bool bDontEmbedProfile = false;
  bool bBulkMode = false;
  uint32_t uZipLevel = 0;

  string strCertFile;
  string strPKeyFile;
  string strProvFile;
  string strPassword;
  string strBundleId;
  string strBundleVersion;
  string strOutputFile;
  string strDisplayName;
  string strEntitlementsFile;
  string strPath;
  string strInputFolder;
  string strOutputFolder;
  int nParallelThreads = 0;

  vector<string> arrDyLibFiles;

  int opt = 0;
  int argslot = -1;
  while (-1 != (opt = getopt_long(argc, argv, "dfvhBc:k:m:o:ip:e:b:n:z:ql:wE",
                                  options, &argslot))) {
    switch (opt) {
    case 'd':
      ZLog::SetLogLever(ZLog::E_DEBUG);
      break;
    case 'f':
      bForce = true;
      break;
    case 'c':
      strCertFile = optarg;
      break;
    case 'k':
      strPKeyFile = optarg;
      break;
    case 'm':
      strProvFile = optarg;
      break;
    case 'p':
      strPassword = optarg;
      break;
    case 'b':
      strBundleId = optarg;
      break;
    case 'r':
      strBundleVersion = optarg;
      break;
    case 'n':
      strDisplayName = optarg;
      break;
    case 'e':
      strEntitlementsFile = optarg;
      break;
    case 'l':
      arrDyLibFiles.push_back(optarg);
      break;
    case 'i':
      bInstall = true;
      break;
    case 'o':
      strOutputFile = GetCanonicalizePath(optarg);
      break;
    case 'z':
      uZipLevel = atoi(optarg);
      break;
    case 'w':
      bWeakInject = true;
      break;
    case 'E':
      bDontEmbedProfile = true;
      break;
    case 'q':
      ZLog::SetLogLever(ZLog::E_NONE);
      break;
    case 'v':
      printf("version: 0.6\n");
      return 0;
    case 'B':
      bBulkMode = true;
      break;
    case 1000: // inputfolder
      strInputFolder = GetCanonicalizePath(optarg);
      break;
    case 1001: // outputfolder
      strOutputFolder = GetCanonicalizePath(optarg);
      break;
    case 1002: // parallel
      if (optarg) {
        nParallelThreads = atoi(optarg);
      } else {
        nParallelThreads = -1; // Auto-detect
      }
      break;
    case 'h':
    case '?':
      return usage();
    case 'I': {
      strPath = GetCanonicalizePath(argv[optind]);
      if (!IsFolder(strPath.c_str()) && !IsZipFile(strPath.c_str())) {
        ZLog::ErrorV(">>> Invalid input file! Please provide an IPA file or app folder.\n");
        return -1;
      }
      
      string strFolder = strPath;
      ZTimer timer;
      if (IsZipFile(strPath.c_str())) {
        StringFormat(strFolder, "/tmp/arksigning_info_%llu", timer.Reset());
        ZLog::PrintV(">>> Unzip:\t%s -> %s ... \n", strPath.c_str(), strFolder.c_str());
        RemoveFolder(strFolder.c_str());
        if (!SystemExec("unzip -qq -d '%s' '%s'", strFolder.c_str(), strPath.c_str())) {
          RemoveFolder(strFolder.c_str());
          ZLog::ErrorV(">>> Unzip Failed!\n");
          return -1;
        }
      }
      
      ZAppBundle bundle;
      bundle.m_strAppFolder = strFolder;
      JValue jvInfo;
      if (bundle.GetAppInfoJson(jvInfo)) {
        if (jvInfo.has("icon_base64")) {
          ZLog::PrintV(">>> App icon found\n");
        } else {
          ZLog::PrintV(">>> No app icon found\n");
        }
        
        string strJson;
        jvInfo.styleWrite(strJson);
        printf("%s\n", strJson.c_str());
      }
      
      if (0 == strFolder.find("/tmp/arksigning_info_")) {
        RemoveFolder(strFolder.c_str());
      }
      return 0;
    }
    }
    ZLog::DebugV(">>> Option:\t-%c, %s\n", opt, optarg);
  }

  if (bBulkMode) {
    if (strInputFolder.empty() || strOutputFolder.empty()) {
      ZLog::ErrorV(">>> Bulk mode requires both --inputfolder and --outputfolder parameters\n");
      return usage();
    }
    
    if (!IsFolder(strInputFolder.c_str())) {
      ZLog::ErrorV(">>> Input folder does not exist or is not a directory: %s\n", strInputFolder.c_str());
      return -1;
    }
    
    arksigningAsset arksigningAsset;
    if (!arksigningAsset.Init(strCertFile, strPKeyFile, strProvFile,
                         strEntitlementsFile, strPassword)) {
      return -1;
    }
    
    bool bSuccess = bulkSign(strInputFolder, strOutputFolder, &arksigningAsset,
                           bForce, bWeakInject, bDontEmbedProfile, arrDyLibFiles,
                           strBundleId, strDisplayName, strBundleVersion,
                           uZipLevel, nParallelThreads);
    
    gtimer.Print(">>> Bulk signing completed.");
    return bSuccess ? 0 : -1;
  }
  
  if (optind >= argc) {
    return usage();
  }

  strPath = GetCanonicalizePath(argv[optind]);
  if (!IsFileExists(strPath.c_str())) {
    ZLog::ErrorV(">>> Invalid Path! %s\n", strPath.c_str());
    return -1;
  }

  if (ZLog::IsDebug()) {
    CreateFolder("./.arksigning_debug");
    for (int i = optind; i < argc; i++) {
      ZLog::DebugV(">>> Argument:\t%s\n", argv[i]);
    }
  }

  bool bZipFile = false;
  if (!IsFolder(strPath.c_str())) {
    bZipFile = IsZipFile(strPath.c_str());
    if (!bZipFile) { // macho file
      ZMachO macho;
      if (macho.Init(strPath.c_str())) {
        if (!arrDyLibFiles.empty()) { // inject dylib
          bool bCreate = false;
          for (string dyLibFile : arrDyLibFiles)
            macho.InjectDyLib(bWeakInject, dyLibFile.c_str(), bCreate);
        } else {
          macho.PrintInfo();
        }
        macho.Free();
      }
      return 0;
    }
  }

  ZTimer timer;
  arksigningAsset arksigningAsset;
  if (!arksigningAsset.Init(strCertFile, strPKeyFile, strProvFile,
                       strEntitlementsFile, strPassword)) {
    return -1;
  }

  bool bEnableCache = true;
  string strFolder = strPath;
  if (bZipFile) {
    bForce = true;
    bEnableCache = false;
    StringFormat(strFolder, "/tmp/arksigning_folder_%llu", timer.Reset());
    ZLog::PrintV(">>> Unzip:\t%s (%s) -> %s ... \n", strPath.c_str(),
                 GetFileSizeString(strPath.c_str()).c_str(), strFolder.c_str());
    RemoveFolder(strFolder.c_str());
    if (!SystemExec("unzip -qq -d '%s' '%s'", strFolder.c_str(),
                    strPath.c_str())) {
      RemoveFolder(strFolder.c_str());
      ZLog::ErrorV(">>> Unzip Failed!\n");
      return -1;
    }
    timer.PrintResult(true, ">>> Unzip OK!");
  }

  timer.Reset();
  ZAppBundle bundle;
  bool bRet = bundle.SignFolder(&arksigningAsset, strFolder, strBundleId,
                                strBundleVersion, strDisplayName, arrDyLibFiles,
                                bForce, bWeakInject, bEnableCache,
                                bDontEmbedProfile);
  timer.PrintResult(bRet, ">>> Signed %s!", bRet ? "OK" : "Failed");

  if (bInstall && strOutputFile.empty()) {
    StringFormat(strOutputFile, "/tmp/arksigning_temp_%llu.ipa", GetMicroSecond());
  }

  if (!strOutputFile.empty()) {
    timer.Reset();
    size_t pos = bundle.m_strAppFolder.rfind("/Payload");
    if (string::npos == pos) {
      ZLog::Error(">>> Can't Find Payload Directory!\n");
      return -1;
    }

    ZLog::PrintV(">>> Archiving: \t%s ... \n", strOutputFile.c_str());
    string strBaseFolder = bundle.m_strAppFolder.substr(0, pos);
    char szOldFolder[PATH_MAX] = {0};
    if (NULL != getcwd(szOldFolder, PATH_MAX)) {
      if (0 == chdir(strBaseFolder.c_str())) {
        uZipLevel = uZipLevel > 9 ? 9 : uZipLevel;
        RemoveFile(strOutputFile.c_str());
        SystemExec("zip -q -%u -r '%s' Payload", uZipLevel,
                   strOutputFile.c_str());
        chdir(szOldFolder);
        if (!IsFileExists(strOutputFile.c_str())) {
          ZLog::Error(">>> Archive Failed!\n");
          return -1;
        }
      }
    }
    timer.PrintResult(true, ">>> Archive OK! (%s)",
                      GetFileSizeString(strOutputFile.c_str()).c_str());
  }

  if (bRet && bInstall) {
    SystemExec("ideviceinstaller -i '%s'", strOutputFile.c_str());
  }

  if (0 == strOutputFile.find("/tmp/arksigning_tmp_")) {
    RemoveFile(strOutputFile.c_str());
  }

  if (0 == strFolder.find("/tmp/arksigning_folder_")) {
    RemoveFolder(strFolder.c_str());
  }

  gtimer.Print(">>> Done.");
  return bRet ? 0 : -1;
}
