#!/bin/bash

# Work at the root directory
# Should have conanfile.py present there.
cd "$(dirname "$0")"

./install_dependencies.sh --buildType Release
./build.sh --buildType Release

cd ..

conan export-pkg .
conan create --profile:host ${CONAN_PROFILE} --profile:build ${CONAN_PROFILE}\
              --build missing .
