#!/bin/bash
RED='\033[0;31m'
GREEN='\033[0;32m'
NORMAL_OUTPUT='\033[0m'

# Work at the root directory
cd "$(dirname "$0")"
cd ..

# Find all .cpp files in src/ and its subdirectories and format them using clang-format
for file in $(find src -name "*.cpp")
do
  # Copy original file for later comparison
  cp "$file" "$file.bak"

  # Run clang-format on the file
  clang-format -i -style=file "$file"

  # Check if the file has changed
  if ! cmp -s "$file" "$file.bak"; then
    echo -e "${GREEN}Formatted $file${NORMAL_OUTPUT}"
  fi

  # Remove the backup file
  rm "$file.bak"
done

echo -e "${GREEN}Done formatting.${NORMAL_OUTPUT}"
