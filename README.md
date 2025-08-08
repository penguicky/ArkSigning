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

### Command Line Syntax
```bash
Usage: arksigning [-options] [-k privkey.pem] [-m dev.prov] [-o output.ipa] file|folder
```

### 📋 Complete Options Reference

#### **Core Signing Options**
| Option | Long Form | Argument | Description |
|--------|-----------|----------|-------------|
| `-k` | `--pkey` | `<file>` | Path to private key or P12 certificate file (PEM or DER format) |
| `-m` | `--prov` | `<file>` | Path to mobile provisioning profile (.mobileprovision) |
| `-c` | `--cert` | `<file>` | Path to certificate file (PEM or DER format) |
| `-o` | `--output` | `<file>` | Path to output IPA file |
| `-p` | `--password` | `<string>` | Password for private key or P12 file |

#### **App Modification Options**
| Option | Long Form | Argument | Description |
|--------|-----------|----------|-------------|
| `-b` | `--bundle_id` | `<string>` | New bundle identifier to change (e.g., com.example.app) |
| `-n` | `--bundle_name` | `<string>` | New bundle display name to change |
| `-r` | `--bundle_version` | `<string>` | New bundle version to change |
| `-e` | `--entitlements` | `<file>` | Path to new entitlements file to apply |

#### **Dylib Injection Options**
| Option | Long Form | Argument | Description |
|--------|-----------|----------|-------------|
| `-l` | `--dylib` | `<file>` | Path to dylib file to inject (can be used multiple times) |
| `-w` | `--weak` | - | Inject dylib as LC_LOAD_WEAK_DYLIB instead of LC_LOAD_DYLIB |

#### **Output & Processing Options**
| Option | Long Form | Argument | Description |
|--------|-----------|----------|-------------|
| `-z` | `--zip_level` | `<0-9>` | Compression level for output IPA file (0=no compression, 9=max) |
| `-f` | `--force` | - | Force sign without cache when signing folder |
| `-d` | `--debug` | - | Generate debug output files (.arksigning_debug folder) |
| `-E` | `--no-embed-profile` | - | Don't generate embedded mobile provisioning profile |

#### **Bulk Signing Options**
| Option | Long Form | Argument | Description |
|--------|-----------|----------|-------------|
| `-B` | `--bulk` | - | Enable bulk signing mode for multiple apps |
| | `--inputfolder` | `<folder>` | Folder containing unsigned apps to process |
| | `--outputfolder` | `<folder>` | Destination folder for signed apps |
| | `--parallel` | `[count]` | Enable parallel processing (optional thread count) |
| | `--base-url` | `<url>` | Base URL for generating OTA installation links |

#### **Information & Utility Options**
| Option | Long Form | Argument | Description |
|--------|-----------|----------|-------------|
| `-I` | `--info` | - | Output app information in JSON format with base64 icon |
| `-i` | `--install` | - | Install IPA file using ideviceinstaller for testing |
| `-q` | `--quiet` | - | Quiet operation (suppress non-error output) |
| `-v` | `--version` | - | Show version information |
| `-h` | `--help` | - | Show help message and exit |

### 🚀 Basic Usage Examples

#### **Simple App Signing**
```bash
# Sign with P12 certificate and provisioning profile
./arksigning -k developer.p12 -p "mypassword" -m development.mobileprovision -o signed.ipa MyApp.ipa

# Sign with separate private key and certificate
./arksigning -k private_key.pem -c certificate.pem -m profile.mobileprovision -o signed.ipa MyApp.ipa

# Sign app folder directly (faster for development)
./arksigning -k dev.p12 -p "pass123" -m dev.mobileprovision -o output.ipa ./MyApp.app/
```

#### **App Information & Analysis**
```bash
# Get detailed app information in JSON format
./arksigning --info MyApp.ipa

# Analyze Mach-O binary structure
./arksigning MyApp.app/MyApp

# Get app info with quiet output (JSON only)
./arksigning --info --quiet MyApp.ipa
```

### 🔧 Advanced Usage Examples

#### **App Modification & Customization**
```bash
# Change bundle ID and app name
./arksigning -k dev.p12 -p "password" -m profile.mobileprovision \
    -b "com.mycompany.newapp" -n "My Custom App" -o customized.ipa original.ipa

# Update bundle version and apply custom entitlements
./arksigning -k cert.p12 -p "pass" -m profile.mobileprovision \
    -r "2.1.0" -e custom_entitlements.plist -o updated.ipa app.ipa

# Sign without embedding provisioning profile (for enterprise distribution)
./arksigning -k enterprise.p12 -p "password" -m enterprise.mobileprovision \
    -E -o enterprise.ipa MyApp.ipa
```

#### **Dylib Injection & Modification**
```bash
# Inject single dylib with strong reference
./arksigning -k dev.p12 -p "password" -m profile.mobileprovision \
    -l "@executable_path/Frameworks/MyFramework.dylib" -o injected.ipa app.ipa

# Inject multiple dylibs at once
./arksigning -k dev.p12 -p "password" -m profile.mobileprovision \
    -l "framework1.dylib" -l "framework2.dylib" -l "library.dylib" -o multi.ipa app.ipa

# Inject dylib with weak reference (won't crash if missing)
./arksigning -k dev.p12 -p "password" -m profile.mobileprovision \
    -w -l "@executable_path/OptionalFramework.dylib" -o weak.ipa app.ipa

# Inject dylib directly into Mach-O binary (no re-signing)
./arksigning -l "@executable_path/Frameworks/MyLib.dylib" MyApp.app/MyApp
```

#### **Bulk Signing Operations**
```bash
# Basic bulk signing with auto-detected thread count
./arksigning --bulk --inputfolder ./unsigned_apps --outputfolder ./signed_apps \
    -k developer.p12 -p "password" -m development.mobileprovision --parallel

# Bulk signing with specific thread count for performance tuning
./arksigning --bulk --inputfolder ./apps --outputfolder ./signed \
    -k cert.p12 -p "pass" -m profile.mobileprovision --parallel 4

# Bulk signing with OTA installation link generation
./arksigning --bulk --inputfolder ./apps --outputfolder ./signed \
    -k dev.p12 -p "password" -m profile.mobileprovision \
    --base-url "https://myserver.com/apps/" --parallel

# Bulk signing with custom bundle modifications
./arksigning --bulk --inputfolder ./apps --outputfolder ./signed \
    -k cert.p12 -p "pass" -m profile.mobileprovision \
    -b "com.company.prefix" --parallel 8
```

#### **Development & Testing Workflows**
```bash
# Sign and immediately install for testing (requires ideviceinstaller)
./arksigning -k dev.p12 -p "password" -m dev.mobileprovision \
    -o test.ipa -i MyApp.ipa

# Development signing with debug output and maximum compression
./arksigning -k dev.p12 -p "password" -m dev.mobileprovision \
    -d -z 9 -o debug.ipa MyApp.ipa

# Force re-signing without cache (useful when certificates change)
./arksigning -f -k new_cert.p12 -p "newpass" -m new_profile.mobileprovision \
    -o resigned.ipa MyApp.app/

# Quiet signing for automated scripts
./arksigning --quiet -k cert.p12 -p "pass" -m profile.mobileprovision \
    -o output.ipa input.ipa
```

#### **Production & Enterprise Workflows**
```bash
# Enterprise distribution signing
./arksigning -k enterprise.p12 -p "enterprise_password" \
    -m enterprise.mobileprovision -E -z 6 -o enterprise.ipa MyApp.ipa

# App Store distribution preparation
./arksigning -k distribution.p12 -p "dist_password" \
    -m appstore.mobileprovision -z 9 -o appstore.ipa MyApp.ipa

# Ad-hoc distribution with device-specific provisioning
./arksigning -k adhoc.p12 -p "password" -m adhoc_devices.mobileprovision \
    -o adhoc.ipa MyApp.ipa
```

### 📊 Performance & Optimization Tips

#### **Signing Performance**
```bash
# Use app folders instead of IPAs for faster development cycles
unzip MyApp.ipa -d extracted/
./arksigning -k dev.p12 -p "pass" -m profile.mobileprovision \
    -o fast.ipa extracted/Payload/MyApp.app/

# Leverage caching for repeated signing (don't use -f flag)
./arksigning -k cert.p12 -p "pass" -m profile.mobileprovision \
    -o cached1.ipa MyApp.app/  # Creates cache
./arksigning -k cert.p12 -p "pass" -m profile.mobileprovision \
    -o cached2.ipa MyApp.app/  # Uses cache (much faster)

# Optimize compression for different use cases
./arksigning -z 0 -k cert.p12 -p "pass" -m profile.mobileprovision \
    -o fast_build.ipa MyApp.ipa  # No compression (fastest)
./arksigning -z 9 -k cert.p12 -p "pass" -m profile.mobileprovision \
    -o small_size.ipa MyApp.ipa  # Maximum compression (smallest)
```

#### **Bulk Processing Optimization**
```bash
# Optimal thread count (usually equals CPU cores)
nproc_count=$(nproc)
./arksigning --bulk --inputfolder ./apps --outputfolder ./signed \
    -k cert.p12 -p "pass" -m profile.mobileprovision --parallel $nproc_count

# Memory-conscious bulk signing for large numbers of apps
./arksigning --bulk --inputfolder ./apps --outputfolder ./signed \
    -k cert.p12 -p "pass" -m profile.mobileprovision --parallel 2 --quiet
```

## 🏆 Modern C++ Features

This project showcases **enterprise-grade modern C++** practices:

- **🔒 Type Safety** - `std::optional`, `enum class`, strong typing
- **⚡ Performance** - Move semantics, perfect forwarding, RAII
- **🧵 Concurrency** - Lambda expressions, thread-safe containers
- **🛡️ Memory Safety** - Smart pointers, automatic resource management
- **📐 Clean Code** - Consistent style, comprehensive documentation

See [MODERNIZATION.md](docs/MODERNIZATION.md) for detailed technical analysis.

### 🔍 Real-World Usage Scenarios

#### **Scenario 1: iOS Developer Testing**
```bash
# Daily development workflow
./arksigning -k ~/Certificates/ios_development.p12 -p "dev_password" \
    -m ~/Profiles/MyApp_Development.mobileprovision \
    -o ~/Desktop/MyApp_$(date +%Y%m%d).ipa ~/Projects/MyApp/build/MyApp.ipa

# Quick testing with automatic installation
./arksigning -k dev.p12 -p "pass" -m dev.mobileprovision \
    -i -o test.ipa MyApp.ipa
```

#### **Scenario 2: Enterprise App Distribution**
```bash
# Enterprise signing with OTA distribution
./arksigning -k enterprise.p12 -p "enterprise_pass" \
    -m enterprise.mobileprovision -E \
    -b "com.company.internal.app" -n "Internal Tool v2.1" \
    -o enterprise_app.ipa MyApp.ipa

# Generate installation link
echo "Install via: itms-services://?action=download-manifest&url=https://company.com/apps/manifest.plist"
```

#### **Scenario 3: App Store Preparation**
```bash
# Final App Store build with maximum compression
./arksigning -k distribution.p12 -p "store_password" \
    -m appstore.mobileprovision -z 9 \
    -r "1.2.3" -o MyApp_AppStore_v1.2.3.ipa MyApp.ipa

# Verify the signed app
./arksigning --info MyApp_AppStore_v1.2.3.ipa
```

#### **Scenario 4: Continuous Integration Pipeline**
```bash
#!/bin/bash
# CI/CD signing script
set -e

APP_NAME="MyApp"
VERSION=$(git describe --tags --always)
CERT_PATH="/secure/certificates/ios_distribution.p12"
PROFILE_PATH="/secure/profiles/appstore.mobileprovision"
CERT_PASSWORD="$IOS_CERT_PASSWORD"  # From environment

echo "Signing $APP_NAME version $VERSION..."
./arksigning --quiet \
    -k "$CERT_PATH" -p "$CERT_PASSWORD" \
    -m "$PROFILE_PATH" \
    -r "$VERSION" \
    -z 9 \
    -o "artifacts/${APP_NAME}_${VERSION}.ipa" \
    "build/${APP_NAME}.ipa"

echo "Signing completed successfully!"
```

#### **Scenario 5: Security Research & Analysis**
```bash
# Inject research framework for analysis
./arksigning -k research.p12 -p "research_pass" \
    -m research.mobileprovision \
    -l "@executable_path/Frameworks/AnalysisFramework.dylib" \
    -l "@executable_path/Frameworks/HookingLibrary.dylib" \
    -b "com.research.modified.app" \
    -o analyzed_app.ipa original_app.ipa

# Extract and analyze app information
./arksigning --info analyzed_app.ipa > app_analysis.json
```

### 🛠️ Troubleshooting Guide

#### **Common Issues & Solutions**

**Issue: "Invalid private key or certificate"**
```bash
# Verify P12 file integrity
openssl pkcs12 -info -in certificate.p12 -noout

# Check certificate expiration
./arksigning --info certificate.p12

# Use correct password
./arksigning -k cert.p12 -p "correct_password" -m profile.mobileprovision -o output.ipa app.ipa
```

**Issue: "Provisioning profile doesn't match"**
```bash
# Check profile details
security cms -D -i profile.mobileprovision

# Ensure bundle ID matches
./arksigning -k cert.p12 -p "pass" -m profile.mobileprovision \
    -b "com.exact.bundle.id" -o output.ipa app.ipa
```

**Issue: "Code signing failed"**
```bash
# Enable debug output to see detailed errors
./arksigning -d -k cert.p12 -p "pass" -m profile.mobileprovision -o output.ipa app.ipa

# Check debug folder for detailed logs
ls -la .arksigning_debug/
```

**Issue: "Bulk signing fails on some apps"**
```bash
# Use single-threaded mode for debugging
./arksigning --bulk --inputfolder ./apps --outputfolder ./signed \
    -k cert.p12 -p "pass" -m profile.mobileprovision --parallel 1

# Check individual app compatibility
for app in ./apps/*.ipa; do
    echo "Testing: $app"
    ./arksigning --info "$app" || echo "Failed: $app"
done
```

#### **Performance Troubleshooting**

**Slow signing performance:**
```bash
# Use app folders instead of IPAs
unzip app.ipa -d extracted/
./arksigning -k cert.p12 -p "pass" -m profile.mobileprovision \
    -o fast.ipa extracted/Payload/App.app/

# Reduce compression for faster builds
./arksigning -z 1 -k cert.p12 -p "pass" -m profile.mobileprovision -o fast.ipa app.ipa
```

**Memory issues with bulk signing:**
```bash
# Reduce thread count
./arksigning --bulk --inputfolder ./apps --outputfolder ./signed \
    -k cert.p12 -p "pass" -m profile.mobileprovision --parallel 2

# Process in smaller batches
mkdir batch1 batch2
mv apps1-50.ipa batch1/
mv apps51-100.ipa batch2/
# Process each batch separately
```

### 📁 File Format Examples & Requirements

#### **Certificate Files (.p12, .pem, .der)**
```bash
# P12 format (most common)
./arksigning -k ios_development.p12 -p "password123" -m profile.mobileprovision -o signed.ipa app.ipa

# PEM format (separate key and certificate)
./arksigning -k private_key.pem -c certificate.pem -m profile.mobileprovision -o signed.ipa app.ipa

# DER format
./arksigning -k certificate.der -m profile.mobileprovision -o signed.ipa app.ipa
```

#### **Provisioning Profiles (.mobileprovision)**
```bash
# Development profile
./arksigning -k dev.p12 -p "pass" -m "MyApp_Development.mobileprovision" -o dev.ipa app.ipa

# Distribution profile
./arksigning -k dist.p12 -p "pass" -m "MyApp_AppStore.mobileprovision" -o store.ipa app.ipa

# Enterprise profile
./arksigning -k ent.p12 -p "pass" -m "MyApp_Enterprise.mobileprovision" -E -o ent.ipa app.ipa
```

#### **Entitlements Files (.plist)**
```xml
<!-- custom_entitlements.plist -->
<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE plist PUBLIC "-//Apple//DTD PLIST 1.0//EN" "http://www.apple.com/DTDs/PropertyList-1.0.dtd">
<plist version="1.0">
<dict>
    <key>application-identifier</key>
    <string>TEAMID.com.example.app</string>
    <key>com.apple.developer.team-identifier</key>
    <string>TEAMID</string>
    <key>get-task-allow</key>
    <true/>
    <key>com.apple.security.application-groups</key>
    <array>
        <string>group.com.example.shared</string>
    </array>
</dict>
</plist>
```

```bash
# Apply custom entitlements
./arksigning -k dev.p12 -p "pass" -m profile.mobileprovision \
    -e custom_entitlements.plist -o custom.ipa app.ipa
```

#### **Dylib Injection Paths**
```bash
# Absolute path injection
./arksigning -k cert.p12 -p "pass" -m profile.mobileprovision \
    -l "/usr/lib/system/libsystem_c.dylib" -o injected.ipa app.ipa

# Relative path injection (recommended)
./arksigning -k cert.p12 -p "pass" -m profile.mobileprovision \
    -l "@executable_path/Frameworks/MyFramework.dylib" -o injected.ipa app.ipa

# Loader path injection
./arksigning -k cert.p12 -p "pass" -m profile.mobileprovision \
    -l "@loader_path/../Frameworks/SharedLib.dylib" -o injected.ipa app.ipa

# rpath injection
./arksigning -k cert.p12 -p "pass" -m profile.mobileprovision \
    -l "@rpath/DynamicFramework.dylib" -o injected.ipa app.ipa
```

### ✅ Argument Validation & Best Practices

#### **Required Arguments Validation**
```bash
# ❌ Missing required arguments
./arksigning app.ipa
# Error: Missing certificate (-k) and provisioning profile (-m)

# ❌ Invalid file paths
./arksigning -k nonexistent.p12 -m profile.mobileprovision -o output.ipa app.ipa
# Error: Certificate file not found

# ✅ Correct minimal usage
./arksigning -k cert.p12 -p "password" -m profile.mobileprovision -o signed.ipa app.ipa
```

#### **Bundle ID Format Validation**
```bash
# ✅ Valid bundle IDs
./arksigning -k cert.p12 -p "pass" -m profile.mobileprovision \
    -b "com.company.app" -o output.ipa app.ipa

./arksigning -k cert.p12 -p "pass" -m profile.mobileprovision \
    -b "com.company.app.extension" -o output.ipa app.ipa

# ❌ Invalid bundle ID formats
./arksigning -k cert.p12 -p "pass" -m profile.mobileprovision \
    -b "invalid..bundle.id" -o output.ipa app.ipa  # Double dots

./arksigning -k cert.p12 -p "pass" -m profile.mobileprovision \
    -b ".com.company.app" -o output.ipa app.ipa  # Leading dot
```

#### **Compression Level Validation**
```bash
# ✅ Valid compression levels (0-9)
./arksigning -z 0 -k cert.p12 -p "pass" -m profile.mobileprovision -o fast.ipa app.ipa     # No compression
./arksigning -z 6 -k cert.p12 -p "pass" -m profile.mobileprovision -o balanced.ipa app.ipa # Balanced
./arksigning -z 9 -k cert.p12 -p "pass" -m profile.mobileprovision -o small.ipa app.ipa    # Maximum

# ❌ Invalid compression levels
./arksigning -z 10 -k cert.p12 -p "pass" -m profile.mobileprovision -o output.ipa app.ipa  # > 9
./arksigning -z -1 -k cert.p12 -p "pass" -m profile.mobileprovision -o output.ipa app.ipa  # < 0
```

#### **Thread Count Validation**
```bash
# ✅ Valid parallel options
./arksigning --bulk --inputfolder ./apps --outputfolder ./signed \
    -k cert.p12 -p "pass" -m profile.mobileprovision --parallel        # Auto-detect

./arksigning --bulk --inputfolder ./apps --outputfolder ./signed \
    -k cert.p12 -p "pass" -m profile.mobileprovision --parallel 4      # Specific count

# ❌ Invalid thread counts
./arksigning --bulk --inputfolder ./apps --outputfolder ./signed \
    -k cert.p12 -p "pass" -m profile.mobileprovision --parallel 0      # Must be > 0

./arksigning --bulk --inputfolder ./apps --outputfolder ./signed \
    -k cert.p12 -p "pass" -m profile.mobileprovision --parallel 1000   # Too high
```

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
