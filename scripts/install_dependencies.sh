#!/bin/bash

# Work at the root directory
# Should have conanfile.py present there.
cd "$(dirname "$0")"
cd ..

# Include common
source scripts/common.sh
_setArgs "$@"

conan source .
conan install . -s build_type=${BUILD_TYPE} -s:h compiler=gcc \
                -s:h compiler.cppstd=gnu20 -s:h compiler.version=12.2\
                --build missing

