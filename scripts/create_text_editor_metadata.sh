#!/bin/bash
RED='\033[0;31m'
GREEN='\033[0;32m'
NORMAL_OUTPUT='\033[0m'

# Change to parent directory
cd "$(dirname "$0")/.."

# Compile the project under "Debug"
./scripts/install_dependencies.sh Debug
./scripts/build.sh Debug

# Remove existing compile_commands.json and tags if they exist
[ -L "compile_commands.json" ] && rm "compile_commands.json"
[ -f "tags" ] && rm "tags"

# Create a symbolic link to compile_commands.json
ln -s -r build/Debug/compile_commands.json compile_commands.json

# Generate ctags
ctags -R src
