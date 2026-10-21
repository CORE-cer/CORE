"""Analytical path-length model (backlog item 1, 2026-10-19 round).

Estimates L̄ (average number of minterm-tree decisions walked per event) for
each query from its tree dump plus the dataset's measured per-atom
selectivity, with no engine changes and no new sweep required for the core
result. Fits measured per-event delta (baseline - tree, from an existing
results CSV) against L̄ and against n (the atom count), to show which one
actually predicts the data.

Deliberately uses only the "guard-clean" queries (the 2026-10-19 follow-up
round's 22 queries: all 13 guard twins, rep_eq_*, worst_mix_*, floor_01),
never the plain originals - several of those have real output and their
processing time is contaminated by match-formatting cost (see
guides/thesis-findings/03-benchmarking-methodology.md), which would pollute
the fit for reasons unrelated to path length.

Usage:
    .venv/bin/python tests/benchmark/path_length_model.py \
        <tree_dump_log> <results_csv>

tree_dump_log: a quill log file produced by running every query in
queries_optimization/ once under CORE_PRINT_MINTERM_TREES=1 (see
dump_all_trees.py in this round's scratchpad for how it was produced -
loops the folder, builds each with MINTERM_TREE, calls get_output() to
flush the async logger).
"""

import bisect
import csv
import glob
import re
import statistics as st
import sys
from pathlib import Path

PROJECT_ROOT = Path(__file__).resolve().parent.parent.parent
SMART_HOMES = PROJECT_ROOT / "src/targets/experiments/smart_homes"
QUERIES_DIR = SMART_HOMES / "queries_optimization"

# Only these are used for the fit - see module docstring.
GUARD_CLEAN_PREFIXES = ("rep_eq_", "worst_mix_", "floor_01")


def is_guard_clean(name: str) -> bool:
    return name.endswith("_guard.txt") or name.startswith(GUARD_CLEAN_PREFIXES)


# ---- Tree-dump parsing -----------------------------------------------------


class Node:
    __slots__ = ("atom", "leaf", "true_child", "false_child")

    def __init__(self, atom: str | None = None, leaf: bool = False):
        self.atom = atom
        self.leaf = leaf
        self.true_child: "Node | None" = None
        self.false_child: "Node | None" = None


TRUE_CONN = "+-- true:  "
FALSE_CONN = "`-- false: "


def calibrate_offset(raw_lines: list[str]) -> int:
    """Finds the fixed column where quill's message content starts, using a
    line whose content (and therefore its leading whitespace) is known
    exactly: print_trees' "  [0] <predicate>" line always has 2 leading
    spaces before "[0]"."""
    for line in raw_lines:
        pos = line.find("[0] ")
        if pos != -1 and "CheckStreamTypePredicate" in line:
            return pos - 2
    raise ValueError("calibration line ('[0] CheckStreamTypePredicate') not found in log")


def extract_message_lines(raw_lines: list[str], offset: int) -> list[str]:
    return [line[offset:].rstrip("\n") if len(line) > offset else "" for line in raw_lines]


def parse_branch(lines: list[str], i: int, prefix: str, connector: str, continuation: str):
    full_prefix = prefix + connector
    line = lines[i]
    if not line.startswith(full_prefix):
        raise ValueError(f"expected prefix {full_prefix!r}, got {line!r}")
    rest = line[len(full_prefix) :]
    if rest.startswith("leaf #"):
        return Node(leaf=True), i + 1
    if not rest.endswith(" ?"):
        raise ValueError(f"expected an atom line ending in ' ?': {rest!r}")
    node = Node(atom=rest[:-2])
    new_prefix = prefix + continuation
    node.true_child, i = parse_branch(lines, i + 1, new_prefix, TRUE_CONN, "|   ")
    node.false_child, i = parse_branch(lines, i, new_prefix, FALSE_CONN, "    ")
    return node, i


def parse_tree(lines: list[str], start: int, indent: str = "  "):
    """Mirrors MintermTreeNode::print/print_branches/print_branch exactly -
    see tree.hpp. Root's own line has no connector; its children's lines use
    the SAME prefix as root (not indent + anything), and only lines below
    that add the 4-char continuation per level."""
    line = lines[start]
    if not line.startswith(indent):
        raise ValueError(f"expected root indent {indent!r}, got {line!r}")
    rest = line[len(indent) :]
    if rest.startswith("leaf #"):
        return Node(leaf=True), start + 1
    if not rest.endswith(" ?"):
        raise ValueError(f"expected the root atom line: {rest!r}")
    node = Node(atom=rest[:-2])
    i = start + 1
    node.true_child, i = parse_branch(lines, i, indent, TRUE_CONN, "|   ")
    node.false_child, i = parse_branch(lines, i, indent, FALSE_CONN, "    ")
    return node, i


SUMMARY_RE = re.compile(
    r"^Predicate evaluation: MintermTreeEvaluator: (\d+) predicates \((\d+) evaluated directly\)"
)


def extract_all_trees(
    log_path: Path, query_order: list[str]
) -> tuple[dict[str, Node], dict[str, int]]:
    """query_order must be the exact order queries were built in when the log
    was produced (sorted glob order, as dump_all_trees.py uses).

    Returns (trees, n_real) where n_real[name] is the query's real top-level
    predicate count (aliases) - NOT the distinct-atom count, which collapses
    textually-identical atoms (rep_eq_*'s whole point: n real, repeated
    predicates that the tree treats as one atom)."""
    raw_lines = log_path.read_text().splitlines(keepends=True)
    offset = calibrate_offset(raw_lines)
    msg_lines = extract_message_lines(raw_lines, offset)

    summaries = []
    for line in msg_lines:
        m = SUMMARY_RE.match(line)
        if m:
            summaries.append((int(m.group(1)), int(m.group(2))))
    if len(summaries) != len(query_order):
        raise ValueError(
            f"found {len(summaries)} summary lines but expected {len(query_order)} "
            f"(1 per query) - log/query-list mismatch"
        )
    n_real = {name: (total - direct) for name, (total, direct) in zip(query_order, summaries)}

    starts = [i for i, l in enumerate(msg_lines) if l.startswith("Event type 0:")]
    if len(starts) != len(query_order):
        raise ValueError(
            f"found {len(starts)} 'Event type 0:' blocks but expected "
            f"{len(query_order)} (one per query) - log/query-list mismatch"
        )
    trees = {}
    for name, start in zip(query_order, starts):
        root, _ = parse_tree(msg_lines, start + 1)
        trees[name] = root
    return trees, n_real


# ---- Selectivity ------------------------------------------------------------


class Selectivity:
    """Loads smart_homes_data.csv once; answers exact conditional
    probabilities for the three attributes this query family uses."""

    def __init__(self, csv_path: Path):
        from collections import Counter

        hh: Counter = Counter()
        plug: Counter = Counter()
        values: list[float] = []
        with open(csv_path, newline="") as f:
            for row in csv.DictReader(f):
                hh[int(row["household_id"])] += 1
                plug[int(row["plug_id"])] += 1
                values.append(float(row["value"]))
        self.n = len(values)
        self.household_counts = hh
        self.plug_counts = plug
        self.sorted_values = sorted(values)

    def count_value_in(self, lo: float, hi: float) -> int:
        """Count of events with lo < value <= hi (hi may be +inf)."""
        left = bisect.bisect_right(self.sorted_values, lo)
        right = (
            len(self.sorted_values)
            if hi == float("inf")
            else bisect.bisect_right(self.sorted_values, hi)
        )
        return max(0, right - left)


ATOM_RE = re.compile(r"^(\w+)(==|!=|>=|<=|>|<)(-?\d+(?:\.\d+)?)$")


def parse_atom(text: str):
    m = ATOM_RE.match(text)
    if not m:
        raise ValueError(f"unrecognized atom text: {text!r}")
    attr, op, value = m.groups()
    return attr, op, value


def expected_depth(node: Node, sel: Selectivity, discrete_state: dict, lo: float, hi: float) -> float:
    """E[number of further decisions walked | reached this node], given the
    attribute constraints accumulated on the path so far. Cross-attribute
    independence assumed (see guides/thesis-findings/05); within an
    attribute, exact conditional probabilities from the measured marginals."""
    if node.leaf:
        return 0.0

    attr, op, value_text = parse_atom(node.atom)

    if attr == "value":
        t = float(value_text)
        remaining = sel.count_value_in(lo, hi)
        if op == ">":
            # true: value in (max(lo,t), hi]; false: value in (lo, min(hi,t)]
            true_bounds = (max(lo, t), hi)
            false_bounds = (lo, min(hi, t))
        elif op == "<":
            # true: value < t, i.e. value in (lo, min(hi,t)); false: (max(lo,t), hi]
            # (count_value_in treats bounds as (lo,hi], close enough here since
            # value is a continuous double - no event sits exactly on t almost surely,
            # and the guard predicates use deliberately impossible values anyway).
            true_bounds = (lo, min(hi, t))
            false_bounds = (max(lo, t), hi)
        else:
            raise ValueError(f"unexpected operator on value: {op}")
        if remaining == 0:
            p_true = 0.0
        else:
            true_count = sel.count_value_in(*true_bounds)
            p_true = true_count / remaining
        true_depth = expected_depth(node.true_child, sel, discrete_state, *true_bounds)
        false_depth = expected_depth(node.false_child, sel, discrete_state, *false_bounds)
    else:
        assert op == "==", f"only == equality expected on {attr}, got {op}"
        k = int(value_text)
        counts = sel.household_counts if attr == "household_id" else sel.plug_counts
        excluded = discrete_state.get(attr, frozenset())
        remaining = sel.n - sum(counts.get(v, 0) for v in excluded)
        p_true = (counts.get(k, 0) / remaining) if remaining > 0 else 0.0
        true_state = discrete_state  # no further same-attribute test expected after a true branch
        false_state = {**discrete_state, attr: excluded | {k}}
        true_depth = expected_depth(node.true_child, sel, true_state, lo, hi)
        false_depth = expected_depth(node.false_child, sel, false_state, lo, hi)

    return 1.0 + p_true * true_depth + (1.0 - p_true) * false_depth


def l_bar(root: Node, sel: Selectivity) -> float:
    return expected_depth(root, sel, {}, float("-inf"), float("inf"))


# ---- CSV join + regression --------------------------------------------------


def load_deltas(csv_path: Path) -> dict[str, tuple[float, int]]:
    """query -> (delta_per_event_us, event_count), strategy=minterm_tree vs baseline."""
    rows = list(csv.DictReader(open(csv_path, newline="")))
    by_query: dict[str, dict[str, list[float]]] = {}
    event_counts: dict[str, int] = {}
    for r in rows:
        by_query.setdefault(r["query"], {}).setdefault(r["strategy"], []).append(
            float(r["processing_time_s"])
        )
        event_counts[r["query"]] = int(r["event_count"])
    out = {}
    for query, strategies in by_query.items():
        if "baseline" not in strategies or "minterm_tree" not in strategies:
            continue
        baseline = st.mean(strategies["baseline"])
        tree = st.mean(strategies["minterm_tree"])
        n_events = event_counts[query]
        delta_us = (baseline - tree) / n_events * 1e6
        out[query] = (delta_us, n_events)
    return out


def two_var_linreg(
    x1s: list[float], x2s: list[float], ys: list[float]
) -> tuple[float, float, float, float]:
    """OLS fit of y = a*x1 + b*x2 + c, via the normal equations (3x3, solved
    directly - no numpy dependency). Returns (a, b, c, r_squared)."""
    n = len(ys)
    sx1, sx2, sy = sum(x1s), sum(x2s), sum(ys)
    sx1x1 = sum(x * x for x in x1s)
    sx2x2 = sum(x * x for x in x2s)
    sx1x2 = sum(a * b for a, b in zip(x1s, x2s))
    sx1y = sum(a * b for a, b in zip(x1s, ys))
    sx2y = sum(a * b for a, b in zip(x2s, ys))

    # Normal equations for [a, b, c]:
    #   [sx1x1 sx1x2 sx1] [a]   [sx1y]
    #   [sx1x2 sx2x2 sx2] [b] = [sx2y]
    #   [sx1   sx2   n  ] [c]   [sy  ]
    A = [[sx1x1, sx1x2, sx1], [sx1x2, sx2x2, sx2], [sx1, sx2, n]]
    b_vec = [sx1y, sx2y, sy]

    def solve_3x3(A, b):
        import copy

        A = copy.deepcopy(A)
        b = list(b)
        for i in range(3):
            pivot = A[i][i]
            for k in range(i, 3):
                A[i][k] /= pivot
            b[i] /= pivot
            for j in range(3):
                if j != i:
                    factor = A[j][i]
                    for k in range(i, 3):
                        A[j][k] -= factor * A[i][k]
                    b[j] -= factor * b[i]
        return b

    a, b, c = solve_3x3(A, b_vec)
    mean_y = sy / n
    predicted = [a * x1 + b * x2 + c for x1, x2 in zip(x1s, x2s)]
    ss_res = sum((y - p) ** 2 for y, p in zip(ys, predicted))
    ss_tot = sum((y - mean_y) ** 2 for y in ys)
    r2 = 1 - ss_res / ss_tot if ss_tot > 0 else 1.0
    return a, b, c, r2


def simple_linreg(xs: list[float], ys: list[float]) -> tuple[float, float, float]:
    """Returns (slope, intercept, r_squared)."""
    n = len(xs)
    mean_x, mean_y = st.mean(xs), st.mean(ys)
    cov = sum((x - mean_x) * (y - mean_y) for x, y in zip(xs, ys))
    var_x = sum((x - mean_x) ** 2 for x in xs)
    slope = cov / var_x if var_x > 0 else 0.0
    intercept = mean_y - slope * mean_x
    predicted = [slope * x + intercept for x in xs]
    ss_res = sum((y - p) ** 2 for y, p in zip(ys, predicted))
    ss_tot = sum((y - mean_y) ** 2 for y in ys)
    r2 = 1 - ss_res / ss_tot if ss_tot > 0 else 1.0
    return slope, intercept, r2


def main():
    if len(sys.argv) != 3:
        print(f"usage: {sys.argv[0]} <tree_dump_log> <results_csv>", file=sys.stderr)
        sys.exit(1)
    log_path, csv_path = Path(sys.argv[1]), Path(sys.argv[2])

    query_files = sorted(glob.glob(str(QUERIES_DIR / "*.txt")))
    query_order = [Path(f).name for f in query_files]
    trees, n_real = extract_all_trees(log_path, query_order)

    sel = Selectivity(SMART_HOMES / "smart_homes_data.csv")
    deltas = load_deltas(csv_path)

    print(f"{'query':30s} {'n':>3s} {'L̄':>8s} {'delta(us)':>10s}")
    rows = []
    for name in query_order:
        if not is_guard_clean(name) or name not in deltas:
            continue
        root = trees[name]
        n = n_real[name]
        lbar = l_bar(root, sel)
        delta_us, _ = deltas[name]
        rows.append((name, n, lbar, delta_us))
        print(f"{name:30s} {n:3d} {lbar:8.3f} {delta_us:10.4f}")

    ns = [r[1] for r in rows]
    lbars = [r[2] for r in rows]
    deltas_us = [r[3] for r in rows]

    print(f"\n{len(rows)} queries used (guard-clean set only)")
    slope_l, intercept_l, r2_l = simple_linreg(lbars, deltas_us)
    slope_n, intercept_n, r2_n = simple_linreg(ns, deltas_us)
    print(f"\ndelta ~ L̄ alone:  slope={slope_l:.4f}  intercept={intercept_l:.4f}  R²={r2_l:.4f}")
    print(f"delta ~ n alone:   slope={slope_n:.4f}  intercept={intercept_n:.4f}  R²={r2_n:.4f}")
    print(
        "(neither alone is the right test of the model - it predicts delta from BOTH "
        "together: delta ~= n*c_predicate - L̄*c_node, not delta ~= f(L̄) alone)"
    )

    # The actual model: delta = a*n + b*L̄ + c, where a estimates c_predicate
    # and -b estimates c_node (b should come out negative: more walking
    # subtracts from the saving).
    a, b, c, r2_model = two_var_linreg(ns, lbars, deltas_us)
    print(
        f"\ndelta ~ n*c_predicate - L̄*c_node (2-variable fit):"
        f"\n  c_predicate estimate = {a:.4f} us/predicate"
        f"\n  c_node estimate      = {-b:.4f} us/node"
        f"\n  intercept            = {c:.4f}"
        f"\n  R²                   = {r2_model:.4f}"
    )

    # A simpler one-variable reduction, valid if c_predicate ~= c_node: delta
    # should then track (n - L̄) directly.
    n_minus_l = [n - l for n, l in zip(ns, lbars)]
    slope_nl, intercept_nl, r2_nl = simple_linreg(n_minus_l, deltas_us)
    print(
        f"\ndelta ~ (n - L̄), the c_predicate≈c_node simplification:"
        f"\n  slope={slope_nl:.4f}  intercept={intercept_nl:.4f}  R²={r2_nl:.4f}"
    )

    # rep_eq closed-form sanity check: L=1 for every n, so delta should fit
    # (n-1) * c_predicate with the intercept pinned near 0.
    rep_rows = [r for r in rows if r[0].startswith("rep_eq_")]
    if rep_rows:
        xs = [r[1] - 1 for r in rep_rows]  # (n - 1)
        ys = [r[3] for r in rep_rows]
        slope_c, intercept_c, r2_c = simple_linreg(xs, ys)
        print(
            f"\nrep_eq sanity check (L=1 for all n): delta ~ (n-1) * c_predicate"
            f"\n  c_predicate estimate = {slope_c:.4f} us/predicate "
            f"(intercept {intercept_c:.4f}, R²={r2_c:.4f})"
        )


if __name__ == "__main__":
    main()
