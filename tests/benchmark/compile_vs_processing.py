"""Benchmark query-compile time vs. event-processing time, across the
baseline and minterm-tree strategies.

Separate from benchmark_queries.py (not modified, not imported from here) -
reuses only the shared e2e test infrastructure both scripts already depend
on (tests/e2e/conftest.py's DATASETS, tests/e2e/csv_parser.py's parse_csv).

benchmark_queries.py starts its timer *after* add_query() returns, so it
only ever measures event-processing cost - never query-compile cost, which
for a MINTERM_TREE strategy includes building the Z3 decision tree inside
add_query() itself. This script times both, separately, for every
(dataset, query, strategy, repeat) combination, and writes one row per
combination to a CSV for later analysis. See
guides/benchmarking/04-the-critical-finding.md for the full writeup.

Usage:
    uv run python tests/benchmark/compile_vs_processing.py --dataset taxis --repeats 1
    uv run python tests/benchmark/compile_vs_processing.py --dataset taxis --dataset stocks \
        --strategies baseline,minterm_tree_interned
    .venv/bin/python tests/benchmark/compile_vs_processing.py --dataset smart_homes \
        --queries-dir queries_optimization --parity --repeats 3

--queries-dir selects another queries folder inside the dataset (the new
optimization-friendly queries live in smart_homes/queries_optimization/).

--cycles K repeats the whole dataset K times, shifting --cycle-attr by one
period per copy (span + --cycle-gap), so the event count grows with the same
value and key distribution. Only meaningful for a dataset whose --cycle-attr
is an integer column that the queries' WITHIN windows read.

--parity checks every tree strategy's output against the baseline's output
on the same events (repeat 1 only), by SHA-256 digest, so no output is kept
in memory. It needs no stored expected results, so it is the correctness
check for queries without an expected_results file. Mismatches are printed
and recorded, and the run continues. A mismatch also re-runs the baseline and
the tree strategy and writes both outputs to tests/benchmark/results/parity_mismatch/
for diff.

Every row records output_bytes (the UTF-8 size of the query output), so output
size can be compared across queries and event counts.

--max-events N truncates the event list, for quick smoke tests.

Rows are written to the CSV as they are produced, so a crash keeps the
finished rows.

Any MINTERM_TREE-based strategy raises at PyOfflineServer construction if
pycer wasn't built with -o: `CMAKE_BUILD_PARALLEL_LEVEL=6 scripts/build_pycer.sh -o`.
If you need to be certain pycer wasn't silently rebuilt *without* -o by an
environment sync in between, use `.venv/bin/python` instead of `uv run` -
`uv run` re-runs `uv sync` first, which can drop -o (see guides/backlog.md
item 9).
"""

import argparse
import csv
import hashlib
import resource
import shutil
import sys
import tarfile
import time
from functools import partial
from pathlib import Path
from typing import TextIO

_PROJECT_ROOT = str(Path(__file__).resolve().parent.parent.parent)
if _PROJECT_ROOT not in sys.path:
    sys.path.insert(0, _PROJECT_ROOT)

import pycer

from tests.e2e.conftest import DATASETS, _version_sort_key
from tests.e2e.csv_parser import parse_csv

RESULTS_DIR = Path(__file__).parent / "results"

STRATEGIES: dict[str, dict[str, object]] = {
    "baseline": {},
    "minterm_tree": {
        "predicate_evaluation": pycer.PyPredicateEvaluation.MINTERM_TREE,
        "string_equality": pycer.PyStringEquality.OPAQUE,
        "atom_ordering": pycer.PyAtomOrdering.AS_DISCOVERED,
    },
    "minterm_tree_ordered": {
        "predicate_evaluation": pycer.PyPredicateEvaluation.MINTERM_TREE,
        "string_equality": pycer.PyStringEquality.OPAQUE,
        "atom_ordering": pycer.PyAtomOrdering.MOST_SHARED_ATTRIBUTES_FIRST,
    },
    "minterm_tree_interned": {
        "predicate_evaluation": pycer.PyPredicateEvaluation.MINTERM_TREE,
        "string_equality": pycer.PyStringEquality.INTERNED_EQUALITY,
        "atom_ordering": pycer.PyAtomOrdering.AS_DISCOVERED,
    },
    "minterm_tree_interned_ordered": {
        "predicate_evaluation": pycer.PyPredicateEvaluation.MINTERM_TREE,
        "string_equality": pycer.PyStringEquality.INTERNED_EQUALITY,
        "atom_ordering": pycer.PyAtomOrdering.MOST_SHARED_ATTRIBUTES_FIRST,
    },
}

CSV_HEADER = (
    "dataset,query,strategy,repeat,event_count,compile_time_s,"
    "processing_time_s,matched_expected,matches_baseline,output_bytes\n"
)

DEFAULT_DATASETS = ["taxis", "stocks", "unordered_stocks"]
BASELINE = "baseline"


def extract_archives(dataset: dict) -> None:
    base = dataset["base"]
    expected_dir = base / "expected_results"
    archive = base / "expected_results.tar.xz"
    if not expected_dir.exists() and archive.exists():
        with tarfile.open(archive) as tf:
            tf.extractall(path=base, filter="data")
    if "csv_archive" in dataset:
        csv_path = base / dataset["csv"]
        csv_archive = base / dataset["csv_archive"]
        if not csv_path.exists() and csv_archive.exists():
            with tarfile.open(csv_archive) as tf:
                tf.extractall(path=base, filter="data")


def cleanup_archives(dataset: dict) -> None:
    expected_dir = dataset["base"] / "expected_results"
    if expected_dir.exists():
        shutil.rmtree(expected_dir)


def discover_queries(
    dataset: dict, query_filter: set[str] | None, queries_dir_name: str
) -> list[tuple[Path, Path]]:
    base = dataset["base"]
    queries_dir = base / queries_dir_name
    expected_dir = base / "expected_results"
    if not queries_dir.exists():
        return []
    cases = []
    for query_file in sorted(queries_dir.iterdir(), key=_version_sort_key):
        if query_file.name in dataset.get("exclude", []):
            continue
        if query_filter and query_file.name not in query_filter:
            continue
        # Queries without an expected_results file (the new query folders) get
        # a path that does not exist; verify_output treats that as "not checked".
        cases.append((query_file, expected_dir / query_file.name))
    return cases


def verify_output(
    output: str,
    query_text: str,
    event_count: int,
    expected_file: Path,
    max_verify_size_mb: float,
) -> bool | None:
    """True/False if checked against expected_results, None if skipped
    (missing file, or too large to safely load for comparison)."""
    if not expected_file.exists():
        return None
    if expected_file.stat().st_size > max_verify_size_mb * 1024 * 1024:
        return None
    header = f"Query: {query_text}\nRead events {event_count}\n"
    return (header + output) == expected_file.read_text()


MISMATCH_DIR = RESULTS_DIR / "parity_mismatch"


def attribute_range(csv_path: Path, attr: str) -> tuple[int, int]:
    """Smallest and largest integer value of one column (one streaming pass)."""
    lo: int | None = None
    hi: int | None = None
    with open(csv_path, newline="") as f:
        reader = csv.DictReader(f)
        if reader.fieldnames is None or attr not in reader.fieldnames:
            raise ValueError(f"--cycle-attr '{attr}' is not a column of {csv_path.name}")
        for row in reader:
            value = int(row[attr])
            lo = value if lo is None else min(lo, value)
            hi = value if hi is None else max(hi, value)
    if lo is None or hi is None:
        raise ValueError(f"{csv_path.name} has no data rows")
    return lo, hi


def load_cycled_events(
    csv_path: Path,
    stream_info: pycer.PyStreamInfo,
    cycles: int,
    gap: int,
    cycle_attr: str,
    max_events: int | None,
) -> tuple[list, list[int]]:
    """Parses the CSV `cycles` times, shifting `cycle_attr` by one period per copy.

    The period is the attribute's span plus `gap`, so copies follow one another
    in time and no window (WITHIN n) can cross a copy boundary. Each copy has the
    same value and key distribution, so per-copy matches are unchanged. cycles=1
    is a plain parse. max_events, if set, keeps the first N events of each copy.
    """
    shift_attr = None
    period = 0
    if cycles > 1:
        lo, hi = attribute_range(csv_path, cycle_attr)
        shift_attr = cycle_attr
        period = (hi - lo) + gap

    events: list = []
    delays_ns: list[int] = []
    for copy in range(cycles):
        copy_events, copy_delays = parse_csv(
            csv_path, stream_info, shift_attr=shift_attr, shift=copy * period
        )
        if max_events is not None:
            copy_events = copy_events[:max_events]
            copy_delays = copy_delays[:max_events]
        events.extend(copy_events)
        delays_ns.extend(copy_delays)
    return events, delays_ns


def sha256_of(output: str) -> str:
    # Parity keeps this digest instead of the output, so memory does not grow with output size.
    return hashlib.sha256(output.encode("utf-8")).hexdigest()


def run_once(
    strategy_kwargs: dict[str, object],
    *,
    declaration_text: str,
    options_text: str | None,
    query_text: str,
    events: list,
    delays_ns: list[int],
    has_timing: bool,
) -> tuple[float, float, str]:
    """One fresh server: returns (compile_time_s, processing_time_s, output).

    Only the add_query call is timed as compile, and only the send calls plus
    get_output as processing, as before.
    """
    server = pycer.PyOfflineServer(**strategy_kwargs)
    server.declare_stream(declaration_text)
    if options_text:
        server.declare_option(options_text)

    compile_start = time.perf_counter()
    server.add_query(query_text)
    compile_time_s = time.perf_counter() - compile_start

    processing_start = time.perf_counter()
    if has_timing:
        for event, delay_ns in zip(events, delays_ns):
            if delay_ns > 0:
                time.sleep(delay_ns / 1e9)
            server.send_event(event)
    else:
        server.send_events(events)
    output = server.get_output()
    processing_time_s = time.perf_counter() - processing_start
    return compile_time_s, processing_time_s, output


def dump_mismatch(query_name: str, strategy_name: str, run) -> Path:
    """Re-runs the baseline and the mismatching strategy once and writes both
    outputs to MISMATCH_DIR, for a diff. Only runs on a mismatch, so its cost is rare."""
    MISMATCH_DIR.mkdir(parents=True, exist_ok=True)
    _, _, baseline_output = run(STRATEGIES[BASELINE])
    _, _, tree_output = run(STRATEGIES[strategy_name])
    stem = MISMATCH_DIR / f"{query_name}.{strategy_name}"
    Path(f"{stem}.baseline.txt").write_text(baseline_output)
    Path(f"{stem}.tree.txt").write_text(tree_output)
    return stem


def benchmark_dataset(
    dataset: dict,
    csv_file: TextIO,
    repeats: int,
    strategies: list[str],
    verify: bool,
    max_verify_size_mb: float,
    query_filter: set[str] | None,
    queries_dir_name: str,
    parity: bool,
    max_events: int | None,
    cycles: int,
    cycle_gap: int,
    cycle_attr: str,
) -> tuple[int, int]:
    """Benchmarks one dataset and appends one CSV row per run to csv_file.

    Returns (parity_checked, parity_mismatches). With parity on, the baseline
    must be the first strategy in the list (main() arranges this), because each
    tree strategy is compared against the baseline output of the same query.
    """
    extract_archives(dataset)
    base = dataset["base"]
    declaration_text = (base / dataset["declaration"]).read_text()
    options_text = (base / dataset["options"]).read_text() if dataset["options"] else None

    tmp_server = pycer.PyOfflineServer()
    stream_info = tmp_server.declare_stream(declaration_text)
    csv_path = base / dataset["csv"]
    events, delays_ns = load_cycled_events(
        csv_path, stream_info, cycles, cycle_gap, cycle_attr, max_events
    )
    has_timing = any(d > 0 for d in delays_ns)
    del tmp_server

    queries = discover_queries(dataset, query_filter, queries_dir_name)
    if not queries:
        print(f"  No queries found for {dataset['name']} in {queries_dir_name}/", file=sys.stderr)
        return 0, 0

    parity_checked = 0
    parity_mismatches = 0
    for query_file, expected_file in queries:
        query_text = query_file.read_text()
        query_name = query_file.name
        # Only the baseline's digest is kept while this query's strategies run.
        baseline_digest: str | None = None
        run = partial(
            run_once,
            declaration_text=declaration_text,
            options_text=options_text,
            query_text=query_text,
            events=events,
            delays_ns=delays_ns,
            has_timing=has_timing,
        )

        for strategy_name in strategies:
            strategy_kwargs = STRATEGIES[strategy_name]
            print(f"  {query_name} [{strategy_name}]...", file=sys.stderr)

            for repeat in range(1, repeats + 1):
                compile_time_s, processing_time_s, output = run(strategy_kwargs)
                output_bytes = len(output.encode("utf-8"))

                matched: bool | None = None
                if verify and repeat == 1:
                    matched = verify_output(
                        output, query_text, len(events), expected_file, max_verify_size_mb
                    )
                    if matched is False:
                        print(f"    MISMATCH for {query_name} [{strategy_name}]", file=sys.stderr)

                # Parity compares repeat-1 output only; repeats 2-3 are for timing.
                matches_baseline: bool | None = None
                if parity and repeat == 1:
                    digest = sha256_of(output)
                    if strategy_name == BASELINE:
                        baseline_digest = digest
                    else:
                        matches_baseline = digest == baseline_digest
                        parity_checked += 1
                        if not matches_baseline:
                            parity_mismatches += 1
                            stem = dump_mismatch(query_name, strategy_name, run)
                            print(
                                f"    PARITY MISMATCH: {query_name} [{strategy_name}] "
                                f"differs from baseline; outputs written to {stem}.*.txt",
                                file=sys.stderr,
                            )

                write_row(
                    csv_file,
                    {
                        "dataset": dataset["name"],
                        "query": query_name,
                        "strategy": strategy_name,
                        "repeat": repeat,
                        "event_count": len(events),
                        "compile_time_s": compile_time_s,
                        "processing_time_s": processing_time_s,
                        "matched_expected": matched,
                        "matches_baseline": matches_baseline,
                        "output_bytes": output_bytes,
                    },
                )

                print(
                    f"    run {repeat}/{repeats}: compile {compile_time_s:.4f}s, "
                    f"processing {processing_time_s:.4f}s, output {output_bytes} bytes",
                    file=sys.stderr,
                )

    cleanup_archives(dataset)
    return parity_checked, parity_mismatches


def open_csv(output_path: Path) -> TextIO:
    output_path.parent.mkdir(parents=True, exist_ok=True)
    csv_file = open(output_path, "w")
    csv_file.write(CSV_HEADER)
    csv_file.flush()
    return csv_file


def write_row(csv_file: TextIO, row: dict) -> None:
    # Flushed per row, so an interrupted run keeps every finished row.
    matched = "" if row["matched_expected"] is None else str(row["matched_expected"])
    matches_baseline = "" if row["matches_baseline"] is None else str(row["matches_baseline"])
    csv_file.write(
        f"{row['dataset']},{row['query']},{row['strategy']},{row['repeat']},"
        f"{row['event_count']},{row['compile_time_s']:.6f},"
        f"{row['processing_time_s']:.6f},{matched},{matches_baseline},{row['output_bytes']}\n"
    )
    csv_file.flush()


def main() -> None:
    parser = argparse.ArgumentParser(
        description="Benchmark query-compile time vs. event-processing time, "
        "across baseline and minterm-tree strategies."
    )
    parser.add_argument(
        "--dataset",
        action="append",
        dest="datasets",
        default=None,
        help=f"Dataset to benchmark (repeatable). Default: {', '.join(DEFAULT_DATASETS)}.",
    )
    parser.add_argument(
        "--repeats", type=int, default=3, help="Repeats per (query, strategy). Default: 3."
    )
    parser.add_argument(
        "--strategies",
        type=str,
        default=",".join(STRATEGIES.keys()),
        help=f"Comma-separated strategy names. Available: {', '.join(STRATEGIES.keys())}.",
    )
    parser.add_argument(
        "--query",
        action="append",
        default=None,
        help="Only this query filename (e.g. q1_any.txt), within each selected dataset. "
        "Repeatable.",
    )
    parser.add_argument(
        "--queries-dir",
        type=str,
        default="queries",
        help="Queries folder inside each dataset. Default: queries.",
    )
    parser.add_argument(
        "--parity",
        action="store_true",
        help="Compare each tree strategy's output with the baseline's (repeat 1). "
        "Adds the baseline to the strategies if it is missing.",
    )
    parser.add_argument(
        "--max-events",
        type=int,
        default=None,
        help="Use only the first N events of each dataset (smoke tests). Default: all.",
    )
    parser.add_argument(
        "--cycles",
        type=int,
        default=1,
        help="Repeat the dataset this many times, shifting --cycle-attr by one period per "
        "copy, so the event count grows with the same distribution. Default: 1 (no cycling).",
    )
    parser.add_argument(
        "--cycle-gap",
        type=int,
        default=3600,
        help="Gap between copies, in units of --cycle-attr. Must exceed the largest WITHIN "
        "window. Default: 3600.",
    )
    parser.add_argument(
        "--cycle-attr",
        type=str,
        default="plug_timestamp",
        help="Integer column shifted for each copy (smart_homes: plug_timestamp). "
        "Default: plug_timestamp.",
    )
    parser.add_argument(
        "--no-verify",
        action="store_true",
        help="Skip comparing output against expected_results entirely.",
    )
    parser.add_argument(
        "--max-verify-size-mb",
        type=float,
        default=200.0,
        help="Skip the correctness check for expected_results files larger than this. Default: 200.",
    )
    parser.add_argument(
        "--output",
        type=str,
        default=None,
        help="Output CSV path. Default: tests/benchmark/results/compile_vs_processing_<timestamp>.csv.",
    )
    args = parser.parse_args()
    if args.cycles < 1 or args.cycle_gap < 0:
        print("--cycles must be >= 1 and --cycle-gap must be >= 0", file=sys.stderr)
        sys.exit(1)

    dataset_names = args.datasets or DEFAULT_DATASETS
    datasets = [d for d in DATASETS if d["name"] in dataset_names]
    missing = set(dataset_names) - {d["name"] for d in datasets}
    if missing:
        valid = ", ".join(d["name"] for d in DATASETS)
        print(f"Unknown dataset(s): {', '.join(missing)}. Valid: {valid}", file=sys.stderr)
        sys.exit(1)

    strategy_names = [s.strip() for s in args.strategies.split(",") if s.strip()]
    unknown_strategies = set(strategy_names) - set(STRATEGIES)
    if unknown_strategies:
        print(
            f"Unknown strategy(ies): {', '.join(unknown_strategies)}. "
            f"Available: {', '.join(STRATEGIES)}",
            file=sys.stderr,
        )
        sys.exit(1)

    if args.parity:
        # The baseline has to run first for each query, so its output is there to compare against.
        if BASELINE not in strategy_names:
            print("--parity: adding the baseline strategy for the comparison", file=sys.stderr)
            strategy_names.insert(0, BASELINE)
        else:
            strategy_names.remove(BASELINE)
            strategy_names.insert(0, BASELINE)

    if args.output:
        output_path = Path(args.output)
    else:
        timestamp = time.strftime("%Y%m%d_%H%M%S")
        output_path = RESULTS_DIR / f"compile_vs_processing_{timestamp}.csv"

    csv_file = open_csv(output_path)
    parity_checked = 0
    parity_mismatches = 0
    try:
        for dataset in datasets:
            print(f"\n=== Benchmarking {dataset['name']} ===", file=sys.stderr)
            checked, mismatches = benchmark_dataset(
                dataset,
                csv_file,
                args.repeats,
                strategy_names,
                not args.no_verify,
                args.max_verify_size_mb,
                set(args.query) if args.query else None,
                args.queries_dir,
                args.parity,
                args.max_events,
                args.cycles,
                args.cycle_gap,
                args.cycle_attr,
            )
            parity_checked += checked
            parity_mismatches += mismatches
    finally:
        csv_file.close()

    print(f"\nWrote rows to {output_path}", file=sys.stderr)
    if args.parity:
        print(
            f"Parity: {parity_checked} tree runs checked against the baseline, "
            f"{parity_mismatches} mismatch(es).",
            file=sys.stderr,
        )
    # ru_maxrss is in KiB on Linux.
    peak_mb = resource.getrusage(resource.RUSAGE_SELF).ru_maxrss / 1024
    print(f"Peak RSS: {peak_mb:.0f} MB", file=sys.stderr)


if __name__ == "__main__":
    main()
