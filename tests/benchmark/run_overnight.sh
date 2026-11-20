#!/bin/bash
#
# Unattended benchmark run (guides/backlog.md items 22, 23, 24), pure C++ through
# core_bench. Resumable: re-running with the same output directory skips every run
# already recorded, so a crash or reboot only loses the run in progress.
#
#   tests/benchmark/run_overnight.sh [OUT_DIR] [--smoke] [--stages S0,S1,S2,S3,S4]
#                                    [--compare EARLIER_OUT_DIR]
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
#   S5  replay decomposition (backlog item 29, guides/benchmarking/
#       09-replay-decomposition.md): corpus, then the real queries, five
#       core_bench methods each, then decompose.py with its checks. Not in the
#       default stage list: run it with --stages S5. --compare points at an
#       earlier run's directory (its S1/S2/S4) for decompose.py's cross-checks.
#
# Every timing build must be Release (-O3 -DNDEBUG); the script refuses to start
# otherwise. Runs under systemd-inhibit so the machine cannot suspend mid-run.
#
# Machine state (guides/benchmarking/09-replay-decomposition.md, "Drift between
# nights"): on this laptop the clock follows the package temperature, and a run
# started hot was ~8% slower than one started cool. So the script
#   * switches the platform power profile to 'performance' (cooler, steadier
#     clock). power-profiles-daemon cannot switch back while the CPU governor is
#     'performance', so it stays there; restore with
#     `echo balanced | sudo tee /sys/firmware/acpi/platform_profile`. After such
#     a manual write the daemon is out of step and its own 'set performance'
#     does nothing, so the script checks the firmware value and warns loudly in
#     run_info.txt if it is not 'performance' (same command, with performance);
#   * waits before every stage until the package is below COOL_C (60 C, at most
#     COOL_MAX_S seconds) and logs the wait;
#   * logs the machine state (profile, EPP, governors, AC, temperature, load)
#     at the start and at every stage, into run_info.txt;
#   * runs a reference probe (4 fixed queries x 2 strategies, plain chrono)
#     first and last, into probe_start/ and probe_end/: every night's own speed
#     anchor, which decompose.py uses to tell machine drift from a code change.
# tracy_runs.py also records each run's clock and package temperature.

set -euo pipefail
cd "$(dirname "$0")/../.."

SMOKE=0
OUT=""
COMPARE=""
STAGES="S0,S1,S2,S3,S4"
ARGS=("$@")
while [ $# -gt 0 ]; do
  case "$1" in
    --smoke) SMOKE=1 ;;
    --stages) shift; STAGES="$1" ;;
    --compare) shift; COMPARE="$1" ;;
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

# The CPUs tracy_runs.py pins core_bench to (its --taskset default).
PINNED="2 4"
COOL_C=60
COOL_MAX_S=600

# The stages run with globbing off (set -f), so the helpers that glob sysfs
# turn it back on for themselves only (local -).
pkg_temp() {
  local - zone
  set +f
  for zone in /sys/class/thermal/thermal_zone*; do
    if [ "$(cat "$zone/type" 2>/dev/null)" = x86_pkg_temp ]; then
      echo $(( $(cat "$zone/temp") / 1000 ))
      return
    fi
  done
  echo "?"
}

env_info() {
  local - cpu epp="" ac=""
  set +f
  for cpu in $PINNED; do
    epp+="cpu$cpu=$(cat /sys/devices/system/cpu/cpu$cpu/cpufreq/energy_performance_preference 2>/dev/null) "
  done
  for supply in /sys/class/power_supply/*; do
    [ "$(cat "$supply/type" 2>/dev/null)" = Mains ] && ac+="$(basename "$supply")=$(cat "$supply/online") "
  done
  echo "  platform_profile=$(cat /sys/firmware/acpi/platform_profile 2>/dev/null)" \
    "ppd=$(powerprofilesctl get 2>/dev/null) epp: $epp" \
    "no_turbo=$(cat /sys/devices/system/cpu/intel_pstate/no_turbo 2>/dev/null)" \
    "ac: $ac pkg_temp=$(pkg_temp)C load=$(cut -d' ' -f1-3 /proc/loadavg)" \
    "uptime_s=$(cut -d' ' -f1 /proc/uptime)"
}

# Wait until the package is below COOL_C, at most COOL_MAX_S seconds.
cool_down() {
  local waited=0 temp
  temp=$(pkg_temp)
  while [ "$temp" != "?" ] && [ "$temp" -ge "$COOL_C" ] && [ "$waited" -lt "$COOL_MAX_S" ]; do
    sleep 10
    waited=$((waited + 10))
    temp=$(pkg_temp)
  done
  echo "  cool-down: waited ${waited}s, pkg_temp=${temp}C (target <${COOL_C}C)"
}

mkdir -p "$OUT"
{
  echo "started: $(date -Is)"
  echo "commit: $(git rev-parse HEAD)"
  git status --short
  echo "governors: $(cat /sys/devices/system/cpu/cpu*/cpufreq/scaling_governor | sort | uniq -c | tr '\n' ' ')"
  # power-profiles-daemon can be out of step with the firmware (after a manual
  # write to platform_profile it still reports its own last profile, and its
  # 'set' is then a no-op), so the firmware value is what gets checked.
  powerprofilesctl set performance 2>&1
  if [ "$(cat /sys/firmware/acpi/platform_profile 2>/dev/null)" = performance ]; then
    echo "platform profile: performance"
  else
    echo "WARNING: platform profile is '$(cat /sys/firmware/acpi/platform_profile 2>/dev/null)'," \
      "not performance; timings will be slower and noisier. Fix with:" \
      "echo performance | sudo tee /sys/firmware/acpi/platform_profile"
  fi
  env_info
} >> "$OUT/run_info.txt" 2>&1

if [ "$SMOKE" = 1 ]; then
  ITEM23="smart_homes/rep_eq_24.txt smart_homes/thr_ladder_08.txt"
  SYN="smart_homes/rep_eq_0[34].txt"
  SYN_TAXIS="taxis/str_rep_eq_04.txt"
  REAL="stocks/q1_*.txt taxis/q1_*.txt"
  EV=20000; FINE_EV=20000; R_CHRONO=1; R_TRACY=1; R_REAL=1; R_DEC=1; R_PROBE=1
  COOL_MAX_S=60
else
  ITEM23="smart_homes/floor_01.txt smart_homes/rep_eq_24.txt smart_homes/eq_ladder_16.txt
          smart_homes/thr_ladder_08.txt smart_homes/worst_mix_16.txt
          smart_homes/late_exit_thr_08.txt smart_homes/late_exit_thr_16.txt
          smart_homes/rep_arith_int_single_24.txt taxis/str_eq_ladder_08.txt"
  SYN="smart_homes/*"
  SYN_TAXIS="taxis/*"
  REAL="stocks/* smart_homes/* taxis/*"
  EV=0; FINE_EV=200000; R_CHRONO=5; R_TRACY=3; R_REAL=3; R_DEC=3; R_PROBE=3
fi
# smart_homes q3_none/q4_none: 7.85/6.67 GB of expected output, excluded like tests/e2e.
REAL_EXCLUDE="smart_homes/q3_none.txt smart_homes/q4_none.txt"

# Fixed reference set for the probe (do not change it: its whole point is to be
# the same every night). Four shapes: the floor, one equality, a threshold
# ladder, repeated equalities.
PROBE="smart_homes/floor_01.txt smart_homes/control_eq_01.txt
       smart_homes/thr_ladder_08.txt smart_homes/rep_eq_04.txt"

stage() {
  { cool_down; echo "=== $1 $(date -Is)"; env_info; } 2>&1 | tee -a "$OUT/run_info.txt"
}

probe() {
  stage "probe ($1)"
  $PY $RUNS --mode chrono --build-dir $PLAIN --queries $PROBE --repeats $R_PROBE \
    --max-events $EV --out-dir "$OUT/probe_$1" >> "$OUT/probe.log" 2>&1
}

# From here on: a failed stage is logged but must not stop the rest of the night,
# and dataset/* patterns are globbed by tracy_runs.py, not by the shell.
set +e
set -f

probe start

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

if want S5; then
  DEC="plain predicate replay replay_no_out calib"
  DECOMPOSE=tests/benchmark/decompose.py
  # Cross-checks only when an earlier run's directories exist.
  cmp_args() {
    local args=()
    [ -n "$COMPARE" ] && [ -f "$COMPARE/$1/manifest.csv" ] && args+=(--compare-chrono "$COMPARE/$1")
    [ -n "$COMPARE" ] && [ -f "$COMPARE/$2/tracy_phases.csv" ] && args+=(--compare-tracy "$COMPARE/$2")
    echo "${args[@]}"
  }
  stage "S5 replay decomposition, synthetic corpus"
  $PY $RUNS --mode chrono --build-dir $PLAIN --queries $SYN --methods $DEC \
    --repeats $R_DEC --max-events $EV --out-dir "$OUT/s5_corpus_decomp" >> "$OUT/s5.log" 2>&1
  $PY $RUNS --mode chrono --build-dir $PLAIN --queries $SYN_TAXIS --methods $DEC \
    --strategies baseline minterm_tree minterm_tree_interned --repeats $R_DEC \
    --max-events $EV --out-dir "$OUT/s5_corpus_decomp" >> "$OUT/s5.log" 2>&1
  $PY $DECOMPOSE "$OUT/s5_corpus_decomp" $(cmp_args s1_corpus_chrono s2_corpus_tracy) \
    > "$OUT/s5_corpus_decompose.log" 2>&1
  stage "S5 replay decomposition, real queries"
  $PY $RUNS --mode chrono --build-dir $PLAIN --query-dir queries --queries $REAL \
    --exclude $REAL_EXCLUDE --strategies baseline minterm_tree --methods $DEC \
    --repeats $R_DEC --max-events $EV --out-dir "$OUT/s5_real_decomp" >> "$OUT/s5.log" 2>&1
  $PY $DECOMPOSE "$OUT/s5_real_decomp" $(cmp_args s4_real_chrono s4_real_tracy) \
    > "$OUT/s5_real_decompose.log" 2>&1
fi

probe end

stage "done ($STAGES)"
