# ArkSigning API Reference

This document provides a comprehensive API reference for the ArkSigning library components.

## 🚀 Quick Start

```cpp
#include <arksigning/arksigning.h>

int main() {
    // Use ArkSigning functionality
    return 0;
}
```

## 📋 Core Components

### **Bundle Processing** (`core/bundle.h`)

Functions for iOS app bundle manipulation:

```cpp
// Find app folder in a directory
bool FindAppFolder(const string &strFolder, string &strAppFolder);

// Process app bundle
bool ProcessBundle(const string &bundlePath);
```

### **Mach-O Binary Handling** (`core/macho.h`)

Classes and functions for Mach-O binary processing:

```cpp
class ZMachO {
public:
    ZMachO();
    ~ZMachO();
    
    bool Init(const char *szFile);
    bool Free();
    // ... additional methods
};
```

### **Archive Operations** (`core/archo.h`)

Archive handling utilities for IPA files:

```cpp
// Archive manipulation functions
bool ExtractArchive(const string &archivePath, const string &outputPath);
bool CreateArchive(const string &inputPath, const string &archivePath);
```

### **Code Signing** (`core/signing.h`)

Digital signature operations:

```cpp
// Signing operations
bool SignBundle(const string &bundlePath, const SigningAsset &asset);
bool VerifySignature(const string &bundlePath);
```

## 🔐 Cryptographic Components

### **OpenSSL Integration** (`crypto/openssl.h`)

Cryptographic operations and certificate handling:

```cpp
// Certificate operations
bool GetCertSubjectCN(const string &strCertData, string &strSubjectCN);
Optional<string> GetCertSubjectCNOptional(const string &strCertData);

// CMS operations  
bool GenerateCMS(const string &strSignerCertData, const string &strSignerPKeyData,
                 const string &strCDHashData, const string &strCDHashPlist, 
                 string &strCMSOutput);
Optional<string> GenerateCMSOptional(const string &strSignerCertData, 
                                    const string &strSignerPKeyData,
                                    const string &strCDHashData, 
                                    const string &strCDHashPlist);

// Content extraction
bool GetCMSContent(const string &strCMSDataInput, string &strContentOutput);
Optional<string> GetCMSContentOptional(const string &strCMSDataInput);
```

### **RAII Wrappers** (`crypto/openssl_raii.h`)

Smart wrappers for OpenSSL resources:

```cpp
// RAII wrapper for X509 certificates
class X509Ptr {
public:
    explicit X509Ptr(X509* cert = nullptr);
    ~X509Ptr();
    
    X509* get() const;
    X509* release();
    void reset(X509* cert = nullptr);
    
    explicit operator bool() const;
};

// RAII wrapper for EVP_PKEY
class EVPKeyPtr {
public:
    explicit EVPKeyPtr(EVP_PKEY* key = nullptr);
    ~EVPKeyPtr();
    
    EVP_PKEY* get() const;
    EVP_PKEY* release();
    void reset(EVP_PKEY* key = nullptr);
    
    explicit operator bool() const;
};
```

## 🛠️ Utility Components

### **Common Utilities** (`utils/common.h`)

File operations and system utilities:

```cpp
// File operations
bool IsFileExists(const char *szFile);
int64_t GetFileSize(const char *szFile);
Optional<int64_t> GetFileSizeOptional(const char *szFile);

bool ReadFile(const char *szFile, string &strData);
Optional<string> ReadFileOptional(const char *szFile);

bool WriteFile(const char *szFile, const string &strData);
bool WriteFile(const char *szFile, const char *pData, size_t uSize);

// Directory operations
bool IsFolder(const char *szFolder);
bool CreateFolder(const char *szFolder);
bool RemoveFolder(const char *szFolder);

// String utilities
string &StringReplace(string &context, const string &from, const string &to);
void StringSplit(const string &src, const string &split, vector<string> &dest);

// Modern lambda-based utilities
void ProcessFilesWithCallback(const vector<string>& files, 
                             const function<bool(const string&)>& processor,
                             const function<void(const string&, bool)>& resultCallback = nullptr);

void ProcessFilesInParallel(const vector<string>& files,
                           const function<bool(const string&)>& processor,
                           const function<void(const string&, bool)>& resultCallback = nullptr,
                           int threadCount = 4);
```

### **Base64 Encoding** (`utils/base64.h`)

Base64 encoding/decoding with modern C++ features:

```cpp
class ZBase64 {
public:
    ZBase64();
    ~ZBase64();
    
    // Move semantics
    ZBase64(ZBase64 &&other) noexcept;
    ZBase64 &operator=(ZBase64 &&other) noexcept;
    
    // Encoding/decoding
    const char *Encode(const char *szSrc, int nSrcLen = 0);
    const char *Encode(const string &strInput);
    const char *Decode(const char *szSrc, int nSrcLen = 0, int *pDecLen = nullptr);
    const char *Decode(const char *szSrc, string &strOutput);
};
```

### **JSON Processing** (`utils/json.h`)

JSON parsing and generation with modern C++ features:

```cpp
class JValue {
public:
    // Modern strongly-typed enum
    using ValueType = ArkSigning::Types::JsonValueType;
    
    // Constructors with move semantics
    JValue();
    JValue(const JValue &other);
    JValue(JValue &&other) noexcept;
    
    // Assignment operators
    JValue &operator=(const JValue &other);
    JValue &operator=(JValue &&other) noexcept;
    
    // Type-safe methods
    ValueType getValueType() const;
    bool isType(ValueType type) const;
    bool isNull() const;
    bool isInteger() const;
    bool isBoolean() const;
    bool isFloat() const;
    bool isArray() const;
    bool isObject() const;
    bool isString() const;
    bool isDate() const;
    bool isData() const;
    
    // Value access
    int asInt() const;
    bool asBool() const;
    double asFloat() const;
    int64_t asInt64() const;
    string asString() const;
    const char *asCString() const;
};
```

## ✨ Modern C++ Features

### **Optional Type** (`modern/optional.h`)

C++11-compatible std::optional implementation:

```cpp
template<typename T>
class Optional {
public:
    Optional();
    Optional(const T& value);
    Optional(T&& value);
    Optional(const Optional& other);
    Optional(Optional&& other);
    
    ~Optional();
    
    Optional& operator=(const Optional& other);
    Optional& operator=(Optional&& other);
    
    bool has_value() const;
    explicit operator bool() const;
    
    const T& value() const;
    T& value();
    
    const T& operator*() const;
    T& operator*();
    
    const T* operator->() const;
    T* operator->();
    
    template<typename U>
    T value_or(U&& default_value) const;
    
    void reset();
};
```

### **Type System** (`modern/types.h`)

Strongly-typed enums and type aliases:

```cpp
// Strongly-typed enums
enum class JsonValueType : uint8_t {
    Null = 0, Integer, Boolean, Float, Array, Object, String, Date, Data
};

enum class ShaType : uint8_t {
    SHA1 = 1, SHA256 = 2
};

enum class LogLevel : uint8_t {
    None = 0, Error = 1, Warning = 2, Info = 3, Debug = 4
};

// Type aliases
using ByteArray = std::vector<uint8_t>;
using StringArray = std::vector<std::string>;
using FilePath = std::string;
using ProgressCallback = std::function<void(double progress, const std::string& message)>;
using ErrorCallback = std::function<void(ErrorCode code, const std::string& message)>;
```

### **Callback System** (`modern/callbacks.h`)

Lambda-based callback management:

```cpp
class CallbackManager {
public:
    CallbackManager();
    ~CallbackManager();
    
    // Callback registration
    void setProgressCallback(ProgressCallback callback);
    void setErrorCallback(ErrorCallback callback);
    void setCompletionCallback(CompletionCallback callback);
    
    // Callback invocation
    void reportProgress(double progress, const std::string& message) const;
    void reportError(Types::ErrorCode code, const std::string& message) const;
    void reportCompletion(bool success, const std::string& result) const;
    
    // Utility methods
    bool hasProgressCallback() const;
    void clearAllCallbacks();
};

// Utility functions
ProgressCallback createConsoleProgressCallback();
ErrorCallback createConsoleErrorCallback();
CompletionCallback createConsoleCompletionCallback();
```

### **Perfect Forwarding Utilities** (`modern/utility.h`)

Advanced template utilities:

```cpp
// Perfect forwarding wrapper
template<typename Func, typename... Args>
auto invoke_with_perfect_forwarding(Func&& func, Args&&... args) 
    -> decltype(std::forward<Func>(func)(std::forward<Args>(args)...));

// Factory function with perfect forwarding
template<typename T, typename... Args>
std::unique_ptr<T> make_unique_with_forwarding(Args&&... args);

// Move-only function wrapper
template<typename Signature>
class MoveOnlyFunction;

// RAII scope guard
template<typename Func>
class ScopeGuard;
```

## 🎯 Usage Examples

### **Basic File Processing**

```cpp
#include <arksigning/arksigning.h>

// Process files with lambda callback
vector<string> files = {"file1.txt", "file2.txt"};
ProcessFilesWithCallback(files, 
    [](const string& file) -> bool {
        // Process file
        return true;
    },
    [](const string& file, bool success) {
        printf("File %s: %s\n", file.c_str(), success ? "OK" : "FAILED");
    }
);
```

### **Optional-based Error Handling**

```cpp
// Safe file size retrieval
auto size = GetFileSizeOptional("myfile.txt");
if (size) {
    printf("File size: %lld bytes\n", size.value());
} else {
    printf("Failed to get file size\n");
}

// Safe certificate processing
auto cn = GetCertSubjectCNOptional(certData);
if (cn) {
    printf("Certificate CN: %s\n", cn.value().c_str());
}
```

### **Modern Callback Usage**

```cpp
// Set up modern callbacks
CallbackManager callbacks;
callbacks.setProgressCallback([](double progress, const string& msg) {
    printf("Progress: %.1f%% - %s\n", progress * 100, msg.c_str());
});

callbacks.setErrorCallback([](ErrorCode code, const string& msg) {
    fprintf(stderr, "Error %d: %s\n", code, msg.c_str());
});

// Use callbacks
callbacks.reportProgress(0.5, "Processing...");
```

---

This API reference provides comprehensive coverage of all ArkSigning components. For more detailed examples and usage patterns, see the main documentation and source code examples.
