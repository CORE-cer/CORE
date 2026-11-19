#!/bin/bash
#
# Unattended benchmark run (guides/backlog.md items 22, 23, 24), pure C++ through
# core_bench. Resumable: re-running with the same output directory skips every run
# already recorded, so a crash or reboot only loses the run in progress.
#
#   tests/benchmark/run_overnight.sh [OUT_DIR] [--smoke] [--stages S0,S1,S2,S3,S4]
#
# OUT_DIR defaults to tests/benchmark/results/overnight/<YYYYMMDD>. --smoke runs a
# tiny version of every stage (2 queries, 1 repeat, 20k events) to check the
# pipeline before a real night. --stages runs only the listed stages (default:
# all); because runs are resumable, a later full invocation on the same OUT_DIR
# skips whatever already finished.
#
# Stages, in order (each converted to CSV as soon as it finishes):
#   S0  item 23: the same 9 queries measured three ways - plain chrono,
#       copy-and-subtract (split) chrono, Tracy coarse corrected by plain chrono
#   S1  synthetic corpus (queries_optimization/), plain chrono
#   S2  synthetic corpus, Tracy coarse (phase breakdown), corrected by S1
#   S3  synthetic corpus, Tracy fine, 200k events (exact per-event op counts)
#   S4  the repo's real queries (queries/), chrono then Tracy coarse; match
#       counts verified against expected_results
#
# Every timing build must be Release (-O3 -DNDEBUG); the script refuses to start
# otherwise. Runs under systemd-inhibit so the machine cannot suspend mid-run.

set -euo pipefail
cd "$(dirname "$0")/../.."

SMOKE=0
OUT=""
STAGES="S0,S1,S2,S3,S4"
ARGS=("$@")
while [ $# -gt 0 ]; do
  case "$1" in
    --smoke) SMOKE=1 ;;
    --stages) shift; STAGES="$1" ;;
    *) OUT="$1" ;;
  esac
  shift
done
set -- "${ARGS[@]}"
want() { [[ ",$STAGES," == *",$1,"* ]]; }
OUT="${OUT:-tests/benchmark/results/overnight/$(date +%Y%m%d)}"

# Re-exec under a sleep/idle/lid inhibitor (only once).
if [ -z "${CORE_OVERNIGHT_INHIBITED:-}" ]; then
  export CORE_OVERNIGHT_INHIBITED=1
  exec systemd-inhibit --what=sleep:idle:handle-lid-switch --who=core_bench \
    --why="CORE overnight benchmark" --mode=block "$0" "$@"
fi

PY=.venv/bin/python
RUNS=tests/benchmark/tracy_runs.py
CONV=tests/benchmark/tracy_to_csv.py
PLAIN=build/Release-opt
PROF=build/Release-opt-prof
FINE=build/Release-opt-prof-fine

for dir in "$PLAIN" "$PROF" "$FINE"; do
  [ -x "$dir/core_bench" ] || { echo "missing $dir/core_bench" >&2; exit 1; }
  grep -q '^CMAKE_BUILD_TYPE:STRING=Release$' "$dir/CMakeCache.txt" \
    || { echo "$dir is not a Release build" >&2; exit 1; }
  grep -q '^CMAKE_CXX_FLAGS_RELEASE:STRING=-O3 -DNDEBUG$' "$dir/CMakeCache.txt" \
    || { echo "$dir does not use -O3 -DNDEBUG" >&2; exit 1; }
done
[ -x build/tracy-tools/bin/tracy-capture ] || { echo "run scripts/build_tracy_tools.sh" >&2; exit 1; }

mkdir -p "$OUT"
{
  echo "started: $(date -Is)"
  echo "commit: $(git rev-parse HEAD)"
  git status --short
  echo "governors: $(cat /sys/devices/system/cpu/cpu*/cpufreq/scaling_governor | sort | uniq -c | tr '\n' ' ')"
} >> "$OUT/run_info.txt"

if [ "$SMOKE" = 1 ]; then
  ITEM23="smart_homes/rep_eq_24.txt smart_homes/thr_ladder_08.txt"
  SYN="smart_homes/rep_eq_0[34].txt"
  SYN_TAXIS="taxis/str_rep_eq_04.txt"
  REAL="stocks/q1_*.txt taxis/q1_*.txt"
  EV=20000; FINE_EV=20000; R_CHRONO=1; R_TRACY=1; R_REAL=1
else
  ITEM23="smart_homes/floor_01.txt smart_homes/rep_eq_24.txt smart_homes/eq_ladder_16.txt
          smart_homes/thr_ladder_08.txt smart_homes/worst_mix_16.txt
          smart_homes/late_exit_thr_08.txt smart_homes/late_exit_thr_16.txt
          smart_homes/rep_arith_int_single_24.txt taxis/str_eq_ladder_08.txt"
  SYN="smart_homes/*"
  SYN_TAXIS="taxis/*"
  REAL="stocks/* smart_homes/* taxis/*"
  EV=0; FINE_EV=200000; R_CHRONO=5; R_TRACY=3; R_REAL=3
fi
# smart_homes q3_none/q4_none: 7.85/6.67 GB of expected output, excluded like tests/e2e.
REAL_EXCLUDE="smart_homes/q3_none.txt smart_homes/q4_none.txt"

stage() { echo "=== $1 $(date -Is)" | tee -a "$OUT/run_info.txt"; }

# From here on: a failed stage is logged but must not stop the rest of the night,
# and dataset/* patterns are globbed by tracy_runs.py, not by the shell.
set +e
set -f

if want S0; then
  stage "S0 item 23"
  $PY $RUNS --mode chrono --build-dir $PLAIN --queries $ITEM23 --repeats $R_CHRONO \
    --max-events $EV --out-dir "$OUT/s0_item23_chrono" >> "$OUT/s0_chrono.log" 2>&1
  $PY $RUNS --mode chrono --method split --build-dir $PLAIN --queries $ITEM23 \
    --repeats $R_CHRONO --max-events $EV --out-dir "$OUT/s0_item23_split" \
    >> "$OUT/s0_split.log" 2>&1
  $PY $RUNS --mode tracy --build-dir $PROF --queries $ITEM23 --repeats $R_CHRONO \
    --max-events $EV --export-and-delete --out-dir "$OUT/s0_item23_tracy" \
    >> "$OUT/s0_tracy.log" 2>&1
  $PY $CONV "$OUT/s0_item23_tracy" --chrono-dir "$OUT/s0_item23_chrono" \
    > "$OUT/s0_convert.log" 2>&1
fi

if want S1; then
  stage "S1 synthetic corpus, chrono"
  $PY $RUNS --mode chrono --build-dir $PLAIN --queries $SYN --repeats $R_CHRONO \
    --max-events $EV --out-dir "$OUT/s1_corpus_chrono" >> "$OUT/s1.log" 2>&1
  $PY $RUNS --mode chrono --build-dir $PLAIN --queries $SYN_TAXIS \
    --strategies baseline minterm_tree minterm_tree_interned --repeats $R_CHRONO \
    --max-events $EV --out-dir "$OUT/s1_corpus_chrono" >> "$OUT/s1.log" 2>&1
fi

if want S2; then
  stage "S2 synthetic corpus, Tracy coarse"
  $PY $RUNS --mode tracy --build-dir $PROF --queries $SYN --repeats $R_TRACY \
    --max-events $EV --export-and-delete --out-dir "$OUT/s2_corpus_tracy" >> "$OUT/s2.log" 2>&1
  $PY $RUNS --mode tracy --build-dir $PROF --queries $SYN_TAXIS \
    --strategies baseline minterm_tree minterm_tree_interned --repeats $R_TRACY \
    --max-events $EV --export-and-delete --out-dir "$OUT/s2_corpus_tracy" >> "$OUT/s2.log" 2>&1
  $PY $CONV "$OUT/s2_corpus_tracy" --chrono-dir "$OUT/s1_corpus_chrono" \
    > "$OUT/s2_convert.log" 2>&1
fi

if want S3; then
  stage "S3 synthetic corpus, Tracy fine (op counts)"
  $PY $RUNS --mode tracy --build-dir $FINE --queries $SYN $SYN_TAXIS --repeats 1 \
    --max-events $FINE_EV --export-and-delete --out-dir "$OUT/s3_corpus_fine" >> "$OUT/s3.log" 2>&1
  $PY $CONV "$OUT/s3_corpus_fine" > "$OUT/s3_convert.log" 2>&1
fi

if want S4; then
  stage "S4 real queries, chrono"
  $PY $RUNS --mode chrono --build-dir $PLAIN --query-dir queries --queries $REAL \
    --exclude $REAL_EXCLUDE --strategies baseline minterm_tree minterm_tree_interned \
    --repeats $R_REAL --max-events $EV --out-dir "$OUT/s4_real_chrono" \
    >> "$OUT/s4_chrono.log" 2>&1
  stage "S4 real queries, Tracy coarse"
  $PY $RUNS --mode tracy --build-dir $PROF --query-dir queries --queries $REAL \
    --exclude $REAL_EXCLUDE --strategies baseline minterm_tree minterm_tree_interned \
    --repeats 1 --max-events $EV --export-and-delete --out-dir "$OUT/s4_real_tracy" \
    >> "$OUT/s4_tracy.log" 2>&1
  $PY $CONV "$OUT/s4_real_tracy" --chrono-dir "$OUT/s4_real_chrono" \
    > "$OUT/s4_convert.log" 2>&1
fi

stage "done ($STAGES)"
