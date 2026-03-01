"""Benchmark queries using PyOfflineServer (in-process C++ engine).

Reuses dataset configuration from the e2e test suite and the CSV parser.
Timing wraps only C++ engine work (send_events/send_event + get_output),
excluding Python CSV parsing and setup overhead.

Usage:
    uv run python tests/benchmark/benchmark_queries.py [--dataset NAME] [--repeats N]

Examples:
    uv run python tests/benchmark/benchmark_queries.py
    uv run python tests/benchmark/benchmark_queries.py --dataset stocks
    uv run python tests/benchmark/benchmark_queries.py --dataset stocks --repeats 5
"""

import argparse
import shutil
import sys
import tarfile
import time
from pathlib import Path

# Ensure project root is on sys.path so `tests.*` imports work when
# running as a standalone script (e.g. `uv run python tests/benchmark/...`).
_PROJECT_ROOT = str(Path(__file__).resolve().parent.parent.parent)
if _PROJECT_ROOT not in sys.path:
    sys.path.insert(0, _PROJECT_ROOT)

import pycer

from tests.e2e.conftest import DATASETS, _version_sort_key
from tests.e2e.csv_parser import parse_csv


def extract_archives(dataset: dict) -> None:
    base = dataset["base"]
    # Extract expected results
    expected_dir = base / "expected_results"
    archive = base / "expected_results.tar.xz"
    if not expected_dir.exists() and archive.exists():
        with tarfile.open(archive) as tf:
            tf.extractall(path=base, filter="data")
    # Extract CSV archive if needed
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


def discover_queries(dataset: dict) -> list[tuple[Path, Path]]:
    base = dataset["base"]
    queries_dir = base / "queries"
    expected_dir = base / "expected_results"

    if not queries_dir.exists():
        return []

    cases = []
    for query_file in sorted(queries_dir.iterdir(), key=_version_sort_key):
        if query_file.name in dataset.get("exclude", []):
            continue
        expected_file = expected_dir / query_file.name
        cases.append((query_file, expected_file))
    return cases


def benchmark_dataset(dataset: dict, repeats: int) -> list[tuple[str, float]]:
    extract_archives(dataset)

    base = dataset["base"]
    declaration_text = (base / dataset["declaration"]).read_text()
    options_text = None
    if dataset["options"]:
        options_text = (base / dataset["options"]).read_text()

    # Pre-parse CSV once (outside timing)
    tmp_server = pycer.PyOfflineServer()
    stream_info = tmp_server.declare_stream(declaration_text)
    csv_path = base / dataset["csv"]
    events, delays_ns = parse_csv(csv_path, stream_info)
    has_timing = any(d > 0 for d in delays_ns)
    del tmp_server

    queries = discover_queries(dataset)
    if not queries:
        print(f"  No queries found for {dataset['name']}", file=sys.stderr)
        return []

    results = []

    for query_file, expected_file in queries:
        query_text = query_file.read_text()
        query_name = query_file.name
        print(f"  Benchmarking {query_name}...", file=sys.stderr)

        times = []
        for run in range(1, repeats + 1):
            server = pycer.PyOfflineServer()
            server.declare_stream(declaration_text)
            if options_text:
                server.declare_option(options_text)
            server.add_query(query_text)

            # --- Timed section: only C++ engine work ---
            start = time.perf_counter()
            if has_timing:
                for event, delay_ns in zip(events, delays_ns):
                    if delay_ns > 0:
                        time.sleep(delay_ns / 1e9)
                    server.send_event(event)
            else:
                server.send_events(events)
            output = server.get_output()
            elapsed = time.perf_counter() - start
            # --- End timed section ---

            times.append(elapsed)

            # Validate output against expected on first run
            if run == 1 and expected_file.exists():
                header = f"Query: {query_text}\nRead events {len(events)}\n"
                actual = header + output
                expected = expected_file.read_text()
                if actual != expected:
                    print(
                        f"  ERROR: Output mismatch for {query_name}",
                        file=sys.stderr,
                    )
                    sys.exit(1)

            print(f"    Run {run}/{repeats}: {elapsed:.4f}s", file=sys.stderr)

        avg = sum(times) / len(times)
        print(f"    Average: {avg:.4f}s", file=sys.stderr)
        results.append((query_name, avg))

    cleanup_archives(dataset)
    return results


def write_csv(results: list[tuple[str, float]], output_path: Path) -> None:
    output_path.parent.mkdir(parents=True, exist_ok=True)
    with open(output_path, "w") as f:
        f.write("query,time\n")
        for query_name, avg_time in results:
            f.write(f"{query_name},{avg_time:.6f}\n")


def main() -> None:
    parser = argparse.ArgumentParser(description="Benchmark CORE queries")
    parser.add_argument(
        "--dataset",
        type=str,
        default=None,
        help="Run only this dataset (e.g., stocks, taxis). Omit to run all.",
    )
    parser.add_argument(
        "--repeats",
        type=int,
        default=3,
        help="Number of times to run each query (default: 3)",
    )
    args = parser.parse_args()

    datasets = DATASETS
    if args.dataset:
        datasets = [d for d in DATASETS if d["name"] == args.dataset]
        if not datasets:
            valid = ", ".join(d["name"] for d in DATASETS)
            print(f"Unknown dataset '{args.dataset}'. Valid: {valid}", file=sys.stderr)
            sys.exit(1)

    for dataset in datasets:
        name = dataset["name"]
        print(f"\n=== Benchmarking {name} ===", file=sys.stderr)
        results = benchmark_dataset(dataset, args.repeats)
        if results:
            output_path = dataset["base"] / "benchmark_Release.csv"
            write_csv(results, output_path)
            print(f"Results saved to {output_path}", file=sys.stderr)

    print("\nBenchmark complete.", file=sys.stderr)


if __name__ == "__main__":
    main()
