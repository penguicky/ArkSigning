#!/bin/bash

# ArkSigning Installation Script
# Automatically detects OS and installs dependencies

set -e  # Exit on any error

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Configuration
CMAKE_MIN_VERSION="3.21.3"
PROJECT_NAME="ArkSigning"

# Logging functions
log_info() {
    echo -e "${BLUE}[INFO]${NC} $1"
}

log_success() {
    echo -e "${GREEN}[SUCCESS]${NC} $1"
}

log_warning() {
    echo -e "${YELLOW}[WARNING]${NC} $1"
}

log_error() {
    echo -e "${RED}[ERROR]${NC} $1"
}

# Check if command exists
command_exists() {
    command -v "$1" >/dev/null 2>&1
}

# Detect OS
detect_os() {
    if [[ "$OSTYPE" =~ ^darwin ]]; then
        echo "macos"
    elif command_exists yum; then
        echo "centos"
    elif command_exists apt-get; then
        echo "ubuntu"
    else
        echo "unknown"
    fi
}

# Install dependencies for macOS
install_macos_deps() {
    log_info "Installing dependencies for macOS..."

    # Check if Homebrew is installed
    if ! command_exists brew; then
        log_error "Homebrew is not installed. Please install it first:"
        log_info "Visit: https://brew.sh/"
        exit 1
    fi

    # Install dependencies
    log_info "Installing OpenSSL, CMake, and utilities..."
    brew install openssl@3 cmake zip unzip || {
        log_warning "Some packages might already be installed, continuing..."
    }

    # Verify OpenSSL installation
    if brew list openssl@3 >/dev/null 2>&1; then
        OPENSSL_PATH=$(brew --prefix openssl@3)
        log_success "OpenSSL 3.x installed at: $OPENSSL_PATH"
    elif brew list openssl >/dev/null 2>&1; then
        OPENSSL_PATH=$(brew --prefix openssl)
        log_success "OpenSSL installed at: $OPENSSL_PATH"
    else
        log_error "OpenSSL installation failed"
        exit 1
    fi
}

# Install dependencies for Ubuntu/Debian
install_ubuntu_deps() {
    log_info "Installing dependencies for Ubuntu/Debian..."

    # Update package list
    sudo apt-get update

    # Install dependencies
    sudo apt-get install -y \
        build-essential \
        cmake \
        libssl-dev \
        zlib1g-dev \
        zip \
        unzip \
        git \
        pkg-config || {
        log_error "Failed to install dependencies"
        exit 1
    }

    log_success "Dependencies installed successfully"
}

# Install dependencies for CentOS/RHEL
install_centos_deps() {
    log_info "Installing dependencies for CentOS/RHEL..."

    # Install development tools and dependencies
    sudo yum groupinstall -y "Development Tools" || {
        log_error "Failed to install development tools"
        exit 1
    }

    sudo yum install -y \
        openssl-devel \
        zlib-devel \
        cmake \
        zip \
        unzip \
        git \
        wget || {
        log_error "Failed to install dependencies"
        exit 1
    }

    log_success "Dependencies installed successfully"
}

# Build the project
build_project() {
    log_info "Building $PROJECT_NAME..."

    # Create build directory
    if [ -d "build" ]; then
        log_info "Removing existing build directory..."
        rm -rf build
    fi

    mkdir build
    cd build

    # Configure with CMake
    log_info "Configuring with CMake..."
    cmake .. || {
        log_error "CMake configuration failed"
        exit 1
    }

    # Build
    log_info "Compiling..."
    make -j$(nproc 2>/dev/null || sysctl -n hw.ncpu 2>/dev/null || echo 4) || {
        log_error "Compilation failed"
        exit 1
    }

    # Verify binary was created
    if [ -f "arksigning" ]; then
        log_success "Build completed successfully!"
        log_info "Binary location: $(pwd)/arksigning"

        # Make it executable
        chmod +x arksigning

        # Show version if possible
        if ./arksigning --version >/dev/null 2>&1; then
            log_info "Version: $(./arksigning --version)"
        fi
    else
        log_error "Build failed - binary not found"
        exit 1
    fi
}

# Main installation function
main() {
    log_info "Starting $PROJECT_NAME installation..."
    log_info "Detected OS: $(detect_os)"

    # Detect and install dependencies based on OS
    case $(detect_os) in
        "macos")
            install_macos_deps
            ;;
        "ubuntu")
            install_ubuntu_deps
            ;;
        "centos")
            install_centos_deps
            ;;
        "unknown")
            log_error "Unsupported operating system"
            log_info "Please install dependencies manually:"
            log_info "- C++ compiler (g++ or clang++)"
            log_info "- CMake (>= 3.14)"
            log_info "- OpenSSL development libraries"
            log_info "- zlib development libraries"
            exit 1
            ;;
    esac

    # Build the project
    build_project

    log_success "Installation completed successfully!"
    log_info "You can now use: ./build/arksigning --help"

    # Optional: offer to install system-wide
    echo ""
    read -p "Would you like to install arksigning system-wide? (y/N): " -n 1 -r
    echo
    if [[ $REPLY =~ ^[Yy]$ ]]; then
        log_info "Installing system-wide..."
        sudo make install || {
            log_warning "System-wide installation failed, but you can still use ./build/arksigning"
        }
        log_success "System-wide installation completed!"
        log_info "You can now use: arksigning --help"
    fi
}

# Run main function
main "$@"
