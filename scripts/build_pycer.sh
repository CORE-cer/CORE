#!/bin/bash

# Rebuild pycer Python bindings
# Forces a fresh build from C++ source

cd "$(dirname "$0")"
cd ..

uv sync --reinstall-package pycer
