#!/bin/bash
BUILD_TYPE="Release"
RED='\033[0;31m'
GREEN='\033[0;32m'
NORMAL_OUTPUT='\033[0m'

# Work at the root directory
# Should have conanfile.py present there.
cd "$(dirname "$0")"
./install_dependencies.sh Release
./build.sh Release
cd ..
conan export-pkg .
conan create build_type=${BUILD_TYPE} -s:b compiler=gcc -s:b compiler.cppstd=gnu20 -s:b compiler.version=12.2 --build missing .
