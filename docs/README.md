
# ArkSigning

A more improved iOS app signing tool based on zsign, with enhanced features for bulk signing, parallel processing, and modern build system support.

## 🚀 Quick Start

### Prerequisites

**macOS:**
```bash
# Install dependencies via Homebrew
brew install openssl cmake
```

**Ubuntu/Debian:**
```bash
sudo apt-get update
sudo apt-get install build-essential libssl-dev zlib1g-dev cmake git
```

**CentOS/RHEL:**
```bash
sudo yum groupinstall "Development Tools"
sudo yum install openssl-devel zlib-devel cmake git
```

### Build Methods

#### Method 1: Using Makefile (Recommended)

The modern Makefile automatically detects your system configuration and OpenSSL installation:

```bash
# Clone the repository
git clone https://github.com/penguicky/ArkSigning.git
cd ArkSigning

# Build (automatically detects OpenSSL paths)
make

# Check detected configuration
make check-deps

# Install system-wide (optional)
sudo make install
```

#### Method 2: Using CMake

```bash
mkdir build && cd build
cmake ..
make
```

#### Method 3: Manual Compilation

**macOS (with automatic OpenSSL detection):**
```bash
# The Makefile handles this automatically, but for manual builds:
OPENSSL_PATH=$(brew --prefix openssl@3)
g++ *.cpp common/*.cpp -lcrypto -lz \
    -I$OPENSSL_PATH/include -L$OPENSSL_PATH/lib \
    -std=c++11 -O3 -o arksigning
```

**Linux:**
```bash
# Using pkg-config (recommended)
g++ *.cpp common/*.cpp $(pkg-config --cflags --libs openssl) -lz -std=c++11 -O3 -o arksigning

# Or manually
g++ *.cpp common/*.cpp -lcrypto -lz -std=c++11 -O3 -o arksigning
```

### Automated Installation Script

For quick setup on Linux systems:

```bash
git clone https://github.com/penguicky/ArkSigning.git
cd ArkSigning
chmod +x INSTALL.sh
./INSTALL.sh
```

## 🔧 Advanced Build Options

### Cross-compilation for Windows (from Linux)

```bash
# Install MinGW
sudo apt-get install mingw-w64

# Build for Windows
x86_64-w64-mingw32-g++ *.cpp common/*.cpp -o arksigning.exe \
    -lcrypto -lz -lws2_32 -lgdi32 -std=c++11 -O3 -DWINDOWS \
    -static -static-libgcc
```

### Using xmake

If you have [xmake](https://xmake.io) installed:

```bash
# Build
xmake

# Run
xmake run arksigning [options]

# Install
xmake install
```

### Using Docker

Build and run ArkSigning in a containerized environment:

```bash
# Build the Docker image
docker build -t arksigning https://github.com/penguicky/ArkSigning.git

# Run with current directory mounted
docker run -v "$PWD:$PWD" -w "$PWD" arksigning \
    -k privkey.pem -m dev.prov -o output.ipa -z 9 demo.ipa

# Extract the static binary for deployment
docker run -v $PWD:/out --rm --entrypoint /bin/cp arksigning arksigning /out
```

## 🛠️ Troubleshooting

### Common Build Issues

**OpenSSL not found on macOS:**
```bash
# Install OpenSSL via Homebrew
brew install openssl@3

# Check installation
brew --prefix openssl@3

# If still having issues, try:
export OPENSSL_ROOT_DIR=$(brew --prefix openssl@3)
make clean && make
```

**OpenSSL not found on Linux:**
```bash
# Ubuntu/Debian
sudo apt-get install libssl-dev

# CentOS/RHEL
sudo yum install openssl-devel

# Check if pkg-config can find OpenSSL
pkg-config --cflags --libs openssl
```

**Permission denied during installation:**
```bash
# Use sudo for system-wide installation
sudo make install

# Or install to user directory
make install DESTDIR=$HOME/.local
```


## 📱 Usage

ArkSigning is a powerful iOS app signing tool. Make sure you have **unzip** and **zip** commands installed.

### Command Line Options

```bash
Usage: arksigning [-options] [-k privkey.pem] [-m dev.prov] [-o output.ipa] file|folder
```

#### Core Options
| Option | Description |
|--------|-------------|
| `-k, --pkey` | Path to private key or p12 file (PEM or DER format) |
| `-m, --prov` | Path to mobile provisioning profile |
| `-c, --cert` | Path to certificate file (PEM or DER format) |
| `-o, --output` | Path to output ipa file |
| `-p, --password` | Password for private key or p12 file |

#### App Modification Options
| Option | Description |
|--------|-------------|
| `-b, --bundle_id` | New bundle ID to change |
| `-n, --bundle_name` | New bundle name to change |
| `-r, --bundle_version` | New bundle version to change |
| `-e, --entitlements` | New entitlements to change |

#### Advanced Options
| Option | Description |
|--------|-------------|
| `-l, --dylib` | Path to inject dylib file (use multiple times for multiple dylibs) |
| `-w, --weak` | Inject dylib as LC_LOAD_WEAK_DYLIB |
| `-z, --zip_level` | Compression level for output ipa file (0-9) |
| `-f, --force` | Force sign without cache when signing folder |
| `-d, --debug` | Generate debug output files (.arksigning_debug folder) |
| `-E, --no-embed-profile` | Don't generate embedded mobile provision |
| `-i, --install` | Install ipa file using ideviceinstaller command for test |
| `-q, --quiet` | Quiet operation |
| `-I, --info` | Output app information in JSON format, including app icon in base64 |

#### Bulk Signing Options
| Option | Description |
|--------|-------------|
| `-B, --bulk` | Enable bulk signing mode |
| `--inputfolder` | Folder containing unsigned apps to process |
| `--outputfolder` | Destination folder for signed apps |
| `--parallel` | Enable parallel processing with optional thread count |
| `--base-url` | Base URL used to generate OTA link |

#### Utility Options
| Option | Description |
|--------|-------------|
| `-v, --version` | Show version information |
| `-h, --help` | Show help message |

### 📋 Usage Examples

#### Basic Signing Operations

**Show app information and mach-o details:**
```bash
./arksigning demo.app/execute
./arksigning --info app.ipa
```

**Sign IPA with private key and provisioning profile:**
```bash
./arksigning -k privkey.pem -m dev.prov -o output.ipa -z 9 demo.ipa
```

**Sign app folder with P12 certificate:**
```bash
# With cache (faster for repeated signing)
./arksigning -k dev.p12 -p 123 -m dev.prov -o output.ipa demo.app

# Without cache (force fresh signing)
./arksigning -f -k dev.p12 -p 123 -m dev.prov -o output.ipa demo.app
```

#### App Modification

**Change bundle ID and name:**
```bash
./arksigning -k dev.p12 -p 123 -m dev.prov -o output.ipa \
    -b 'com.example.newapp' -n 'New App Name' demo.ipa
```

**Sign without embedded provisioning profile:**
```bash
./arksigning -k dev.p12 -p 123 -m dev.prov -E -o output.ipa demo.ipa
```

#### Dylib Injection

**Inject dylib into IPA and re-sign:**
```bash
./arksigning -k dev.p12 -p 123 -m dev.prov -o output.ipa -l demo.dylib demo.ipa
```

**Inject dylib directly into mach-o binary:**
```bash
# Strong reference (LC_LOAD_DYLIB)
./arksigning -l "@executable_path/demo.dylib" demo.app/execute

# Weak reference (LC_LOAD_WEAK_DYLIB)
./arksigning -w -l "@executable_path/demo.dylib" demo.app/execute
```

#### Bulk Signing

**Sign multiple apps in parallel:**
```bash
# Auto-detect thread count
./arksigning --bulk --inputfolder ./unsigned_apps --outputfolder ./signed_apps \
    -k dev.p12 -p 123 -m dev.prov --parallel

# Specify thread count
./arksigning --bulk --inputfolder ./unsigned_apps --outputfolder ./signed_apps \
    -k dev.p12 -p 123 -m dev.prov --parallel 4
```

**Generate OTA installation links:**
```bash
./arksigning --bulk --inputfolder ./unsigned_apps --outputfolder ./signed_apps \
    -k dev.p12 -p 123 -m dev.prov --base-url "https://example.com/signed/"
```
## ⚡ Performance Tips

### Fast Re-signing with Cache

ArkSigning uses intelligent caching to speed up repeated signing operations:

1. **First-time signing:** Unzip your IPA and sign the app folder instead of the IPA directly
2. **Cache creation:** ArkSigning creates a `.arksigning_cache` directory with signing information
3. **Subsequent signings:** When re-signing the same folder, the cache dramatically speeds up the process

```bash
# Extract IPA for faster re-signing
unzip demo.ipa -d demo_extracted/
./arksigning -k dev.p12 -p 123 -m dev.prov -o output.ipa demo_extracted/Payload/Demo.app/

# Re-signing the same folder is now much faster!
./arksigning -k dev.p12 -p 123 -m dev.prov -o output2.ipa demo_extracted/Payload/Demo.app/
```

### Bulk Signing Performance

For maximum performance when signing multiple apps:
- Use `--parallel` for multi-threaded processing
- Optimal thread count is usually equal to your CPU cores
- Use SSD storage for better I/O performance

## 📝 Changelog

### v0.6.1 (2025-05-01)
- ✨ **NEW:** Base URL support for OTA .plist generation
- 🔗 Automatically generates plist files and logs itms-services:// install links

### v0.6 (2025-04-26)
- 🖼️ Added app icon retrieval in base64 format with `--info` flag
- 📦 Implemented bulk signing feature with `--bulk`
- ⚡ Added parallel processing support with `--parallel` option

### v0.5
- 🚫 Added support for removing embedded.mobileprovision with `-E` flag

### Build System Improvements (Latest)
- 🔧 Added modern Makefile with automatic OpenSSL detection
- 📚 Comprehensive documentation updates
- 🐛 Fixed macOS Apple Silicon and Intel compatibility
- 🔍 Enhanced troubleshooting guides

## 🙏 Credits

- **[SignerVIP](https://signer.vip)** - Enhanced signing service
- **[NabzClan](https://nabzclan.vip)** - Development team and improvements
- **[arinawzad](https://github.com/arinawzad)** - Embedded provisioning profile removal support
- **[zhlynn](https://github.com/zhlynn)** - Original zsign creator

## 📄 License

This project is licensed under the Apache License 2.0 - see the [LICENSE](LICENSE) file for details.

## 🤝 Contributing

Contributions are welcome! Please feel free to submit a Pull Request. For major changes, please open an issue first to discuss what you would like to change.

## ⭐ Support

If you find this project helpful, please consider giving it a star on GitHub!
