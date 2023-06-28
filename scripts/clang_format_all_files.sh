#!/bin/bash
RED='\033[0;31m'
GREEN='\033[0;32m'
NORMAL_OUTPUT='\033[0m'

# Work at the root directory
cd "$(dirname "$0")"
cd ..

# Flag indicating whether any files were changed
changed_files=false

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
    changed_files=true
  fi

  # Remove the backup file
  rm "$file.bak"
done

if [ "$changed_files" = true ]; then
  echo -e "${GREEN}Done formatting. Some files were changed.${NORMAL_OUTPUT}"
  exit 1
else
  echo -e "${GREEN}Done formatting. No files were changed.${NORMAL_OUTPUT}"
  exit 0
fi
