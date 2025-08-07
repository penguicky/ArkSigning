#!/bin/bash

# Script to update include paths after reorganization
# This script updates all source files to use the new organized header paths

echo "Updating include paths in source files..."

# Function to update includes in a file
update_includes() {
    local file="$1"
    echo "Updating: $file"
    
    # Update common includes
    sed -i.bak 's|#include "common/common.h"|#include "utils/common.h"|g' "$file"
    sed -i.bak 's|#include "common/base64.h"|#include "utils/base64.h"|g' "$file"
    sed -i.bak 's|#include "common/json.h"|#include "utils/json.h"|g' "$file"
    sed -i.bak 's|#include "common/constants.h"|#include "utils/constants.h"|g' "$file"
    sed -i.bak 's|#include "common/mach-o.h"|#include "utils/mach-o.h"|g' "$file"
    
    # Update modern includes
    sed -i.bak 's|#include "common/optional.h"|#include "modern/optional.h"|g' "$file"
    sed -i.bak 's|#include "common/types.h"|#include "modern/types.h"|g' "$file"
    sed -i.bak 's|#include "common/utility.h"|#include "modern/utility.h"|g' "$file"
    sed -i.bak 's|#include "common/callbacks.h"|#include "modern/callbacks.h"|g' "$file"
    
    # Update crypto includes
    sed -i.bak 's|#include "openssl.h"|#include "crypto/openssl.h"|g' "$file"
    sed -i.bak 's|#include "common/openssl_raii.h"|#include "crypto/openssl_raii.h"|g' "$file"
    
    # Update core includes
    sed -i.bak 's|#include "bundle.h"|#include "core/bundle.h"|g' "$file"
    sed -i.bak 's|#include "macho.h"|#include "core/macho.h"|g' "$file"
    sed -i.bak 's|#include "archo.h"|#include "core/archo.h"|g' "$file"
    sed -i.bak 's|#include "signing.h"|#include "core/signing.h"|g' "$file"
    
    # Remove backup file
    rm -f "$file.bak"
}

# Update all source files
find src -name "*.cpp" -type f | while read -r file; do
    update_includes "$file"
done

# Update all header files
find include -name "*.h" -type f | while read -r file; do
    update_includes "$file"
done

echo "Include path updates completed!"
echo ""
echo "Summary of changes:"
echo "- common/* -> utils/* (for utility headers)"
echo "- common/* -> modern/* (for modern C++ headers)"
echo "- *.h -> core/*.h (for core component headers)"
echo "- openssl.h -> crypto/openssl.h"
echo ""
echo "Please review the changes and test the build."
