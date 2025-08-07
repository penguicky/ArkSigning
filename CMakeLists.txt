cmake_minimum_required(VERSION 3.14)

project(arksigning)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Enable verbose output for debugging
set(CMAKE_VERBOSE_MAKEFILE ON)

# Dependencies
# On macOS, search Homebrew for keg-only versions of OpenSSL
if (CMAKE_HOST_SYSTEM_NAME MATCHES "Darwin")
    message(STATUS "Detecting OpenSSL on macOS...")

    # Try OpenSSL 3.x first (preferred)
    execute_process(
        COMMAND brew --prefix openssl@3
        RESULT_VARIABLE BREW_OPENSSL3_RESULT
        OUTPUT_VARIABLE BREW_OPENSSL3_PREFIX
        OUTPUT_STRIP_TRAILING_WHITESPACE
        ERROR_QUIET
    )

    if (BREW_OPENSSL3_RESULT EQUAL 0 AND EXISTS "${BREW_OPENSSL3_PREFIX}")
        message(STATUS "Found OpenSSL 3.x installed by Homebrew at ${BREW_OPENSSL3_PREFIX}")
        set(OPENSSL_ROOT_DIR "${BREW_OPENSSL3_PREFIX}")
        set(OPENSSL_INCLUDE_DIR "${BREW_OPENSSL3_PREFIX}/include")
        set(OPENSSL_LIBRARIES "${BREW_OPENSSL3_PREFIX}/lib/libcrypto.dylib")
        set(OPENSSL_FOUND TRUE)
    else()
        # Fallback to default OpenSSL
        execute_process(
            COMMAND brew --prefix openssl
            RESULT_VARIABLE BREW_OPENSSL_RESULT
            OUTPUT_VARIABLE BREW_OPENSSL_PREFIX
            OUTPUT_STRIP_TRAILING_WHITESPACE
            ERROR_QUIET
        )

        if (BREW_OPENSSL_RESULT EQUAL 0 AND EXISTS "${BREW_OPENSSL_PREFIX}")
            message(STATUS "Found OpenSSL installed by Homebrew at ${BREW_OPENSSL_PREFIX}")
            set(OPENSSL_ROOT_DIR "${BREW_OPENSSL_PREFIX}")
            set(OPENSSL_INCLUDE_DIR "${BREW_OPENSSL_PREFIX}/include")
            set(OPENSSL_LIBRARIES "${BREW_OPENSSL_PREFIX}/lib/libcrypto.dylib")
            set(OPENSSL_FOUND TRUE)
        endif()
    endif()

    # If Homebrew detection failed, try common paths
    if (NOT OPENSSL_FOUND)
        message(STATUS "Homebrew OpenSSL not found, trying common paths...")

        # Check Apple Silicon Homebrew paths
        if (EXISTS "/opt/homebrew/opt/openssl@3/include")
            set(OPENSSL_ROOT_DIR "/opt/homebrew/opt/openssl@3")
            set(OPENSSL_INCLUDE_DIR "/opt/homebrew/opt/openssl@3/include")
            set(OPENSSL_LIBRARIES "/opt/homebrew/opt/openssl@3/lib/libcrypto.dylib")
            set(OPENSSL_FOUND TRUE)
            message(STATUS "Using Apple Silicon Homebrew OpenSSL 3.x")
        elseif (EXISTS "/opt/homebrew/opt/openssl/include")
            set(OPENSSL_ROOT_DIR "/opt/homebrew/opt/openssl")
            set(OPENSSL_INCLUDE_DIR "/opt/homebrew/opt/openssl/include")
            set(OPENSSL_LIBRARIES "/opt/homebrew/opt/openssl/lib/libcrypto.dylib")
            set(OPENSSL_FOUND TRUE)
            message(STATUS "Using Apple Silicon Homebrew OpenSSL")
        # Check Intel Homebrew paths
        elseif (EXISTS "/usr/local/opt/openssl@3/include")
            set(OPENSSL_ROOT_DIR "/usr/local/opt/openssl@3")
            set(OPENSSL_INCLUDE_DIR "/usr/local/opt/openssl@3/include")
            set(OPENSSL_LIBRARIES "/usr/local/opt/openssl@3/lib/libcrypto.dylib")
            set(OPENSSL_FOUND TRUE)
            message(STATUS "Using Intel Homebrew OpenSSL 3.x")
        elseif (EXISTS "/usr/local/opt/openssl/include")
            set(OPENSSL_ROOT_DIR "/usr/local/opt/openssl")
            set(OPENSSL_INCLUDE_DIR "/usr/local/opt/openssl/include")
            set(OPENSSL_LIBRARIES "/usr/local/opt/openssl/lib/libcrypto.dylib")
            set(OPENSSL_FOUND TRUE)
            message(STATUS "Using Intel Homebrew OpenSSL")
        endif()
    endif()

    # Final fallback to system OpenSSL (not recommended but might work)
    if (NOT OPENSSL_FOUND)
        message(WARNING "Homebrew OpenSSL not found, falling back to system search")
        find_package(OpenSSL REQUIRED)
    endif()
else()
    # Linux and other systems
    find_package(OpenSSL REQUIRED)
endif()

# Verify OpenSSL was found
if (NOT OPENSSL_FOUND)
    message(FATAL_ERROR "OpenSSL not found! Please install OpenSSL development libraries.")
endif()

message(STATUS "OpenSSL include dir: ${OPENSSL_INCLUDE_DIR}")
message(STATUS "OpenSSL libraries: ${OPENSSL_LIBRARIES}")

# Find ZLIB
find_package(ZLIB REQUIRED)
message(STATUS "ZLIB include dir: ${ZLIB_INCLUDE_DIR}")
message(STATUS "ZLIB libraries: ${ZLIB_LIBRARIES}")

# Collect all libraries
set(LIB_LIST ${OPENSSL_LIBRARIES} ${ZLIB_LIBRARIES})

# Main sources (organized by component)
file(GLOB CORE_SRC src/core/*.cpp)
file(GLOB CRYPTO_SRC src/crypto/*.cpp)
file(GLOB UTILS_SRC src/utils/*.cpp)
set(ALL_SRC ${CORE_SRC} ${CRYPTO_SRC} ${UTILS_SRC})

# Remove any potential duplicates and ensure we have sources
list(REMOVE_DUPLICATES ALL_SRC)
list(LENGTH ALL_SRC SRC_COUNT)
message(STATUS "Found ${SRC_COUNT} source files")

if (SRC_COUNT EQUAL 0)
    message(FATAL_ERROR "No source files found!")
endif()

# Create executable
add_executable(arksigning ${ALL_SRC})

# Set include directories
target_include_directories(arksigning PRIVATE
    ${OPENSSL_INCLUDE_DIR}
    ${ZLIB_INCLUDE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/include/arksigning
)

# Link libraries
target_link_libraries(arksigning ${LIB_LIST})

# Set compiler flags
target_compile_options(arksigning PRIVATE -Wall -Wextra)

# Installation
install(TARGETS arksigning DESTINATION bin)

# Print build summary
message(STATUS "")
message(STATUS "=== Build Configuration Summary ===")
message(STATUS "Project: ${PROJECT_NAME}")
message(STATUS "C++ Standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "Build Type: ${CMAKE_BUILD_TYPE}")
message(STATUS "OpenSSL Root: ${OPENSSL_ROOT_DIR}")
message(STATUS "OpenSSL Include: ${OPENSSL_INCLUDE_DIR}")
message(STATUS "OpenSSL Libraries: ${OPENSSL_LIBRARIES}")
message(STATUS "ZLIB Include: ${ZLIB_INCLUDE_DIR}")
message(STATUS "ZLIB Libraries: ${ZLIB_LIBRARIES}")
message(STATUS "Source Files: ${SRC_COUNT}")
message(STATUS "===================================")
message(STATUS "")
