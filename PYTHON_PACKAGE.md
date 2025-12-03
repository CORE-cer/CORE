# Python Package Installation Guide

## Quick Start

### Install in Development Mode
```bash
pip install -e .
```

This installs the package in "editable" mode - changes to the C++ code will be reflected after rebuilding.

### Build a Wheel for Distribution
```bash
pip install build
python -m build
```

This creates a wheel file in `dist/` directory.

### Install from Wheel
```bash
pip install dist/core_cer-0.0.1-*.whl
```

### Upload to PyPI
```bash
pip install twine
twine upload dist/*
```

## Testing the Installation

After installing with pip, you can import the module from anywhere:

```python
import _pycore

# Test basic functionality
_pycore.hello()

# Create objects
client = _pycore.PyClient('localhost', 5555)
int_val = _pycore.PyIntValue(42)
```

## Type Checking

The package includes type stubs (`_pycore.pyi` and `py.typed` marker), so type checkers work automatically:

```bash
mypy your_script.py
```

## Notes

- **First Build**: The first `pip install -e .` will take time as it builds all vcpkg dependencies
- **Rebuilding**: After C++ changes, run `pip install -e . --force-reinstall --no-deps`
- **Dependencies**: vcpkg dependencies are statically linked into the wheel
- **Update Version**: Edit version in `pyproject.toml` before building for distribution

## Customization

Edit `pyproject.toml` to:
- Change package name (currently `core-cer`)
- Update author information
- Modify build options (sanitizers, logging level, etc.)
- Add Python dependencies if needed
