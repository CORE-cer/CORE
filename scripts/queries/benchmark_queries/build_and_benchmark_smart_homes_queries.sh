#!/bin/bash

# Work at the root directory
cd "$(dirname "$0")" || exit
cd ../../.. || exit

export BENCHMARK_BASE_DIR="src/targets/experiments/smart_homes"
export BENCHMARK_CSV="smart_homes_data.csv"
export BENCHMARK_DECLARATION="declaration.core"
export BENCHMARK_COMPRESSED_RESULTS="expected_results.tar.xz"

exec scripts/queries/benchmark_queries/benchmark_runner.sh "$@"
