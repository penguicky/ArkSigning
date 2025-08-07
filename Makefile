# ArkSigning Makefile
# Modern cross-platform build system with automatic dependency detection

# Project configuration
PROJECT_NAME = arksigning
CXX = g++
CXXFLAGS = -std=c++11 -O3 -Wall -Wextra
LDFLAGS = 
LIBS = -lcrypto -lz

# Source files
# Source directories (organized by component)
CORE_SRCDIR = src/core
CRYPTO_SRCDIR = src/crypto
UTILS_SRCDIR = src/utils

# Source files
CORE_SOURCES = $(wildcard $(CORE_SRCDIR)/*.cpp)
CRYPTO_SOURCES = $(wildcard $(CRYPTO_SRCDIR)/*.cpp)
UTILS_SOURCES = $(wildcard $(UTILS_SRCDIR)/*.cpp)
SOURCES = $(CORE_SOURCES) $(CRYPTO_SOURCES) $(UTILS_SOURCES)

# Object files
OBJECTS = $(SOURCES:.cpp=.o)
TARGET = $(PROJECT_NAME)

# Include directories
INCLUDE_DIRS = -Iinclude -Iinclude/arksigning

# Platform detection
UNAME_S := $(shell uname -s)
UNAME_M := $(shell uname -m)

# Default OpenSSL paths
OPENSSL_INCLUDE = 
OPENSSL_LIB = 

# macOS configuration
ifeq ($(UNAME_S),Darwin)
    # Try to detect OpenSSL via Homebrew
    BREW_PREFIX := $(shell brew --prefix 2>/dev/null)
    ifneq ($(BREW_PREFIX),)
        # Check for OpenSSL 3.x first (preferred)
        OPENSSL3_PREFIX := $(shell brew --prefix openssl@3 2>/dev/null)
        ifneq ($(OPENSSL3_PREFIX),)
            OPENSSL_INCLUDE = -I$(OPENSSL3_PREFIX)/include
            OPENSSL_LIB = -L$(OPENSSL3_PREFIX)/lib
            $(info Found OpenSSL 3.x at: $(OPENSSL3_PREFIX))
        else
            # Fallback to default OpenSSL
            OPENSSL_PREFIX := $(shell brew --prefix openssl 2>/dev/null)
            ifneq ($(OPENSSL_PREFIX),)
                OPENSSL_INCLUDE = -I$(OPENSSL_PREFIX)/include
                OPENSSL_LIB = -L$(OPENSSL_PREFIX)/lib
                $(info Found OpenSSL at: $(OPENSSL_PREFIX))
            endif
        endif
    endif
    
    # If Homebrew detection failed, try common paths
    ifeq ($(OPENSSL_INCLUDE),)
        ifneq ($(wildcard /opt/homebrew/opt/openssl@3/include),)
            OPENSSL_INCLUDE = -I/opt/homebrew/opt/openssl@3/include
            OPENSSL_LIB = -L/opt/homebrew/opt/openssl@3/lib
            $(info Using Apple Silicon Homebrew OpenSSL 3.x path)
        else ifneq ($(wildcard /usr/local/opt/openssl@3/include),)
            OPENSSL_INCLUDE = -I/usr/local/opt/openssl@3/include
            OPENSSL_LIB = -L/usr/local/opt/openssl@3/lib
            $(info Using Intel Homebrew OpenSSL 3.x path)
        else ifneq ($(wildcard /opt/homebrew/opt/openssl/include),)
            OPENSSL_INCLUDE = -I/opt/homebrew/opt/openssl/include
            OPENSSL_LIB = -L/opt/homebrew/opt/openssl/lib
            $(info Using Apple Silicon Homebrew OpenSSL path)
        else ifneq ($(wildcard /usr/local/opt/openssl/include),)
            OPENSSL_INCLUDE = -I/usr/local/opt/openssl/include
            OPENSSL_LIB = -L/usr/local/opt/openssl/lib
            $(info Using Intel Homebrew OpenSSL path)
        endif
    endif
endif

# Linux configuration
ifeq ($(UNAME_S),Linux)
    # Try pkg-config first
    PKG_CONFIG := $(shell which pkg-config 2>/dev/null)
    ifneq ($(PKG_CONFIG),)
        OPENSSL_CFLAGS := $(shell pkg-config --cflags openssl 2>/dev/null)
        OPENSSL_LDFLAGS := $(shell pkg-config --libs openssl 2>/dev/null)
        ifneq ($(OPENSSL_CFLAGS),)
            OPENSSL_INCLUDE = $(OPENSSL_CFLAGS)
            LIBS = $(OPENSSL_LDFLAGS) -lz
            $(info Using pkg-config for OpenSSL)
        endif
    endif
    
    # Fallback to system paths
    ifeq ($(OPENSSL_INCLUDE),)
        ifneq ($(wildcard /usr/include/openssl),)
            $(info Using system OpenSSL)
        else
            $(warning OpenSSL headers not found. Please install libssl-dev or openssl-devel)
        endif
    endif
endif

# Windows/MinGW configuration
ifneq (,$(findstring MINGW,$(UNAME_S)))
    LIBS += -lws2_32 -lgdi32
    CXXFLAGS += -DWINDOWS
    TARGET = $(PROJECT_NAME).exe
endif

# Combine flags
ALL_CXXFLAGS = $(CXXFLAGS) $(INCLUDE_DIRS) $(OPENSSL_INCLUDE)
ALL_LDFLAGS = $(LDFLAGS) $(OPENSSL_LIB)

# Build rules
.PHONY: all clean install uninstall help check-deps

all: check-deps $(TARGET)

$(TARGET): $(OBJECTS)
	@echo "Linking $(TARGET)..."
	$(CXX) $(OBJECTS) -o $(TARGET) $(ALL_LDFLAGS) $(LIBS)
	@echo "Build complete: $(TARGET)"

%.o: %.cpp
	@echo "Compiling $<..."
	$(CXX) $(ALL_CXXFLAGS) -c $< -o $@

clean:
	@echo "Cleaning build artifacts..."
	rm -f $(OBJECTS) $(TARGET)
	rm -rf build/
	@echo "Clean complete"

install: $(TARGET)
	@echo "Installing $(TARGET)..."
	install -d $(DESTDIR)/usr/local/bin
	install -m 755 $(TARGET) $(DESTDIR)/usr/local/bin/
	@echo "Installation complete"

uninstall:
	@echo "Uninstalling $(TARGET)..."
	rm -f $(DESTDIR)/usr/local/bin/$(TARGET)
	@echo "Uninstall complete"

check-deps:
	@echo "Checking dependencies..."
	@echo "Platform: $(UNAME_S) $(UNAME_M)"
	@echo "Compiler: $(CXX)"
	@echo "OpenSSL include: $(OPENSSL_INCLUDE)"
	@echo "OpenSSL lib: $(OPENSSL_LIB)"
	@echo "All CXXFLAGS: $(ALL_CXXFLAGS)"
	@echo "All LDFLAGS: $(ALL_LDFLAGS)"
	@echo "Libraries: $(LIBS)"
	@echo ""

help:
	@echo "ArkSigning Build System"
	@echo ""
	@echo "Available targets:"
	@echo "  all        - Build the project (default)"
	@echo "  clean      - Remove build artifacts"
	@echo "  install    - Install to /usr/local/bin"
	@echo "  uninstall  - Remove from /usr/local/bin"
	@echo "  check-deps - Show detected dependencies and build configuration"
	@echo "  help       - Show this help message"
	@echo ""
	@echo "Environment variables:"
	@echo "  CXX        - C++ compiler (default: g++)"
	@echo "  CXXFLAGS   - Additional compiler flags"
	@echo "  LDFLAGS    - Additional linker flags"
	@echo "  DESTDIR    - Installation prefix (default: /usr/local)"
	@echo ""
	@echo "Examples:"
	@echo "  make                    # Build with auto-detected settings"
	@echo "  make CXX=clang++        # Use clang++ compiler"
	@echo "  make CXXFLAGS=-g        # Add debug symbols"
	@echo "  make install DESTDIR=~  # Install to home directory"

# Dependency tracking
-include $(OBJECTS:.o=.d)

%.d: %.cpp
	@$(CXX) $(ALL_CXXFLAGS) -MM -MT $(@:.d=.o) $< > $@
