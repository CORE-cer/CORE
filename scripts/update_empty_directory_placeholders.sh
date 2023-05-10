#!/bin/bash

# This script traverses the 'src' directory, adds a '.keep' file to
# empty directories, and removes '.keep' from non-empty ones.
# .keep are used to track empty directories in github.

cd "$(dirname "$0")"
cd ../src

# Function to add/remove .keep files as necessary
function update_directories() {
    for d in "$1"/*; do
        if [ -d "$d" ]; then
            files=$(ls "$d")
            if [ -z "$files" ]; then
                touch "$d/.keep"
            else
                if [ -f "$d/.keep" ]; then
                    git rm "$d/.keep"
                fi
            fi
            update_directories "$d"
        fi
    done
}

# Call the function
update_directories .

