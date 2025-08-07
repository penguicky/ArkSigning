# ArkSigning Project Structure

This document describes the organized directory structure of the ArkSigning project after modernization and cleanup.

## 📁 Directory Overview

```
ArkSigning/
├── 📁 src/                     # Source code (organized by component)
│   ├── 📁 core/               # Core application logic
│   ├── 📁 crypto/             # Cryptographic operations  
│   └── 📁 utils/              # Utility functions
├── 📁 include/arksigning/     # Headers (organized by component)
│   ├── 📁 core/               # Core component headers
│   ├── 📁 crypto/             # Cryptographic headers
│   ├── 📁 utils/              # Utility headers
│   ├── 📁 modern/             # Modern C++ features
│   └── 📄 arksigning.h        # Master header file
├── 📁 docs/                   # Documentation
├── 📁 tools/                  # Build tools and scripts
├── 📁 build/                  # Build artifacts (generated)
├── 📄 Makefile                # Primary build system
├── 📄 CMakeLists.txt          # CMake build configuration
├── 📄 xmake.lua               # XMake build configuration
├── 📄 .gitignore              # Git ignore rules
└── 📄 README.md               # Main project documentation
```

## 🔧 Source Code Organization

### **Core Components** (`src/core/`)

Contains the main application logic and processing components:

| File | Purpose | Description |
|------|---------|-------------|
| `zsign.cpp` | Main application | Entry point and command-line interface |
| `bundle.cpp` | App bundle processing | iOS app bundle manipulation |
| `macho.cpp` | Mach-O binary handling | Binary format processing |
| `archo.cpp` | Archive operations | ZIP/IPA archive handling |
| `signing.cpp` | Code signing logic | Digital signature operations |

### **Cryptographic Components** (`src/crypto/`)

Contains cryptographic operations and security-related functionality:

| File | Purpose | Description |
|------|---------|-------------|
| `openssl.cpp` | OpenSSL integration | Cryptographic operations and certificate handling |

### **Utility Components** (`src/utils/`)

Contains utility functions and data structures:

| File | Purpose | Description |
|------|---------|-------------|
| `common.cpp` | Common utilities | File operations, string manipulation, system utilities |
| `base64.cpp` | Base64 encoding | Base64 encoding/decoding with modern C++ features |
| `json.cpp` | JSON processing | JSON parsing and generation with move semantics |

## 📋 Header Organization

### **Core Headers** (`include/arksigning/core/`)

| File | Purpose | Exports |
|------|---------|---------|
| `bundle.h` | App bundle processing | Bundle manipulation functions |
| `macho.h` | Mach-O binary handling | Binary format structures and functions |
| `archo.h` | Archive operations | Archive handling utilities |
| `signing.h` | Code signing logic | Signing operations and structures |

### **Cryptographic Headers** (`include/arksigning/crypto/`)

| File | Purpose | Exports |
|------|---------|---------|
| `openssl.h` | OpenSSL integration | Crypto functions and certificate operations |
| `openssl_raii.h` | RAII wrappers | Smart wrappers for OpenSSL resources |

### **Utility Headers** (`include/arksigning/utils/`)

| File | Purpose | Exports |
|------|---------|---------|
| `common.h` | Common utilities | File operations, system utilities, optional APIs |
| `base64.h` | Base64 encoding | Encoding/decoding classes with move semantics |
| `json.h` | JSON processing | JSON parsing classes with modern C++ features |
| `constants.h` | Application constants | Compile-time constants and definitions |
| `mach-o.h` | Mach-O definitions | Binary format structures and constants |

### **Modern C++ Features** (`include/arksigning/modern/`)

| File | Purpose | Exports |
|------|---------|---------|
| `optional.h` | Optional type | C++11-compatible std::optional implementation |
| `types.h` | Type system | Strongly-typed enums and type aliases |
| `utility.h` | Modern utilities | Perfect forwarding and move utilities |
| `callbacks.h` | Callback system | Lambda-based callback management |

## 🏗️ Build System

### **Primary Build System** (`Makefile`)

- **Auto-detection**: Automatically detects platform and OpenSSL installation
- **Organized sources**: Uses component-based source organization
- **Include paths**: Properly configured for new header structure
- **Zero warnings**: Strict warning flags with clean compilation

### **CMake Support** (`CMakeLists.txt`)

- **Cross-platform**: Works on macOS, Linux, and Windows
- **Dependency management**: Automatic OpenSSL and zlib detection
- **Modern CMake**: Uses target-based configuration
- **IDE support**: Generates project files for various IDEs

### **XMake Support** (`xmake.lua`)

- **Fast builds**: Optimized build system
- **Package management**: Integrated dependency management
- **Cross-compilation**: Support for multiple targets

## 📚 Documentation

### **Main Documentation** (`docs/`)

| File | Purpose | Content |
|------|---------|---------|
| `README.md` | Main documentation | Project overview, features, usage |
| `BUILD.md` | Build instructions | Comprehensive build guide |
| `MODERNIZATION.md` | Technical analysis | Modernization report and improvements |
| `STRUCTURE.md` | This document | Project organization guide |
| `LICENSE` | Legal information | MIT license terms |

## 🛠️ Tools and Scripts

### **Build Tools** (`tools/`)

| File | Purpose | Description |
|------|---------|-------------|
| `update_includes.sh` | Include path updater | Script to update include paths after reorganization |
| `Dockerfile` | Container build | Docker configuration for containerized builds |
| `INSTALL.sh` | Installation script | Automated installation script |
| `signervip.patch` | Legacy patch | Compatibility patch for legacy systems |

## 🎯 Design Principles

### **Separation of Concerns**

- **Core**: Application logic and main functionality
- **Crypto**: Security and cryptographic operations
- **Utils**: Reusable utilities and data structures
- **Modern**: Modern C++ features and enhancements

### **Header Organization**

- **Logical grouping**: Headers grouped by functionality
- **Clear dependencies**: Minimal cross-component dependencies
- **Master header**: Single include for all functionality
- **Forward declarations**: Reduced compilation dependencies

### **Build System Design**

- **Component-based**: Source files organized by component
- **Flexible includes**: Multiple include path strategies
- **Clean separation**: Build artifacts isolated from source
- **Cross-platform**: Works across different platforms

## 🔄 Migration Guide

### **From Old Structure**

If you're migrating from the old flat structure:

1. **Update includes**: Use the new component-based paths
2. **Build system**: Use updated Makefile or CMake
3. **Documentation**: Refer to new docs/ directory
4. **Tools**: Use scripts in tools/ directory

### **Include Path Changes**

| Old Path | New Path | Component |
|----------|----------|-----------|
| `common/common.h` | `utils/common.h` | Utilities |
| `common/optional.h` | `modern/optional.h` | Modern C++ |
| `openssl.h` | `crypto/openssl.h` | Cryptography |
| `bundle.h` | `core/bundle.h` | Core logic |

## 📈 Benefits

### **Improved Organization**

- **Clear structure**: Easy to navigate and understand
- **Logical grouping**: Related functionality grouped together
- **Reduced clutter**: Build artifacts separated from source
- **Professional layout**: Enterprise-grade project organization

### **Better Maintainability**

- **Component isolation**: Changes isolated to specific components
- **Clear dependencies**: Easy to understand component relationships
- **Modular design**: Components can be developed independently
- **Documentation**: Comprehensive documentation for all aspects

### **Enhanced Development**

- **IDE support**: Better IDE navigation and project management
- **Build efficiency**: Faster builds with organized dependencies
- **Code reuse**: Clear component boundaries enable better reuse
- **Testing**: Component-based testing strategies

---

This organized structure makes ArkSigning a professional, maintainable, and scalable codebase that demonstrates modern C++ best practices while maintaining excellent functionality and performance.
