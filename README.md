# ArkSigning

[![C++11](https://img.shields.io/badge/C%2B%2B-11-blue.svg)](https://en.wikipedia.org/wiki/C%2B%2B11)
[![Build Status](https://img.shields.io/badge/build-passing-brightgreen.svg)](https://github.com/penguicky/ArkSigning)
[![License](https://img.shields.io/badge/license-MIT-blue.svg)](docs/LICENSE)
[![Modern C++](https://img.shields.io/badge/Modern%20C%2B%2B-✓-green.svg)](docs/MODERNIZATION.md)

A **modernized, enterprise-grade iOS app signing tool** based on zsign, featuring enhanced security, parallel processing, and modern C++ best practices.

## 🚀 Quick Start

```bash
# Build the project
make

# Sign an app
./arksigning -k dev.p12 -p password -m dev.prov -o signed.ipa app.ipa

# Bulk sign with parallel processing
./arksigning --bulk --inputfolder ./apps --outputfolder ./signed \
    -k dev.p12 -p password -m dev.prov --parallel
```

## ✨ Key Features

### 🔧 **Core Functionality**
- **iOS App Signing** - Sign .ipa files and app bundles
- **Bulk Processing** - Sign multiple apps in parallel
- **Dylib Injection** - Inject dynamic libraries into apps
- **Provisioning Profiles** - Automatic profile management
- **Code Directory** - Generate and validate code signatures

### 🏗️ **Modern Architecture**
- **C++11 Compliant** - Modern C++ features throughout
- **Zero Warnings** - Clean compilation with strict warnings
- **Memory Safe** - RAII and smart pointer usage
- **Type Safe** - Strong typing with enum classes
- **Thread Safe** - Parallel processing with modern concurrency

### 🛡️ **Security & Reliability**
- **OpenSSL Integration** - Industry-standard cryptography
- **Error Handling** - Comprehensive error reporting
- **Input Validation** - Robust input sanitization
- **Resource Management** - Automatic cleanup and leak prevention

## 📁 Project Structure

```
ArkSigning/
├── src/                    # Source code
│   ├── core/              # Core application logic
│   ├── crypto/            # Cryptographic operations
│   └── utils/             # Utility functions
├── include/arksigning/    # Headers
│   ├── core/              # Core headers
│   ├── crypto/            # Crypto headers
│   ├── utils/             # Utility headers
│   └── modern/            # Modern C++ features
├── docs/                  # Documentation
├── tools/                 # Build tools and scripts
└── build/                 # Build artifacts (generated)
```

## 🔨 Building

### Prerequisites

**macOS:**
```bash
brew install openssl cmake
```

**Ubuntu/Debian:**
```bash
sudo apt-get install build-essential libssl-dev zlib1g-dev cmake
```

### Build Methods

**Using Makefile (Recommended):**
```bash
make                    # Build with auto-detected settings
make clean             # Clean build artifacts
make install           # Install to system (optional)
```

**Using CMake:**
```bash
mkdir build && cd build
cmake ..
make -j$(nproc)
```

## 📖 Usage

### Basic Signing
```bash
# Sign with certificate and provisioning profile
./arksigning -k certificate.p12 -p password -m profile.prov -o signed.ipa app.ipa

# Sign with custom bundle ID
./arksigning -k cert.p12 -p pass -m profile.prov -b com.example.newid -o signed.ipa app.ipa
```

### Advanced Features
```bash
# Inject dylib and sign
./arksigning -k cert.p12 -p pass -m profile.prov -l library.dylib -o signed.ipa app.ipa

# Bulk signing with parallel processing
./arksigning --bulk --inputfolder ./unsigned --outputfolder ./signed \
    -k cert.p12 -p pass -m profile.prov --parallel 8

# Get app information
./arksigning --info app.ipa
```

## 🏆 Modern C++ Features

This project showcases **enterprise-grade modern C++** practices:

- **🔒 Type Safety** - `std::optional`, `enum class`, strong typing
- **⚡ Performance** - Move semantics, perfect forwarding, RAII
- **🧵 Concurrency** - Lambda expressions, thread-safe containers
- **🛡️ Memory Safety** - Smart pointers, automatic resource management
- **📐 Clean Code** - Consistent style, comprehensive documentation

See [MODERNIZATION.md](docs/MODERNIZATION.md) for detailed technical analysis.

## 📚 Documentation

- **[Build Guide](docs/BUILD.md)** - Comprehensive build instructions
- **[Modernization Report](docs/MODERNIZATION.md)** - Technical improvements made
- **[API Reference](docs/API.md)** - Function and class documentation

## 🤝 Contributing

1. Fork the repository
2. Create a feature branch (`git checkout -b feature/amazing-feature`)
3. Commit your changes (`git commit -m 'Add amazing feature'`)
4. Push to the branch (`git push origin feature/amazing-feature`)
5. Open a Pull Request

## 📄 License

This project is licensed under the MIT License - see the [LICENSE](docs/LICENSE) file for details.

## 🙏 Acknowledgments

- Based on the original [zsign](https://github.com/zhlynn/zsign) project
- Enhanced with modern C++ practices and enterprise-grade features
- OpenSSL for cryptographic operations
- Community contributors and testers

---

**ArkSigning** - *Professional iOS app signing with modern C++ excellence* 🚀
