"""Summarizes a compile_vs_processing.py results CSV as ratio + per-event
delta (the metric convention agreed with the supervisor, 2026-10-19), with
compile time shown for reference but kept out of the headline numbers.

Compile time is a one-time cost per query; processing time is paid once per
event, for as long as the query runs. Since queries run indefinitely, the
headline "is this worth it" number is the per-event numbers below, not
compile time - compile is printed for comparison/sanity (e.g. to notice a
tree that silently fell back to direct evaluation) but never folded into the
ratio or delta.

Usage:
    .venv/bin/python tests/benchmark/summarize_results.py <results.csv> [...]

Multiple CSVs can be given; rows are pooled before summarizing (handy for a
sweep split across several invocations, as the best/worst-case runs are).
"""

import csv
import statistics as st
import sys
from collections import defaultdict
from pathlib import Path

BASELINE = "baseline"


def load_rows(paths: list[str]) -> list[dict]:
    rows: list[dict] = []
    for path in paths:
        with open(path, newline="") as f:
            rows.extend(csv.DictReader(f))
    return rows


def summarize(rows: list[dict]) -> None:
    # Group by (dataset, query) -> strategy -> list of rows, preserving the
    # order queries first appear in so the printed report follows the CSV.
    groups: dict[tuple[str, str], dict[str, list[dict]]] = defaultdict(
        lambda: defaultdict(list)
    )
    order: list[tuple[str, str]] = []
    for row in rows:
        key = (row["dataset"], row["query"])
        if key not in groups:
            order.append(key)
        groups[key][row["strategy"]].append(row)

    best_overall: tuple[float, str, str] | None = None  # (ratio, query, strategy)
    worst_overall: tuple[float, str, str] | None = None

    for key in order:
        dataset, query = key
        by_strategy = groups[key]
        if BASELINE not in by_strategy:
            print(f"\n{query}: no baseline rows, skipping", file=sys.stderr)
            continue

        event_count = int(by_strategy[BASELINE][0]["event_count"])
        mean_proc = {
            s: st.mean(float(r["processing_time_s"]) for r in rs)
            for s, rs in by_strategy.items()
        }
        mean_compile = {
            s: st.mean(float(r["compile_time_s"]) for r in rs)
            for s, rs in by_strategy.items()
        }
        baseline_proc = mean_proc[BASELINE]

        print(f"\n=== {dataset} / {query} (n={event_count:,} events) ===")

        # Compile time: reference only, every strategy, never used below.
        compile_cells = ", ".join(
            f"{s}={mean_compile[s] * 1000:.2f}ms" for s in sorted(mean_compile)
        )
        print(f"  compile (reference, not part of the speedup metric): {compile_cells}")

        # Headline: ratio and per-event delta, processing time only.
        best_ratio = None
        for strategy in sorted(s for s in by_strategy if s != BASELINE):
            tree_proc = mean_proc[strategy]
            ratio = baseline_proc / tree_proc if tree_proc > 0 else float("inf")
            delta_us = (baseline_proc - tree_proc) / event_count * 1e6
            marker = ""
            if best_ratio is None or ratio > best_ratio[0]:
                best_ratio = (ratio, strategy)
            print(f"  {strategy:30s} {ratio:6.2f}x   {delta_us:+8.4f} us/event")

            if best_overall is None or ratio > best_overall[0]:
                best_overall = (ratio, query, strategy)
            if worst_overall is None or ratio < worst_overall[0]:
                worst_overall = (ratio, query, strategy)

        if best_ratio is not None:
            print(f"  best: {best_ratio[1]} ({best_ratio[0]:.2f}x)")

    print("\n" + "=" * 60)
    if best_overall:
        print(f"Best observed ratio:  {best_overall[0]:.2f}x  ({best_overall[1]}, {best_overall[2]})")
    if worst_overall:
        print(f"Worst observed ratio: {worst_overall[0]:.2f}x  ({worst_overall[1]}, {worst_overall[2]})")


def main() -> None:
    if len(sys.argv) < 2:
        print(f"usage: {sys.argv[0]} <results.csv> [more.csv ...]", file=sys.stderr)
        sys.exit(1)
    paths = sys.argv[1:]
    for path in paths:
        if not Path(path).exists():
            print(f"not found: {path}", file=sys.stderr)
            sys.exit(1)
    summarize(load_rows(paths))


if __name__ == "__main__":
    main()
