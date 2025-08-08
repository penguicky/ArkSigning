# ArkSigning v0.6 - Pre-built Binaries

**Professional iOS app signing tool with modern C++ excellence** 🚀

## 📦 Available Downloads

### macOS (Apple Silicon - M1/M2/M3)
- **File**: `arksigning-v0.6-macos-arm64`
- **Architecture**: ARM64 (Apple Silicon)
- **Size**: ~317KB
- **Requirements**: macOS 11.0+ (Big Sur or later)

## 🚀 Quick Start

### 1. Download and Install
```bash
# Download the binary (replace with actual download URL)
curl -L -o arksigning https://github.com/penguicky/ArkSigning/releases/download/v0.6/arksigning-v0.6-macos-arm64

# Make it executable
chmod +x arksigning

# Move to PATH (optional)
sudo mv arksigning /usr/local/bin/
```

### 2. Verify Installation
```bash
arksigning -v
# Should output: version: 0.6
```

### 3. Basic Usage
```bash
# Sign an IPA file
./arksigning -k certificate.p12 -p password -m profile.mobileprovision -o signed.ipa app.ipa

# Get app information
./arksigning --info app.ipa

# Show help
./arksigning -h
```

## ✨ Key Features

- **iOS App Signing** - Sign .ipa files and app bundles
- **Bulk Processing** - Sign multiple apps in parallel
- **Dylib Injection** - Inject dynamic libraries into apps
- **Modern C++11** - Enterprise-grade code quality
- **Zero Dependencies** - Self-contained binary (OpenSSL statically linked)

## 📖 Full Documentation

For complete usage examples and documentation, visit:
- **GitHub Repository**: https://github.com/penguicky/ArkSigning
- **Complete README**: https://github.com/penguicky/ArkSigning/blob/main/README.md

## 🔧 Build Information

- **Version**: 0.6
- **Build Date**: August 2025
- **Compiler**: Apple Clang 15.0.0
- **OpenSSL**: 3.5.1 (Homebrew)
- **Architecture**: ARM64 (Apple Silicon)
- **Optimization**: -O3 (Maximum optimization)

## 🛡️ Security Notes

- This binary is built from the official ArkSigning source code
- OpenSSL cryptographic operations for secure signing
- No telemetry or data collection
- Open source - audit the code yourself!

## 🐛 Issues & Support

If you encounter any issues:
1. Check the [GitHub Issues](https://github.com/penguicky/ArkSigning/issues)
2. Review the [documentation](https://github.com/penguicky/ArkSigning/blob/main/README.md)
3. Create a new issue with details about your problem

## 📄 License

MIT License - see [LICENSE](https://github.com/penguicky/ArkSigning/blob/main/docs/LICENSE) for details.

---

**ArkSigning** - *Professional iOS app signing made simple* ⚡
