#ifndef ARKSIGNING_CONSTANTS_H
#define ARKSIGNING_CONSTANTS_H

#include <cstdint>
#include <cstddef>

namespace ArkSigning {
namespace Constants {

// JSON Processing Constants
constexpr size_t MAX_ARRAY_INLINE_SIZE = 25;     // Maximum array size for inline formatting
constexpr size_t MAX_LINE_LENGTH = 75;           // Maximum line length for JSON formatting
constexpr size_t JSON_INDENT_SIZE = 4;           // Standard JSON indentation size

// Base64 Processing Constants
constexpr size_t BASE64_BUFFER_PADDING = 128;    // Extra padding for Base64 buffer allocation
constexpr size_t BASE64_LINE_LENGTH = 76;        // Standard Base64 line length (RFC 2045)
constexpr size_t BASE64_ENCODE_RATIO = 4;        // 4 output chars for every 3 input bytes
constexpr size_t BASE64_DECODE_RATIO = 3;        // 3 output bytes for every 4 input chars

// Code Signing Constants
constexpr uint32_t CODESIGN_HEADER_SIZE = 8;     // Size of code signature header
constexpr uint32_t CODESIGN_SLOT_SIZE = 32;      // Size of each code signature slot
constexpr uint32_t SHA1_HASH_SIZE = 20;          // SHA-1 hash size in bytes
constexpr uint32_t SHA256_HASH_SIZE = 32;        // SHA-256 hash size in bytes

// Mach-O Constants
constexpr uint32_t MACHO_MAGIC_32 = 0xfeedface; // 32-bit Mach-O magic number
constexpr uint32_t MACHO_MAGIC_64 = 0xfeedfacf; // 64-bit Mach-O magic number
constexpr uint32_t MACHO_FAT_MAGIC = 0xcafebabe; // Fat binary magic number
constexpr uint32_t MACHO_FAT_CIGAM = 0xbebafeca; // Fat binary magic number (swapped)

// File I/O Constants
constexpr size_t FILE_BUFFER_SIZE = 8192;        // Standard file I/O buffer size
constexpr size_t MAX_PATH_LENGTH = 4096;         // Maximum file path length
constexpr size_t MAX_FILENAME_LENGTH = 255;      // Maximum filename length

// String Processing Constants
constexpr size_t MAX_STRING_BUFFER = 1024;       // Maximum string buffer size
constexpr size_t SMALL_STRING_BUFFER = 256;      // Small string buffer size
constexpr size_t TINY_STRING_BUFFER = 64;        // Tiny string buffer size

// Bundle Processing Constants
constexpr size_t MAX_BUNDLE_ID_LENGTH = 255;     // Maximum bundle identifier length
constexpr size_t MAX_BUNDLE_NAME_LENGTH = 255;   // Maximum bundle name length
constexpr size_t MAX_VERSION_STRING_LENGTH = 64; // Maximum version string length

// Parallel Processing Constants
constexpr size_t DEFAULT_THREAD_COUNT = 4;       // Default number of threads for parallel processing
constexpr size_t MAX_THREAD_COUNT = 16;          // Maximum number of threads
constexpr size_t MIN_THREAD_COUNT = 1;           // Minimum number of threads

// Memory Management Constants
constexpr size_t MEMORY_ALIGNMENT = 16;          // Memory alignment boundary
constexpr size_t SMALL_ALLOCATION_THRESHOLD = 1024; // Threshold for small allocations
constexpr size_t LARGE_ALLOCATION_THRESHOLD = 1048576; // Threshold for large allocations (1MB)

// Network/URL Constants
constexpr size_t MAX_URL_LENGTH = 2048;          // Maximum URL length
constexpr size_t MAX_HOST_LENGTH = 253;          // Maximum hostname length (RFC 1035)

// Compression Constants
constexpr int MIN_COMPRESSION_LEVEL = 0;         // Minimum compression level
constexpr int MAX_COMPRESSION_LEVEL = 9;         // Maximum compression level
constexpr int DEFAULT_COMPRESSION_LEVEL = 6;     // Default compression level

// Time Constants
constexpr int SECONDS_PER_MINUTE = 60;
constexpr int MINUTES_PER_HOUR = 60;
constexpr int HOURS_PER_DAY = 24;
constexpr int DAYS_PER_WEEK = 7;

// Error Codes
constexpr int SUCCESS = 0;
constexpr int ERROR_INVALID_ARGUMENT = -1;
constexpr int ERROR_FILE_NOT_FOUND = -2;
constexpr int ERROR_PERMISSION_DENIED = -3;
constexpr int ERROR_OUT_OF_MEMORY = -4;
constexpr int ERROR_INVALID_FORMAT = -5;
constexpr int ERROR_SIGNATURE_FAILED = -6;
constexpr int ERROR_NETWORK_ERROR = -7;

// Validation Constants
constexpr size_t MIN_PASSWORD_LENGTH = 1;        // Minimum password length
constexpr size_t MAX_PASSWORD_LENGTH = 1024;     // Maximum password length
constexpr size_t MIN_CERTIFICATE_SIZE = 100;     // Minimum certificate size in bytes
constexpr size_t MAX_CERTIFICATE_SIZE = 10485760; // Maximum certificate size (10MB)

// Format Constants
constexpr const char* DATE_FORMAT_ISO8601 = "%Y-%m-%dT%H:%M:%SZ";
constexpr const char* DATE_FORMAT_READABLE = "%Y-%m-%d %H:%M:%S";
constexpr const char* VERSION_FORMAT = "%d.%d.%d";

// File Extensions
constexpr const char* IPA_EXTENSION = ".ipa";
constexpr const char* APP_EXTENSION = ".app";
constexpr const char* DYLIB_EXTENSION = ".dylib";
constexpr const char* P12_EXTENSION = ".p12";
constexpr const char* PEM_EXTENSION = ".pem";
constexpr const char* MOBILEPROVISION_EXTENSION = ".mobileprovision";

// Bundle File Names
constexpr const char* INFO_PLIST_NAME = "Info.plist";
constexpr const char* EMBEDDED_PROVISION_NAME = "embedded.mobileprovision";
constexpr const char* CODE_RESOURCES_NAME = "_CodeSignature/CodeResources";

} // namespace Constants
} // namespace ArkSigning

// Legacy compatibility macros (to be phased out)
#define ARKSIGNING_MAX_ARRAY_INLINE_SIZE ArkSigning::Constants::MAX_ARRAY_INLINE_SIZE
#define ARKSIGNING_MAX_LINE_LENGTH ArkSigning::Constants::MAX_LINE_LENGTH
#define ARKSIGNING_BASE64_BUFFER_PADDING ArkSigning::Constants::BASE64_BUFFER_PADDING
#define ARKSIGNING_CODESIGN_HEADER_SIZE ArkSigning::Constants::CODESIGN_HEADER_SIZE

#endif // ARKSIGNING_CONSTANTS_H
