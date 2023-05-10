#!/bin/bash
RED='\033[0;31m'
GREEN='\033[0;32m'
NORMAL_OUTPUT='\033[0m'

# Work at the root directory
# Should have conanfile.py present there.
cd "$(dirname "$0")"
./compile_script.sh

ln -s -r ../build/Debug/compile_commands.json ../compile_commands.json
ctags -R ../src ../tests
mv tags ../
