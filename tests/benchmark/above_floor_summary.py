"""Summarizes above-floor processing time for a compile_vs_processing.py
results CSV: subtracts a per-strategy floor measured in the SAME CSV/session
(never pooled across files - mixing sessions produced nonsense numbers once
already, see guides/thesis-findings/06-open-questions-and-limits.md), and
reports the above-floor delta as the headline number, plus an above-floor
ratio where it's numerically stable enough to mean anything.

Why delta, not ratio, is the headline: the above-floor ratio divides two
above-floor costs that can both be noise-sized, which produces meaningless
ratios - checked directly against the rep_eq/rep_eq_single family, where it
ranged from -623x to +396x depending on which strategy and repeat's noise
happened to land where.

How "noise-sized" is decided (2026-10-29, revised): earlier this compared
each above-floor value's magnitude against the floor's cross-strategy
spread, a proxy. Checked against the raw per-repeat data, the real cause is
plainer: with 5 repeats, a single repeat running 5-15% slower than its
siblings - ordinary scheduler jitter, seen in nearly every (query, strategy)
block, not concentrated in one strategy or time window - can shift a mean by
more than the real effect size for a small-n query. Comparing against the
floor's spread caught some of this but not reliably: recomputing with the
median instead of the mean flipped the sign or the stability call on 14 of
about 124 (query, strategy) combinations in one sweep alone.

The fix here is a direct significance test instead of a proxy: every mean is
reported with its own standard error (from that specific measurement's
repeat-to-repeat variance, propagated through the floor subtraction and the
baseline-vs-tree difference), and a value only counts as distinguishable
from zero when it clears twice that standard error (~95% CI excluding zero).
The above-floor delta is always printed with its CI, reliable or not; the
above-floor ratio is only printed when both the baseline's and the tree's
above-floor cost individually clear this test, since a ratio of two
unreliable numbers is worse than no ratio at all.

A second, different failure mode (2026-10-29): raising the repeat count from
5 to 15 to shrink those standard errors instead tripled the sweep's wall
time, giving a rare contamination event (something external competing for
CPU for 15-20+ seconds) room to land inside one strategy's floor
measurement, which runs as a single uninterrupted block of repeats. This
isn't ordinary jitter and the significance test above does not catch it:
checked against the raw per-repeat values, `minterm_tree_ordered`'s floor
was elevated across *all 15* of its repeats (no clean sub-cluster left to
contrast against within its own data), while `minterm_tree_interned`'s was
elevated for its first 8 repeats then dropped back to normal for the last 7.
A within-strategy changepoint test only catches the second shape. The first
shape is only visible by comparing against a strategy that stayed clean in
the same sweep - here, baseline, whose floor CV was under 1.5% in every
sweep tried so far. So every strategy's floor is also checked against
baseline's in the same file: a deviation beyond `--contamination-threshold`
(default 6%, chosen to sit well above the ~2% spread seen in clean sweeps
and well below the 20%+ seen in the contaminated one) means that strategy's
floor is unusable for this file, and above-floor numbers for it are skipped
with a warning rather than silently computed from a broken floor.

Also reports throughput (events/sec) for baseline and each tree strategy -
the reciprocal of the same processing_time_s already measured, no new data
needed, but the framing a 2026-11-09 meeting note asked for specifically.

Usage:
    .venv/bin/python tests/benchmark/above_floor_summary.py <results.csv> [...] \
        [--floor-query floor_01.txt] [--dataset smart_homes] [--z 2.0]

Unlike summarize_results.py, CSVs are NOT pooled together: the floor and the
families it's subtracted from must come from the same file/session, so each
input CSV is processed independently, even when several are given at once.
"""

import argparse
import csv
import math
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


def mean_sem(by_strategy: dict[str, list[dict]]) -> dict[str, tuple[float, float]]:
    """Per strategy: (mean processing time, standard error of that mean)."""
    out = {}
    for s, rs in by_strategy.items():
        vals = [float(r["processing_time_s"]) for r in rs]
        mean = st.mean(vals)
        sem = st.stdev(vals) / math.sqrt(len(vals)) if len(vals) > 1 else float("inf")
        out[s] = (mean, sem)
    return out


def contaminated_strategies(
    by_strategy: dict[str, list[dict]], threshold: float
) -> dict[str, float]:
    """Strategies whose floor mean deviates from baseline's by more than
    `threshold` (a fraction, e.g. 0.06) in this same file. A real contamination
    event can elevate every repeat in a strategy's block uniformly, leaving no
    internal changepoint to detect - the only tell is disagreement with a
    strategy that stayed clean in the same sweep. Returns {strategy: deviation}."""
    base_vals = [float(r["processing_time_s"]) for r in by_strategy.get(BASELINE, [])]
    if not base_vals:
        return {}
    base_mean = st.mean(base_vals)
    bad = {}
    for s, rs in by_strategy.items():
        if s == BASELINE:
            continue
        mean = st.mean(float(r["processing_time_s"]) for r in rs)
        deviation = abs(mean - base_mean) / base_mean
        if deviation > threshold:
            bad[s] = deviation
    return bad


def summarize_file(
    path: str, floor_query: str, dataset: str, z: float, contamination_threshold: float
) -> None:
    rows = load_rows(path)
    order, groups = group_by_query(rows, dataset)

    if floor_query not in groups:
        print(
            f"\n{path}: no '{floor_query}' row for dataset '{dataset}' - skipping file "
            "(above-floor numbers require a floor measured in this same session)",
            file=sys.stderr,
        )
        return

    floor = mean_sem(groups[floor_query])
    if BASELINE not in floor:
        print(f"\n{path}: floor query has no baseline row - skipping", file=sys.stderr)
        return

    bad_floor = contaminated_strategies(groups[floor_query], contamination_threshold)

    print(f"\n### {path}")
    print(
        "  floor ("
        + floor_query
        + "): "
        + ", ".join(f"{s}={m:.4f}s (SEM {e:.4f}s)" for s, (m, e) in sorted(floor.items()))
    )
    for s, deviation in sorted(bad_floor.items()):
        print(
            f"  WARNING: {s}'s floor deviates {deviation * 100:.1f}% from baseline's in this "
            f"file (threshold {contamination_threshold * 100:.0f}%) - treating it as "
            "contaminated (see the module docstring); above-floor numbers for this "
            "strategy are skipped below, not computed from a broken floor.",
            file=sys.stderr,
        )

    for query in order:
        if query == floor_query:
            continue
        by_strategy = groups[query]
        if BASELINE not in by_strategy:
            print(f"\n  {query}: no baseline rows, skipping", file=sys.stderr)
            continue

        event_count = int(by_strategy[BASELINE][0]["event_count"])
        proc = mean_sem(by_strategy)
        base_mean, base_sem = proc[BASELINE]
        floor_base_mean, floor_base_sem = floor[BASELINE]
        base_above_mean = base_mean - floor_base_mean
        base_above_sem = math.hypot(base_sem, floor_base_sem)

        base_throughput = event_count / base_mean if base_mean > 0 else float("inf")
        print(f"\n  === {query} (n={event_count:,} events) ===")
        print(
            f"    baseline: proc={base_mean:.4f}s  above_floor={base_above_mean:+.4f}s (SEM {base_above_sem:.4f}s)  "
            f"throughput={base_throughput:,.0f} events/s"
        )

        for strategy in sorted(s for s in by_strategy if s != BASELINE):
            if strategy not in floor:
                print(f"    {strategy}: no floor measurement for this strategy, skipping")
                continue
            if strategy in bad_floor:
                print(f"    {strategy:30s} skipped - contaminated floor in this file (see warning above)")
                continue

            tree_mean, tree_sem = proc[strategy]
            floor_tree_mean, floor_tree_sem = floor[strategy]
            tree_above_mean = tree_mean - floor_tree_mean
            tree_above_sem = math.hypot(tree_sem, floor_tree_sem)

            raw_ratio = base_mean / tree_mean if tree_mean > 0 else float("inf")
            raw_delta_us = (base_mean - tree_mean) / event_count * 1e6
            tree_throughput = event_count / tree_mean if tree_mean > 0 else float("inf")

            delta_mean_us = (base_above_mean - tree_above_mean) / event_count * 1e6
            delta_sem_us = math.hypot(base_above_sem, tree_above_sem) / event_count * 1e6
            delta_reliable = abs(delta_mean_us) > z * delta_sem_us

            ratio_reliable = (
                base_above_mean > z * base_above_sem
                and tree_above_mean > z * tree_above_sem
            )
            above_ratio = base_above_mean / tree_above_mean if tree_above_mean != 0 else None

            # Two different reasons a ratio can fail the reliability check,
            # worth telling apart rather than lumping into one message: if
            # the delta itself isn't reliable either, there's genuinely not
            # enough signal yet ("too small"); if the delta IS reliable but
            # the ratio still isn't, the effect is real but one side (almost
            # always the tree's, since that's the side the optimization is
            # trying to shrink toward zero) is too close to its own floor to
            # divide by soundly ("not reliable") - checked directly: this is
            # exactly the rep_eq_single case that printed -37x before the
            # sign check was added.
            if ratio_reliable:
                ratio_str = f"{above_ratio:6.2f}x"
                ratio_category = "reliable"
            elif not delta_reliable:
                shown = f"{above_ratio:.2f}x" if above_ratio is not None else "n/a"
                ratio_str = f"({shown}, too small to trust)"
                ratio_category = "too_small"
            else:
                shown = f"{above_ratio:.2f}x" if above_ratio is not None else "n/a"
                ratio_str = f"({shown}, not reliable)"
                ratio_category = "not_reliable"

            flag = "" if delta_reliable else "  [delta within noise of zero]"
            print(
                f"    {strategy:30s} raw={raw_ratio:5.2f}x  delta={raw_delta_us:+7.4f}us/evt   "
                f"above_delta={delta_mean_us:+7.4f} ± {z * delta_sem_us:.4f} us/evt (95% CI)   "
                f"above_ratio={ratio_str}{flag}\n"
                f"{'':34s}throughput={tree_throughput:,.0f} events/s "
                f"(baseline {base_throughput:,.0f} events/s)"
            )


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("csv", nargs="+", help="one or more compile_vs_processing.py result CSVs")
    parser.add_argument("--floor-query", default="floor_01.txt")
    parser.add_argument("--dataset", default="smart_homes")
    parser.add_argument(
        "--z",
        type=float,
        default=2.0,
        help="how many standard errors a value must clear to count as distinguishable "
        "from zero (default 2.0, ~95%% CI, assuming the repeat-to-repeat noise is "
        "roughly normal - not verified distributionally, just the standard convention)",
    )
    parser.add_argument(
        "--contamination-threshold",
        type=float,
        default=0.06,
        help="flag a strategy's floor as contaminated when it deviates from baseline's "
        "floor (same file) by more than this fraction (default 0.06 - clean sweeps seen "
        "so far stayed under 0.02, a contaminated one hit 0.23-0.43)",
    )
    args = parser.parse_args()

    for path in args.csv:
        if not Path(path).exists():
            print(f"not found: {path}", file=sys.stderr)
            sys.exit(1)

    for path in args.csv:
        summarize_file(path, args.floor_query, args.dataset, args.z, args.contamination_threshold)


if __name__ == "__main__":
    main()
