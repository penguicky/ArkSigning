#ifndef ARKSIGNING_TYPES_H
#define ARKSIGNING_TYPES_H

#include <cstdint>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <functional>

namespace ArkSigning {
namespace Types {

// ============================================================================
// STRONGLY-TYPED ENUMS (enum class)
// ============================================================================

/**
 * @brief JSON Value Types - Strongly typed replacement for JValue::TYPE
 */
enum class JsonValueType : uint8_t {
    Null = 0,
    Integer,
    Boolean,
    Float,
    Array,
    Object,
    String,
    Date,
    Data
};

/**
 * @brief JSON Token Types - Strongly typed replacement for Token::TYPE
 */
enum class JsonTokenType : uint8_t {
    Error = 0,
    End,
    Null,
    True,
    False,
    Number,
    String,
    ArrayBegin,
    ArrayEnd,
    ObjectBegin,
    ObjectEnd,
    ArraySeparator,
    MemberSeparator
};

/**
 * @brief PList Token Types - Strongly typed replacement for PReader::Token::TYPE
 */
enum class PListTokenType : uint8_t {
    Error = 0,
    End,
    Null,
    True,
    False,
    Key,
    Data,
    Date,
    Integer,
    Real,
    String,
    ArrayBegin,
    ArrayEnd,
    ArrayNull,
    DictionaryBegin,
    DictionaryEnd,
    DictionaryNull,
    ArraySeparator,
    MemberSeparator
};

/**
 * @brief SHA Sum Types - Strongly typed replacement for E_SHASUM_TYPE_*
 */
enum class ShaType : uint8_t {
    SHA1 = 1,
    SHA256 = 2
};

/**
 * @brief Log Levels - Strongly typed replacement for ZLog::eLogType
 */
enum class LogLevel : uint8_t {
    None = 0,
    Error = 1,
    Warning = 2,
    Info = 3,
    Debug = 4
};

/**
 * @brief CPU Architecture Types - Strongly typed replacement for CPU_TYPE_*
 */
enum class CpuType : int32_t {
    Any = -1,
    VAX = 1,
    MC680x0 = 6,
    X86 = 7,
    I386 = X86,  // compatibility
    MIPS = 8,
    MC98000 = 10,
    HPPA = 11,
    ARM = 12,
    MC88000 = 13,
    SPARC = 14,
    I860 = 15,
    ALPHA = 16,
    POWERPC = 18,
    X86_64 = (X86 | 0x01000000),  // CPU_ARCH_ABI64
    ARM64 = (ARM | 0x01000000),   // CPU_ARCH_ABI64
    ARM64_32 = (ARM | 0x02000000), // CPU_ARCH_ABI64_32
    POWERPC64 = (POWERPC | 0x01000000) // CPU_ARCH_ABI64
};

/**
 * @brief Mach-O Load Command Types
 */
enum class LoadCommandType : uint32_t {
    SEGMENT = 0x1,
    SYMTAB = 0x2,
    SYMSEG = 0x3,
    THREAD = 0x4,
    UNIXTHREAD = 0x5,
    LOADFVMLIB = 0x6,
    IDFVMLIB = 0x7,
    IDENT = 0x8,
    FVMFILE = 0x9,
    PREPAGE = 0xa,
    DYSYMTAB = 0xb,
    LOAD_DYLIB = 0xc,
    ID_DYLIB = 0xd,
    LOAD_DYLINKER = 0xe,
    ID_DYLINKER = 0xf,
    PREBOUND_DYLIB = 0x10,
    ROUTINES = 0x11,
    SUB_FRAMEWORK = 0x12,
    SUB_UMBRELLA = 0x13,
    SUB_CLIENT = 0x14,
    SUB_LIBRARY = 0x15,
    TWOLEVEL_HINTS = 0x16,
    PREBIND_CKSUM = 0x17,
    LOAD_WEAK_DYLIB = 0x80000018,
    SEGMENT_64 = 0x19,
    ROUTINES_64 = 0x1a,
    UUID = 0x1b,
    RPATH = 0x8000001c,
    CODE_SIGNATURE = 0x1d,
    SEGMENT_SPLIT_INFO = 0x1e,
    REEXPORT_DYLIB = 0x8000001f,
    LAZY_LOAD_DYLIB = 0x20,
    ENCRYPTION_INFO = 0x21,
    DYLD_INFO = 0x22,
    DYLD_INFO_ONLY = 0x80000022,
    LOAD_UPWARD_DYLIB = 0x80000023,
    VERSION_MIN_MACOSX = 0x24,
    VERSION_MIN_IPHONEOS = 0x25,
    FUNCTION_STARTS = 0x26,
    DYLD_ENVIRONMENT = 0x27,
    MAIN = 0x80000028,
    DATA_IN_CODE = 0x29,
    SOURCE_VERSION = 0x2A,
    DYLIB_CODE_SIGN_DRS = 0x2B,
    ENCRYPTION_INFO_64 = 0x2C,
    LINKER_OPTION = 0x2D,
    LINKER_OPTIMIZATION_HINT = 0x2E,
    VERSION_MIN_TVOS = 0x2F,
    VERSION_MIN_WATCHOS = 0x30
};

/**
 * @brief Code Signature Magic Numbers
 */
enum class CodeSignMagic : uint32_t {
    REQUIREMENT = 0xfade0c00,
    REQUIREMENTS = 0xfade0c01,
    CODEDIRECTORY = 0xfade0c02,
    EMBEDDED_SIGNATURE = 0xfade0cc0,
    EMBEDDED_SIGNATURE_OLD = 0xfade0b02,
    EMBEDDED_ENTITLEMENTS = 0xfade7171,
    EMBEDDED_DER_ENTITLEMENTS = 0xfade7172,
    DETACHED_SIGNATURE = 0xfade0cc1,
    BLOBWRAPPER = 0xfade0b01
};

// ============================================================================
// TYPE ALIASES (using declarations)
// ============================================================================

// Basic type aliases for better readability
using ByteArray = std::vector<uint8_t>;
using StringArray = std::vector<std::string>;
using StringMap = std::map<std::string, std::string>;
using FileSize = int64_t;
using Timestamp = uint64_t;
using ProcessId = uint32_t;
using ThreadId = uint32_t;

// Mach-O specific type aliases
using CpuTypeValue = int32_t;
using CpuSubtypeValue = int32_t;
using VmProtection = int32_t;
using MachOOffset = uint32_t;
using MachOSize = uint32_t;
using MachOAddress = uint64_t;

// Code signing type aliases
using HashValue = std::vector<uint8_t>;
using CertificateData = std::vector<uint8_t>;
using SignatureData = std::vector<uint8_t>;
using EntitlementsData = std::string;

// File system type aliases
using FilePath = std::string;
using FileName = std::string;
using DirectoryPath = std::string;
using FilePermissions = uint32_t;

// Network type aliases
using UrlString = std::string;
using HostName = std::string;
using PortNumber = uint16_t;

// Utility type aliases
using ErrorCode = int32_t;
using StatusCode = int32_t;
using VersionNumber = uint32_t;
using BuildNumber = uint32_t;

// Function pointer type aliases
using ProgressCallback = std::function<void(double progress, const std::string& message)>;
using ErrorCallback = std::function<void(ErrorCode code, const std::string& message)>;
using CompletionCallback = std::function<void(bool success, const std::string& result)>;

// Container type aliases for common patterns
template<typename T>
using UniquePtr = std::unique_ptr<T>;

template<typename T>
using SharedPtr = std::shared_ptr<T>;

template<typename T>
using WeakPtr = std::weak_ptr<T>;

template<typename Key, typename Value>
using HashMap = std::map<Key, Value>;

template<typename T>
using List = std::vector<T>;

template<typename T, typename Compare = std::less<T>>
using Set = std::set<T, Compare>;

// ============================================================================
// UTILITY FUNCTIONS FOR TYPE CONVERSIONS
// ============================================================================

/**
 * @brief Convert legacy enum values to strongly-typed enums
 */
inline JsonValueType toJsonValueType(int legacyType) {
    return static_cast<JsonValueType>(legacyType);
}

inline ShaType toShaType(int legacyType) {
    return static_cast<ShaType>(legacyType);
}

inline LogLevel toLogLevel(int legacyLevel) {
    return static_cast<LogLevel>(legacyLevel);
}

/**
 * @brief Convert strongly-typed enums to legacy values (for backward compatibility)
 */
inline int fromJsonValueType(JsonValueType type) {
    return static_cast<int>(type);
}

inline int fromShaType(ShaType type) {
    return static_cast<int>(type);
}

inline int fromLogLevel(LogLevel level) {
    return static_cast<int>(level);
}

/**
 * @brief String conversion utilities
 */
inline std::string toString(JsonValueType type) {
    switch (type) {
        case JsonValueType::Null: return "null";
        case JsonValueType::Integer: return "integer";
        case JsonValueType::Boolean: return "boolean";
        case JsonValueType::Float: return "float";
        case JsonValueType::Array: return "array";
        case JsonValueType::Object: return "object";
        case JsonValueType::String: return "string";
        case JsonValueType::Date: return "date";
        case JsonValueType::Data: return "data";
        default: return "unknown";
    }
}

inline std::string toString(ShaType type) {
    switch (type) {
        case ShaType::SHA1: return "SHA1";
        case ShaType::SHA256: return "SHA256";
        default: return "unknown";
    }
}

inline std::string toString(LogLevel level) {
    switch (level) {
        case LogLevel::None: return "NONE";
        case LogLevel::Error: return "ERROR";
        case LogLevel::Warning: return "WARNING";
        case LogLevel::Info: return "INFO";
        case LogLevel::Debug: return "DEBUG";
        default: return "UNKNOWN";
    }
}

inline std::string toString(CpuType type) {
    switch (type) {
        case CpuType::X86: return "x86";
        case CpuType::X86_64: return "x86_64";
        case CpuType::ARM: return "arm";
        case CpuType::ARM64: return "arm64";
        case CpuType::ARM64_32: return "arm64_32";
        case CpuType::POWERPC: return "powerpc";
        case CpuType::POWERPC64: return "powerpc64";
        default: return "unknown";
    }
}

} // namespace Types
} // namespace ArkSigning

// Convenience aliases in global namespace for backward compatibility
using JsonValueType = ArkSigning::Types::JsonValueType;
using ShaType = ArkSigning::Types::ShaType;
using LogLevel = ArkSigning::Types::LogLevel;
using ByteArray = ArkSigning::Types::ByteArray;
using StringArray = ArkSigning::Types::StringArray;
using FilePath = ArkSigning::Types::FilePath;

#endif // ARKSIGNING_TYPES_H
