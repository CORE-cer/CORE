"""Compare the three timing methods on the same queries (backlog item 23).

Reads the S0 stage of a `run_overnight.sh` results directory, where the same
queries were measured three ways with `core_bench` on the same Release build and
the same two-core pinning:

  s0_item23_chrono/  plain chrono (`--method plain`): whole-loop wall clock, no
                     Tracy. The ground truth for totals and speedups.
  s0_item23_split/   copy-and-subtract (`--method split`): each event's
                     predicates are evaluated on a copy first (timed), then the
                     event is processed (timed); predicate time = the copy's
                     time, the rest = process - predicate.
  s0_item23_tracy/   Tracy coarse zones, converted by tracy_to_csv.py with the
                     per-zone overhead fitted against the plain runs
                     (`*_corr_s` columns in tracy_phases.csv).

For every (query, strategy) it prints medians in ns/event: the total by each
method, the predicate time by split and corrected Tracy (raw Tracy alongside),
and the non-predicate rest. The split rest omits output conversion; Tracy's
rest includes it as automaton + output. It then prints the relative error of
each method's total against plain chrono, and the baseline/minterm_tree speedup
computed three ways.

  .venv/bin/python tests/benchmark/compare_methods.py \\
      tests/benchmark/results/overnight/20261118 [--markdown]
"""
import argparse
import csv
import math
import statistics as st
from pathlib import Path

NAN = float("nan")


def rows(results, stage, filename="manifest.csv"):
    """Rows of one stage's CSV with status ok (tracy_phases.csv has no status)."""
    path = results / stage / filename
    if not path.exists():
        return []
    return [r for r in csv.DictReader(open(path)) if r.get("status", "ok") == "ok"]


def medians(stage_rows, key, per_event=True):
    """{(query, strategy): median of key}, in ns/event when per_event."""
    values = {}
    for r in stage_rows:
        if r.get(key) in (None, ""):
            continue
        v = float(r[key])
        if per_event:
            v = v / int(r["event_count"]) * 1e9
        values.setdefault((r["query"], r["strategy"]), []).append(v)
    return {k: st.median(v) for k, v in values.items()}


def rel(value, truth):
    """Relative error of value against truth, as a percentage."""
    if math.isnan(value) or math.isnan(truth) or truth == 0:
        return NAN
    return 100.0 * (value - truth) / truth


def main():
    parser = argparse.ArgumentParser(description=__doc__.split("\n")[0])
    parser.add_argument("results_dir", type=Path)
    parser.add_argument("--markdown", action="store_true",
                        help="print markdown tables instead of aligned text")
    args = parser.parse_args()
    results = args.results_dir

    plain = rows(results, "s0_item23_chrono")
    split = rows(results, "s0_item23_split")
    tracy = rows(results, "s0_item23_tracy", "tracy_phases.csv")
    print(f"runs: plain={len(plain)} split={len(split)} tracy={len(tracy)}")
    overhead = sorted({r.get("zone_overhead_ns") for r in tracy})
    print(f"Tracy zone overhead used: {overhead} ns/zone\n")

    p_loop = medians(plain, "loop_s")
    s_proc = medians(split, "split_process_s")
    s_pred = medians(split, "split_predicate_s")
    t_loop = medians(tracy, "event_loop_corr_s")
    t_pred = medians(tracy, "predicate_corr_s")
    t_pred_raw = medians(tracy, "predicate_time_s")
    t_auto = medians(tracy, "automaton_corr_s")
    t_out = medians(tracy, "output_corr_s")

    # ---- per (query, strategy): totals, predicate, rest ----
    table = []
    for k in sorted(p_loop):
        sp, spr = s_proc.get(k, NAN), s_pred.get(k, NAN)
        table.append((
            k[0].removesuffix(".txt"), k[1],
            p_loop[k], sp, t_loop.get(k, NAN),
            spr, t_pred.get(k, NAN), t_pred_raw.get(k, NAN),
            sp - spr, t_auto.get(k, NAN) + t_out.get(k, NAN),
            rel(sp, p_loop[k]), rel(t_loop.get(k, NAN), p_loop[k]),
            rel(spr, t_pred.get(k, NAN)),
        ))

    if args.markdown:
        print("| query | strategy | total plain | total split | total Tracy | "
              "pred split | pred Tracy (raw) | rest split | rest Tracy | "
              "split total err | Tracy total err | split pred vs Tracy |")
        print("|---|---|" + "---:|" * 10)
        for t in table:
            print(f"| {t[0]} | {t[1]} | {t[2]:.0f} | {t[3]:.0f} | {t[4]:.0f} | "
                  f"{t[5]:.0f} | {t[6]:.0f} ({t[7]:.0f}) | {t[8]:.0f} | {t[9]:.0f} | "
                  f"{t[10]:+.1f}% | {t[11]:+.1f}% | {t[12]:+.1f}% |")
    else:
        print("ns/event (medians)                    |  TOTAL: plain   split   Tracy |"
              "  PRED: split  Tracy  (raw) |  REST: split  Tracy |"
              "  ERR: split.tot Tracy.tot split.pred")
        for t in table:
            print(f"{t[0][:24]:24s} {t[1][:12]:12s} | {t[2]:7.0f} {t[3]:7.0f} {t[4]:7.0f} |"
                  f" {t[5]:7.0f} {t[6]:6.0f} ({t[7]:5.0f}) | {t[8]:7.0f} {t[9]:6.0f} |"
                  f" {t[10]:+9.1f}% {t[11]:+8.1f}% {t[12]:+9.1f}%")

    # ---- speedup baseline / minterm_tree, three ways ----
    print()
    if args.markdown:
        print("| query | plain | split | Tracy |\n|---|---:|---:|---:|")
    else:
        print("speedup baseline/minterm_tree on the total loop time:")
    for q in sorted({k[0] for k in p_loop}):
        b, m = (q, "baseline"), (q, "minterm_tree")
        if b not in p_loop or m not in p_loop:
            continue
        sp = s_proc.get(b, NAN) / s_proc.get(m, NAN)
        tr = t_loop.get(b, NAN) / t_loop.get(m, NAN)
        if args.markdown:
            print(f"| {q.removesuffix('.txt')} | {p_loop[b] / p_loop[m]:.2f}x | "
                  f"{sp:.2f}x | {tr:.2f}x |")
        else:
            print(f"  {q:30s} plain={p_loop[b] / p_loop[m]:.2f}x  split={sp:.2f}x  "
                  f"Tracy={tr:.2f}x")


if __name__ == "__main__":
    main()
