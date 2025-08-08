/**
 * @file arksigning.h
 * @brief Master header for ArkSigning - Modern iOS App Signing Tool
 * 
 * This is the main header file that includes all ArkSigning components.
 * Include this single header to access all ArkSigning functionality.
 * 
 * @author ArkSigning Team
 * @version 0.6
 * @date 2024
 * 
 * @section DESCRIPTION
 * 
 * ArkSigning is a modernized iOS app signing tool featuring:
 * - Enterprise-grade C++11 implementation
 * - Parallel processing capabilities
 * - Modern memory management with RAII
 * - Type-safe APIs with std::optional
 * - Comprehensive error handling
 * - Zero-warning compilation
 * 
 * @section USAGE
 * 
 * @code{.cpp}
 * #include <arksigning/arksigning.h>
 * 
 * int main() {
 *     // Use ArkSigning functionality
 *     return 0;
 * }
 * @endcode
 * 
 * @section LICENSE
 * 
 * MIT License - see LICENSE file for details
 */

#ifndef ARKSIGNING_H
#define ARKSIGNING_H

// ============================================================================
// VERSION INFORMATION
// ============================================================================

#define ARKSIGNING_VERSION_MAJOR 0
#define ARKSIGNING_VERSION_MINOR 6
#define ARKSIGNING_VERSION_PATCH 0
#define ARKSIGNING_VERSION_STRING "0.6.0"

// ============================================================================
// MODERN C++ FEATURES
// ============================================================================

/**
 * @defgroup modern Modern C++ Features
 * @brief Modern C++11 features and utilities
 * 
 * These headers provide modern C++ functionality including:
 * - Type-safe optional values
 * - Strongly-typed enums and type aliases
 * - Perfect forwarding utilities
 * - Lambda-based callback system
 */
///@{
#include "modern/optional.h"      ///< C++11-compatible std::optional implementation
#include "modern/types.h"         ///< Strongly-typed enums and type aliases
#include "modern/utility.h"       ///< Perfect forwarding and move utilities
#include "modern/callbacks.h"     ///< Modern lambda-based callback system
///@}

// ============================================================================
// UTILITY COMPONENTS
// ============================================================================

/**
 * @defgroup utils Utility Components
 * @brief Core utility functions and data structures
 * 
 * These headers provide fundamental utilities including:
 * - File system operations
 * - String manipulation
 * - Base64 encoding/decoding
 * - JSON parsing and generation
 * - Mach-O format definitions
 */
///@{
#include "utils/common.h"         ///< Common utilities and file operations
#include "utils/base64.h"         ///< Base64 encoding/decoding
#include "utils/json.h"           ///< JSON parsing and generation
#include "utils/constants.h"      ///< Application constants
#include "utils/mach-o.h"         ///< Mach-O format definitions
///@}

// ============================================================================
// CRYPTOGRAPHIC COMPONENTS
// ============================================================================

/**
 * @defgroup crypto Cryptographic Components
 * @brief Cryptographic operations and certificate handling
 * 
 * These headers provide cryptographic functionality including:
 * - OpenSSL integration
 * - Certificate management
 * - Digital signatures
 * - RAII wrappers for OpenSSL resources
 */
///@{
#include "crypto/openssl.h"       ///< OpenSSL integration and crypto operations
#include "crypto/openssl_raii.h"  ///< RAII wrappers for OpenSSL resources
///@}

// ============================================================================
// CORE APPLICATION COMPONENTS
// ============================================================================

/**
 * @defgroup core Core Application Components
 * @brief Main application logic and processing
 * 
 * These headers provide core application functionality including:
 * - App bundle processing
 * - Mach-O binary manipulation
 * - Archive handling
 * - Code signing operations
 */
///@{
#include "core/bundle.h"          ///< App bundle processing
#include "core/macho.h"           ///< Mach-O binary manipulation
#include "core/archo.h"           ///< Archive handling
#include "core/signing.h"         ///< Code signing operations
///@}

// ============================================================================
// NAMESPACE ALIASES
// ============================================================================

/**
 * @brief Main ArkSigning namespace
 * 
 * All ArkSigning functionality is contained within this namespace.
 * Use namespace aliases for convenience:
 * 
 * @code{.cpp}
 * namespace AS = ArkSigning;
 * using namespace ArkSigning::Types;
 * @endcode
 */
namespace ArkSigning {
    
    /**
     * @brief Get ArkSigning version string
     * @return Version string in format "major.minor.patch"
     */
    inline const char* getVersion() {
        return ARKSIGNING_VERSION_STRING;
    }
    
    /**
     * @brief Get ArkSigning version components
     * @param major Output parameter for major version
     * @param minor Output parameter for minor version  
     * @param patch Output parameter for patch version
     */
    inline void getVersionComponents(int& major, int& minor, int& patch) {
        major = ARKSIGNING_VERSION_MAJOR;
        minor = ARKSIGNING_VERSION_MINOR;
        patch = ARKSIGNING_VERSION_PATCH;
    }
    
    /**
     * @brief Check if ArkSigning version is at least the specified version
     * @param major Required major version
     * @param minor Required minor version
     * @param patch Required patch version
     * @return true if current version >= required version
     */
    inline bool isVersionAtLeast(int major, int minor = 0, int patch = 0) {
        if (ARKSIGNING_VERSION_MAJOR > major) return true;
        if (ARKSIGNING_VERSION_MAJOR < major) return false;
        if (ARKSIGNING_VERSION_MINOR > minor) return true;
        if (ARKSIGNING_VERSION_MINOR < minor) return false;
        return ARKSIGNING_VERSION_PATCH >= patch;
    }
    
} // namespace ArkSigning

// ============================================================================
// CONVENIENCE ALIASES
// ============================================================================

/**
 * @brief Convenience namespace alias
 * 
 * Use AS:: as a shorter alias for ArkSigning::
 */
namespace AS = ArkSigning;

// ============================================================================
// FEATURE DETECTION MACROS
// ============================================================================

/**
 * @defgroup features Feature Detection
 * @brief Macros for detecting available features
 */
///@{

/// Modern C++ features are available
#define ARKSIGNING_HAS_MODERN_CPP 1

/// Optional type is available
#define ARKSIGNING_HAS_OPTIONAL 1

/// Move semantics are available
#define ARKSIGNING_HAS_MOVE_SEMANTICS 1

/// Lambda expressions are available
#define ARKSIGNING_HAS_LAMBDAS 1

/// Strongly-typed enums are available
#define ARKSIGNING_HAS_ENUM_CLASS 1

/// RAII wrappers are available
#define ARKSIGNING_HAS_RAII 1

/// Parallel processing is available
#define ARKSIGNING_HAS_PARALLEL 1

///@}

// ============================================================================
// COMPILER COMPATIBILITY
// ============================================================================

/**
 * @brief Compiler and standard library compatibility checks
 */
#if __cplusplus < 201103L
    #error "ArkSigning requires C++11 or later"
#endif

#ifdef _MSC_VER
    #if _MSC_VER < 1900
        #error "ArkSigning requires Visual Studio 2015 or later"
    #endif
#endif

#ifdef __GNUC__
    #if __GNUC__ < 4 || (__GNUC__ == 4 && __GNUC_MINOR__ < 8)
        #error "ArkSigning requires GCC 4.8 or later"
    #endif
#endif

#ifdef __clang__
    #if __clang_major__ < 3 || (__clang_major__ == 3 && __clang_minor__ < 3)
        #error "ArkSigning requires Clang 3.3 or later"
    #endif
#endif

// ============================================================================
// DOCUMENTATION GROUPS
// ============================================================================

/**
 * @mainpage ArkSigning - Modern iOS App Signing Tool
 * 
 * @section intro_sec Introduction
 * 
 * ArkSigning is a modernized iOS app signing tool that demonstrates
 * enterprise-grade C++ development practices while providing robust
 * app signing functionality.
 * 
 * @section features_sec Key Features
 * 
 * - **Modern C++11** implementation with zero warnings
 * - **Type Safety** with std::optional and enum classes
 * - **Memory Safety** with RAII and smart pointers
 * - **Parallel Processing** for bulk operations
 * - **Comprehensive Error Handling** with detailed reporting
 * - **Cross-platform** support (macOS, Linux)
 * 
 * @section usage_sec Quick Start
 * 
 * @code{.cpp}
 * #include <arksigning/arksigning.h>
 * 
 * int main() {
 *     // Sign a single app
 *     // Implementation details in core components
 *     return 0;
 * }
 * @endcode
 * 
 * @section modules_sec Modules
 * 
 * - @ref modern "Modern C++ Features"
 * - @ref utils "Utility Components" 
 * - @ref crypto "Cryptographic Components"
 * - @ref core "Core Application Components"
 */

#endif // ARKSIGNING_H
