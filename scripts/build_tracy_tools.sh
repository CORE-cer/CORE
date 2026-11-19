#!/bin/bash
#
# Builds Tracy's headless command-line tools, used by tests/benchmark/tracy_runs.py
# and tests/benchmark/tracy_to_csv.py:
#   tracy-capture    connects to a running Tracy-enabled program, writes a .tracy file
#   tracy-csvexport  turns a .tracy file into per-zone CSV statistics
#
# They are built from the same Tracy checkout CMake already fetched for the engine
# (build/*/_deps/tracy-src, pinned by GIT_TAG in CMakeLists.txt), so the capture
# tool always speaks the same protocol version as the instrumented client.
# Requires that some engine build directory has been configured at least once.
# The first run needs network access: Tracy's own CMake downloads capstone, zstd
# and PPQSort through CPM.
#
# Usage:
#   scripts/build_tracy_tools.sh
# Result:
#   build/tracy-tools/bin/tracy-capture
#   build/tracy-tools/bin/tracy-csvexport

set -euo pipefail

cd "$(dirname "$0")/.."

TRACY_SRC=""
for candidate in build/*/_deps/tracy-src; do
  if [ -f "${candidate}/capture/CMakeLists.txt" ] && [ -f "${candidate}/csvexport/CMakeLists.txt" ]; then
    TRACY_SRC="${candidate}"
    break
  fi
done
if [ -z "${TRACY_SRC}" ]; then
  echo "No Tracy source found under build/*/_deps/tracy-src." >&2
  echo "Configure any engine build first (e.g. scripts/build.sh), then re-run." >&2
  exit 1
fi
echo "Using Tracy source: ${TRACY_SRC}"

OUT_DIR="build/tracy-tools"
BIN_DIR="${OUT_DIR}/bin"
mkdir -p "${BIN_DIR}"
JOBS="$(($(nproc) - 1))"

for tool in capture csvexport; do
  echo "=== Building tracy-${tool} ==="
  cmake -S "${TRACY_SRC}/${tool}" -B "${OUT_DIR}/${tool}" \
    -G Ninja \
    -DCMAKE_BUILD_TYPE=Release \
    -DNO_FILESELECTOR=ON
  cmake --build "${OUT_DIR}/${tool}" -j "${JOBS}"
  cp "${OUT_DIR}/${tool}/tracy-${tool}" "${BIN_DIR}/tracy-${tool}"
done

echo "Built:"
ls -la "${BIN_DIR}"
