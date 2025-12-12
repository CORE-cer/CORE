#!/bin/bash
set -e  # Exit on error

# Work at the root directory
cd "$(dirname "$0")"
cd ..

# Include common
source scripts/common.sh
_setArgs "$@"


# Install vcpkg if not present or not bootstrapped
if [ ! -f "vcpkg/vcpkg" ]; then
    if [ ! -d "vcpkg" ]; then
        echo "Cloning vcpkg..."
        git clone https://github.com/microsoft/vcpkg.git
    fi
    echo "Bootstrapping vcpkg..."
    ./vcpkg/bootstrap-vcpkg.sh

    # Verify bootstrap succeeded
    if [ ! -f "vcpkg/vcpkg" ]; then
        echo "ERROR: vcpkg bootstrap failed!"
        exit 1
    fi
fi

echo "vcpkg is ready at: $(pwd)/vcpkg/vcpkg"

# Generate ANTLR scripts
python3 scripts/generate_antlr.py

