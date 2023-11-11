#!/bin/bash

# Work at the root directory
# Should have conanfile.py present there.
cd "$(dirname "$0")"
cd ..

# Include common
source scripts/common.sh
_setArgs "$@"

# Call build function from common
build
