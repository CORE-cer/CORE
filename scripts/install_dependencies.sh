#!/bin/bash

# Work at the root directory
cd "$(dirname "$0")"
cd ..

# Clone vcpkg if not present
if [ ! -d "vcpkg" ]; then
    echo "Cloning vcpkg..."
    git clone https://github.com/microsoft/vcpkg.git
fi

# Bootstrap vcpkg if binary not present
if [ ! -f "vcpkg/vcpkg" ]; then
    echo "Bootstrapping vcpkg..."
    ./vcpkg/bootstrap-vcpkg.sh
fi

# Generate ANTLR autogeneration scripts
echo "Generating ANTLR scripts..."
python3 scripts/generate_antlr.py
