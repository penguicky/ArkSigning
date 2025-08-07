# ArkSigning Build Guide

This document provides comprehensive build instructions and troubleshooting for ArkSigning.

## 📋 Prerequisites

### System Requirements

- **C++ Compiler**: GCC 4.8+ or Clang 3.3+
- **CMake**: Version 3.14 or higher
- **OpenSSL**: Version 1.1.0 or higher (3.x recommended)
- **zlib**: Development libraries
- **Git**: For cloning the repository

### Platform-Specific Dependencies

#### macOS
```bash
# Using Homebrew (recommended)
brew install openssl@3 cmake git

# Verify installation
brew --prefix openssl@3
```

#### Ubuntu/Debian
```bash
sudo apt-get update
sudo apt-get install build-essential cmake libssl-dev zlib1g-dev git pkg-config
```

#### CentOS/RHEL/Fedora
```bash
# CentOS/RHEL
sudo yum groupinstall "Development Tools"
sudo yum install cmake openssl-devel zlib-devel git

# Fedora
sudo dnf groupinstall "Development Tools"
sudo dnf install cmake openssl-devel zlib-devel git
```

## 🔨 Build Methods

### Method 1: Makefile (Recommended)

The Makefile provides automatic dependency detection and cross-platform support:

```bash
# Clone repository
git clone https://github.com/penguicky/ArkSigning.git
cd ArkSigning

# Build with automatic configuration
make

# Check detected configuration
make check-deps

# Clean build artifacts
make clean

# Install system-wide
sudo make install
```

#### Makefile Options

```bash
# Use different compiler
make CXX=clang++

# Add debug symbols
make CXXFLAGS="-g -O0"

# Install to custom location
make install DESTDIR=$HOME/.local

# Show help
make help
```

### Method 2: CMake

```bash
mkdir build && cd build
cmake ..
make

# With specific build type
cmake -DCMAKE_BUILD_TYPE=Release ..
make

# Install
sudo make install
```

### Method 3: Automated Script

```bash
chmod +x INSTALL.sh
./INSTALL.sh
```

## 🔍 Troubleshooting

### OpenSSL Issues

#### Problem: "openssl/pem.h not found"

**Solution for macOS:**
```bash
# Install OpenSSL 3.x
brew install openssl@3

# Verify installation
brew --prefix openssl@3

# If using Makefile, it should auto-detect
make clean && make

# For manual compilation
OPENSSL_PATH=$(brew --prefix openssl@3)
g++ *.cpp common/*.cpp -lcrypto -lz \
    -I$OPENSSL_PATH/include -L$OPENSSL_PATH/lib \
    -std=c++11 -O3 -o arksigning
```

**Solution for Linux:**
```bash
# Ubuntu/Debian
sudo apt-get install libssl-dev

# CentOS/RHEL
sudo yum install openssl-devel

# Verify pkg-config can find OpenSSL
pkg-config --cflags --libs openssl
```

#### Problem: Wrong OpenSSL version detected

**Check current version:**
```bash
# macOS
brew list | grep openssl
brew --prefix openssl@3

# Linux
pkg-config --modversion openssl
openssl version
```

**Force specific OpenSSL path:**
```bash
# Set environment variables
export OPENSSL_ROOT_DIR=/path/to/openssl
export PKG_CONFIG_PATH=/path/to/openssl/lib/pkgconfig:$PKG_CONFIG_PATH

# Then build
make clean && make
```

### Compilation Issues

#### Problem: "No such file or directory" for headers

**Check source files:**
```bash
ls -la *.cpp common/*.cpp
```

**Verify include paths:**
```bash
make check-deps
```

#### Problem: Linker errors

**Common solutions:**
```bash
# Missing zlib
sudo apt-get install zlib1g-dev  # Ubuntu
sudo yum install zlib-devel      # CentOS

# Missing crypto library
sudo apt-get install libssl-dev  # Ubuntu
sudo yum install openssl-devel   # CentOS
```

### Platform-Specific Issues

#### macOS Apple Silicon vs Intel

The build system automatically detects your architecture:

```bash
# Check detected paths
make check-deps

# Apple Silicon paths: /opt/homebrew/opt/openssl@3
# Intel paths: /usr/local/opt/openssl@3
```

#### Linux Distribution Differences

**Package names vary:**
- Ubuntu/Debian: `libssl-dev`, `zlib1g-dev`
- CentOS/RHEL: `openssl-devel`, `zlib-devel`
- Fedora: `openssl-devel`, `zlib-devel`

### Build Performance

#### Parallel Compilation

```bash
# Makefile (auto-detects CPU cores)
make -j$(nproc)

# CMake
make -j$(nproc)

# Manual specification
make -j4  # Use 4 cores
```

## 🧪 Testing the Build

### Basic Functionality Test

```bash
# Show help
./arksigning --help

# Show version
./arksigning --version

# Test with a sample app (if available)
./arksigning --info sample.ipa
```

### Dependency Verification

```bash
# Check linked libraries (Linux)
ldd arksigning

# Check linked libraries (macOS)
otool -L arksigning

# Verify OpenSSL linkage
strings arksigning | grep -i openssl
```

## 🚀 Optimization

### Release Build

```bash
# CMake
cmake -DCMAKE_BUILD_TYPE=Release ..
make

# Makefile (already optimized with -O3)
make
```

### Static Linking (Advanced)

For creating portable binaries:

```bash
# Linux static build
g++ *.cpp common/*.cpp -static -lcrypto -lz -ldl -lpthread \
    -std=c++11 -O3 -o arksigning-static

# Note: Static linking with OpenSSL can be complex
# Consider using the provided Docker method for portable binaries
```

## 📦 Packaging

### Creating Distribution Package

```bash
# Build release version
make clean && make

# Create tarball
tar -czf arksigning-$(uname -s)-$(uname -m).tar.gz arksigning README.md LICENSE

# Create installer package (macOS)
pkgbuild --root . --identifier com.arksigning.pkg arksigning.pkg
```

## 🔧 Development Setup

### Debug Build

```bash
# CMake debug build
cmake -DCMAKE_BUILD_TYPE=Debug ..
make

# Makefile with debug flags
make CXXFLAGS="-g -O0 -DDEBUG"
```

### Code Analysis

```bash
# Static analysis with clang
make CXX=clang++ CXXFLAGS="-Wall -Wextra -Wanalyzer"

# Memory checking with valgrind (Linux)
valgrind --leak-check=full ./arksigning --help
```

## 📞 Getting Help

If you encounter issues not covered here:

1. Check the [main README](README.md) for basic usage
2. Search existing [GitHub Issues](https://github.com/YOUR_USERNAME/ArkSigning/issues)
3. Create a new issue with:
   - Your OS and version
   - Build method used
   - Complete error output
   - Output of `make check-deps`

## 🤝 Contributing

When contributing build system improvements:

1. Test on multiple platforms
2. Update this documentation
3. Ensure backward compatibility
4. Add appropriate error handling
