#!/bin/bash
RED='\033[0;31m'
GREEN='\033[0;32m'
NORMAL_OUTPUT='\033[0m'

# Change to parent directory
cd "$(dirname "$0")/.."

# Compile the project under "Debug"
./scripts/build.sh

./build/Debug/tests
./build/Debug/third_party_tests
