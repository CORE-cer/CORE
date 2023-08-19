#!/bin/bash

# Work at the root directory
# Should have conanfile.py present there.
cd "$(dirname "$0")"
cd ..

# Include common
source scripts/common.sh
_setArgs "$@"

conan build . --profile:host ${CONAN_PROFILE} --profile:build ${CONAN_PROFILE}\
              -s build_type=${BUILD_TYPE}\
              --build missing
