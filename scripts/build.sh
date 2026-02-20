#!/bin/bash

# Work at the root directory
cd "$(dirname "$0")"
cd ..

# Include common
source scripts/common.sh
_setArgs "$@"

# Call build function from common
build
