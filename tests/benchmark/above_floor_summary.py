"""Summarizes above-floor processing time for a compile_vs_processing.py
results CSV: subtracts a per-strategy floor measured in the SAME CSV/session
(never pooled across files - mixing sessions produced nonsense numbers once
already, see guides/thesis-findings/06-open-questions-and-limits.md), and
reports the above-floor delta as the headline number, plus an above-floor
ratio where it's numerically stable enough to mean anything.

Why delta, not ratio, is the headline: the above-floor ratio divides two
above-floor costs that can both be noise-sized (smaller than the floor's own
cross-strategy spread), which produces meaningless ratios - checked directly
against the rep_eq/rep_eq_single family, where it ranged from -623x to
+396x depending on which strategy and repeat's noise happened to land where.
The ratio is only printed once both above-floor magnitudes clear that
spread; otherwise the delta is still printed (nothing is hidden), flagged as
not a reliable ratio.

Usage:
    .venv/bin/python tests/benchmark/above_floor_summary.py <results.csv> [...] \
        [--floor-query floor_01.txt] [--dataset smart_homes]

Unlike summarize_results.py, CSVs are NOT pooled together: the floor and the
families it's subtracted from must come from the same file/session, so each
input CSV is processed independently, even when several are given at once.
"""

import argparse
import csv
import statistics as st
import sys
from collections import defaultdict
from pathlib import Path

BASELINE = "baseline"


def load_rows(path: str) -> list[dict]:
    with open(path, newline="") as f:
        return list(csv.DictReader(f))


def group_by_query(
    rows: list[dict], dataset: str
) -> tuple[list[str], dict[str, dict[str, list[dict]]]]:
    groups: dict[str, dict[str, list[dict]]] = defaultdict(lambda: defaultdict(list))
    order: list[str] = []
    for row in rows:
        if row["dataset"] != dataset:
            continue
        query = row["query"]
        if query not in groups:
            order.append(query)
        groups[query][row["strategy"]].append(row)
    return order, groups


def mean_proc(by_strategy: dict[str, list[dict]]) -> dict[str, float]:
    return {
        s: st.mean(float(r["processing_time_s"]) for r in rs)
        for s, rs in by_strategy.items()
    }


def summarize_file(path: str, floor_query: str, dataset: str) -> None:
    rows = load_rows(path)
    order, groups = group_by_query(rows, dataset)

    if floor_query not in groups:
        print(
            f"\n{path}: no '{floor_query}' row for dataset '{dataset}' - skipping file "
            "(above-floor numbers require a floor measured in this same session)",
            file=sys.stderr,
        )
        return

    floor_proc = mean_proc(groups[floor_query])
    if BASELINE not in floor_proc:
        print(f"\n{path}: floor query has no baseline row - skipping", file=sys.stderr)
        return

    floor_spread = max(floor_proc.values()) - min(floor_proc.values())
    print(f"\n### {path}")
    print(
        "  floor ("
        + floor_query
        + "): "
        + ", ".join(f"{s}={v:.4f}s" for s, v in sorted(floor_proc.items()))
    )
    print(
        f"  cross-strategy floor spread: {floor_spread:.4f}s "
        f"({floor_spread / floor_proc[BASELINE] * 100:.1f}% of baseline's floor) "
        "- an above-floor value smaller than this is noise-sized, not a reliable signal"
    )

    for query in order:
        if query == floor_query:
            continue
        by_strategy = groups[query]
        if BASELINE not in by_strategy:
            print(f"\n  {query}: no baseline rows, skipping", file=sys.stderr)
            continue

        event_count = int(by_strategy[BASELINE][0]["event_count"])
        proc = mean_proc(by_strategy)
        baseline_proc = proc[BASELINE]
        baseline_above = baseline_proc - floor_proc[BASELINE]

        print(f"\n  === {query} (n={event_count:,} events) ===")
        print(f"    baseline: proc={baseline_proc:.4f}s  above_floor={baseline_above:+.4f}s")

        for strategy in sorted(s for s in by_strategy if s != BASELINE):
            if strategy not in floor_proc:
                print(f"    {strategy}: no floor measurement for this strategy, skipping")
                continue

            tree_proc = proc[strategy]
            tree_above = tree_proc - floor_proc[strategy]

            raw_ratio = baseline_proc / tree_proc if tree_proc > 0 else float("inf")
            raw_delta_us = (baseline_proc - tree_proc) / event_count * 1e6
            above_delta_us = (baseline_above - tree_above) / event_count * 1e6

            stable = abs(baseline_above) > floor_spread and abs(tree_above) > floor_spread
            if stable:
                above_ratio = baseline_above / tree_above if tree_above != 0 else float("inf")
                ratio_str = f"{above_ratio:6.2f}x"
            else:
                ratio_str = "   (near-floor, not a reliable ratio)"

            print(
                f"    {strategy:30s} raw={raw_ratio:5.2f}x  delta={raw_delta_us:+7.4f}us/evt   "
                f"above_floor={tree_above:+.4f}s  above_delta={above_delta_us:+7.4f}us/evt   "
                f"above_ratio={ratio_str}"
            )


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("csv", nargs="+", help="one or more compile_vs_processing.py result CSVs")
    parser.add_argument("--floor-query", default="floor_01.txt")
    parser.add_argument("--dataset", default="smart_homes")
    args = parser.parse_args()

    for path in args.csv:
        if not Path(path).exists():
            print(f"not found: {path}", file=sys.stderr)
            sys.exit(1)

    for path in args.csv:
        summarize_file(path, args.floor_query, args.dataset)


if __name__ == "__main__":
    main()
