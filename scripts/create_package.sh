#!/bin/bash
BUILD_TYPE="Release"
RED='\033[0;31m'
GREEN='\033[0;32m'
NORMAL_OUTPUT='\033[0m'

# Work at the root directory
# Should have conanfile.py present there.
cd "$(dirname "$0")"
./compile_script
cd ..
conan export-pkg .
conan create .
