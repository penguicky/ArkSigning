#!/bin/bash

# ArkSigning v0.6 - Installation Script
# Professional iOS app signing tool installer

set -e

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Configuration
VERSION="0.6"
REPO="penguicky/ArkSigning"
BINARY_NAME="arksigning"

echo -e "${BLUE}🚀 ArkSigning v${VERSION} Installer${NC}"
echo -e "${BLUE}Professional iOS App Signing Tool${NC}"
echo ""

# Detect architecture
ARCH=$(uname -m)
OS=$(uname -s)

echo -e "${YELLOW}📋 System Information:${NC}"
echo "  OS: $OS"
echo "  Architecture: $ARCH"
echo ""

# Determine download URL based on system
if [[ "$OS" == "Darwin" && "$ARCH" == "arm64" ]]; then
    DOWNLOAD_URL="https://github.com/${REPO}/releases/download/v${VERSION}/arksigning-v${VERSION}-macos-arm64.tar.gz"
    BINARY_FILE="arksigning-v${VERSION}-macos-arm64"
    echo -e "${GREEN}✅ Compatible system detected: macOS Apple Silicon${NC}"
elif [[ "$OS" == "Darwin" && "$ARCH" == "x86_64" ]]; then
    echo -e "${RED}❌ Intel Mac detected${NC}"
    echo -e "${YELLOW}⚠️  Currently only Apple Silicon (M1/M2/M3) binaries are available.${NC}"
    echo -e "${YELLOW}   Please build from source or request Intel binaries.${NC}"
    exit 1
else
    echo -e "${RED}❌ Unsupported system: $OS $ARCH${NC}"
    echo -e "${YELLOW}   Currently supported:${NC}"
    echo -e "${YELLOW}   - macOS Apple Silicon (arm64)${NC}"
    echo -e "${YELLOW}   Please build from source for your platform.${NC}"
    exit 1
fi

# Create temporary directory
TEMP_DIR=$(mktemp -d)
cd "$TEMP_DIR"

echo -e "${BLUE}📥 Downloading ArkSigning v${VERSION}...${NC}"
if command -v curl >/dev/null 2>&1; then
    curl -L -o "arksigning.tar.gz" "$DOWNLOAD_URL"
elif command -v wget >/dev/null 2>&1; then
    wget -O "arksigning.tar.gz" "$DOWNLOAD_URL"
else
    echo -e "${RED}❌ Neither curl nor wget found. Please install one of them.${NC}"
    exit 1
fi

echo -e "${BLUE}📦 Extracting archive...${NC}"
tar -xzf "arksigning.tar.gz"

echo -e "${BLUE}🔧 Installing ArkSigning...${NC}"

# Make binary executable
chmod +x "$BINARY_FILE"

# Determine installation directory
if [[ -w "/usr/local/bin" ]]; then
    INSTALL_DIR="/usr/local/bin"
    cp "$BINARY_FILE" "$INSTALL_DIR/$BINARY_NAME"
    echo -e "${GREEN}✅ Installed to: $INSTALL_DIR/$BINARY_NAME${NC}"
elif [[ -w "$HOME/.local/bin" ]]; then
    INSTALL_DIR="$HOME/.local/bin"
    mkdir -p "$INSTALL_DIR"
    cp "$BINARY_FILE" "$INSTALL_DIR/$BINARY_NAME"
    echo -e "${GREEN}✅ Installed to: $INSTALL_DIR/$BINARY_NAME${NC}"
    echo -e "${YELLOW}⚠️  Make sure $INSTALL_DIR is in your PATH${NC}"
else
    INSTALL_DIR="$HOME"
    cp "$BINARY_FILE" "$INSTALL_DIR/$BINARY_NAME"
    echo -e "${GREEN}✅ Installed to: $INSTALL_DIR/$BINARY_NAME${NC}"
    echo -e "${YELLOW}⚠️  Binary installed to home directory. Consider moving to PATH.${NC}"
fi

# Cleanup
cd - >/dev/null
rm -rf "$TEMP_DIR"

echo ""
echo -e "${GREEN}🎉 Installation completed successfully!${NC}"
echo ""
echo -e "${BLUE}📖 Quick Start:${NC}"
echo "  # Check version"
echo "  $BINARY_NAME -v"
echo ""
echo "  # Sign an IPA file"
echo "  $BINARY_NAME -k cert.p12 -p password -m profile.mobileprovision -o signed.ipa app.ipa"
echo ""
echo "  # Get help"
echo "  $BINARY_NAME -h"
echo ""
echo -e "${BLUE}📚 Documentation:${NC}"
echo "  https://github.com/${REPO}/blob/main/README.md"
echo ""
echo -e "${BLUE}🐛 Issues & Support:${NC}"
echo "  https://github.com/${REPO}/issues"
echo ""

# Test installation
if command -v "$BINARY_NAME" >/dev/null 2>&1; then
    echo -e "${GREEN}✅ Installation verified - $BINARY_NAME is in PATH${NC}"
    echo -e "${BLUE}Version: $($BINARY_NAME -v)${NC}"
else
    echo -e "${YELLOW}⚠️  $BINARY_NAME not found in PATH. You may need to:${NC}"
    echo "  export PATH=\"$INSTALL_DIR:\$PATH\""
fi

echo ""
echo -e "${GREEN}Happy signing! 🚀${NC}"
