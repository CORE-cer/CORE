"""Turn a tracy_runs.py output directory (.tracy traces + manifest.csv) into CSVs.

For each run, tracy-csvexport is called twice: once for inclusive zone times
(a zone's total includes the zones nested inside it) and once with -e for self
times (nested zones subtracted). Rows are summed by zone name (one zone name can
come from several source locations) and every zone is assigned a phase through
ZONE_PHASES below.

Outputs, written next to the traces:
  tracy_zones.csv   long format, one row per (run, zone): count, inclusive and
                    self totals, mean/min/max.
  tracy_phases.csv  one row per run. Starts with the same columns as
                    split_timing_full_corpus.csv (dataset, query, strategy,
                    repeat, event_count, compile_time_s, predicate_time_s,
                    automaton_time_s, total_time_s, matches) so existing analysis
                    keeps working, followed by the finer phase columns.

How the phases are built: per-event phases are sums of SELF times, so they
partition the per-event harness zone Bench::event exactly - nothing is counted
twice and nothing is a residual of a subtraction. The closure check below
verifies that: the self times of every zone nested in Bench::event must add up
to Bench::event's inclusive total. A zone not listed in ZONE_PHASES lands in
"unmapped" with a warning, never silently dropped.

Column meanings (all seconds, totals over the whole run):
  compile_time_s         inclusive Bench::add_query (parse ... evaluator build)
  compile_tree_build_s   inclusive MintermTree::build (0 for baseline)
  predicate_time_s       inclusive PredicateEvaluator::operator()
  predicate_tree_walk_s  inclusive MintermTree::eval (0 for baseline)
  transition_s           exec_trans/DetCEA/StateManager self times
  tecs_s                 tECS + node manager self times (incl. the per-event
                         bottom node, Evaluator::init_iteration)
  window_s               historic loop + time-window checks self times
  output_s               Evaluator::output, enumeration and conversion
  evaluator_other_s      process_event / Evaluator::next bodies outside any
                         nested zone (bookkeeping not covered by a finer zone)
  harness_s              Bench::event self: wrapping the event, the handler call
  automaton_time_s       transition + tecs + window + evaluator_other
                         (output is no longer folded in, unlike the old
                         copy-and-subtract split)
  total_time_s           inclusive Interface::SingleEvaluator::process_event
  event_loop_s           inclusive Bench::event (what the closure check uses)

With --profiling (coarse), the tECS/node-manager/determinization zones do not
exist, so that time shows up in the self time of the enclosing coarse zone
(mostly exec_trans, i.e. transition_s). Use a --profiling-fine build for that
split, keeping its higher zone overhead in mind.

Zone overhead correction (--chrono-dir or --zone-overhead-ns): in a Release
build every zone costs ~14 ns (measured 2026-11-18, two-core pinning), which
is not small next to sub-microsecond per-event phases. That cost is linear in
the number of zones, so given a no-Tracy run directory of the same queries
(tracy_runs.py --mode chrono, non-profiling build), the script fits
  traced loop time - untraced loop time = c * zones fired in the loop
through the origin, prints c and R^2, and adds *_corr columns where every
zone's self time has c * its count subtracted. The raw columns are unchanged.
The correction attributes each zone's cost to the zone itself; phases of a
few tens of ns/event remain uncertain by roughly that much.

Usage:
    .venv/bin/python tests/benchmark/tracy_to_csv.py <run_dir> \\
        [--chrono-dir <no-Tracy run_dir>] [--zone-overhead-ns C] [--tools-dir DIR]
"""

import argparse
import csv
import subprocess
import sys
from collections import defaultdict
from pathlib import Path

REPO = Path(__file__).resolve().parent.parent.parent
SEP = ";"

ZONE_PHASES = {
    # outside the event loop
    "Bench::load_events": "load",
    "Bench::add_query": "compile",
    "Compile::parse": "compile",
    "Compile::query_catalog": "compile",
    "Compile::annotate_predicates": "compile",
    "Compile::predicate_evaluator": "compile",
    "Compile::formula_to_logical_cea": "compile",
    "Compile::cea": "compile",
    "Compile::det_cea": "compile",
    "Compile::evaluator": "compile",
    "MintermTree::build": "compile",
    "MintermTree::build_type": "compile",
    "MintermTree::z3_translate": "compile",
    "MintermTree::refine": "compile",
    "MintermTree::leaf_bitmasks": "compile",
    "MintermTree::reduce": "compile",
    # the event loop
    "Bench::event": "harness",
    "Interface::SingleEvaluator::process_event": "evaluator_other",
    "Interface::GenericEvaluator::tuple_time": "evaluator_other",
    "Evaluator::next": "evaluator_other",
    "Evaluator::reset": "evaluator_other",
    "PredicateEvaluator::operator()": "predicate",
    "MintermTree::eval": "predicate",
    "MintermTree::catch_all": "predicate",
    "MintermTree::tree_walk": "predicate",
    "MintermTree::direct_fallback": "predicate",
    "AndPredicate::eval()": "predicate",
    "CompareWithConstant::eval()": "predicate",
    "CompareWithAttribute::eval()": "predicate",
    "CompareMathExprs::eval()": "predicate",
    "Attribute::eval(event)": "predicate",
    "Evaluator::exec_trans": "transition",
    "DetCEA::next": "transition",
    "DetCEA::determinize": "transition",
    "StateManager::create_state": "transition",
    "StateManager::unpin_states": "transition",
    "Evaluator::init_iteration": "tecs",
    "tECS::new_extend": "tecs",
    "tECS::merge": "tecs",
    "tECS::insert": "tecs",
    "NodeManager::alloc": "tecs",
    "NodeManager::recycle_or_grow": "tecs",
    "NodeManager::grow": "tecs",
    "Evaluator::historic_loop": "window",
    "Evaluator::is_ul_out_time_window": "window",
    "Evaluator::remove_out_of_time_nodes_ul": "window",
    "Evaluator::output": "output",
    "Catalog::convert_enumerator": "output",
    "Catalog::event_wrappers_to_complex_event": "output",
    "Internal::Enumerator::has_next": "output",
    "Internal::Enumerator::next": "output",
    "Internal::Enumerator::iterator::operator++": "output",
}
EVENT_PHASES = ["predicate", "transition", "tecs", "window", "output",
                "evaluator_other", "harness", "unmapped"]
NON_LOOP_PHASES = {"compile", "load"}
CLOSURE_TOLERANCE = 0.01


def fit_zone_overhead(runs, traced, chrono_dir: Path) -> tuple[float, float]:
    """Fit ns-per-zone from (traced loop - median untraced loop) vs zones in the loop."""
    untraced: dict[tuple, list[float]] = defaultdict(list)
    for r in csv.DictReader(open(chrono_dir / "manifest.csv")):
        if r.get("status", "ok") != "ok" or r.get("method", "plain") != "plain":
            continue
        untraced[(r["dataset"], r["query"], r["strategy"])].append(float(r["loop_s"]))
    xs, ys = [], []
    for run in runs:
        key = (run["dataset"], run["query"], run["strategy"])
        if key not in untraced or run["trace"] not in traced:
            continue
        base = sorted(untraced[key])[len(untraced[key]) // 2]
        zones = sum(z["count"] for name, z in traced[run["trace"]].items()
                    if ZONE_PHASES.get(name, "unmapped") not in NON_LOOP_PHASES)
        xs.append(zones)
        ys.append((float(run["loop_s"]) - base) * 1e9)
    if not xs:
        sys.exit(f"no run in {chrono_dir} matches a traced run")
    c = sum(x * y for x, y in zip(xs, ys)) / sum(x * x for x in xs)
    mean_y = sum(ys) / len(ys)
    ss_tot = sum((y - mean_y) ** 2 for y in ys)
    r2 = 1 - sum((y - c * x) ** 2 for x, y in zip(xs, ys)) / ss_tot if ss_tot else 1.0
    return c, r2


def export(csvexport: Path, trace: Path, self_time: bool) -> dict[str, dict]:
    """Zone name -> {count, total_ns, min_ns, max_ns}, summed over source locations.

    `trace` is either a .tracy file (exported now) or the <run>.incl.csv that
    tracy_runs.py --export-and-delete already wrote (its .self.csv sibling holds
    the self times)."""
    if trace.name.endswith(".incl.csv"):
        source = trace if not self_time else trace.with_name(
            trace.name.replace(".incl.csv", ".self.csv"))
        text = source.read_text()
    else:
        cmd = [str(csvexport), "-s", SEP] + (["-e"] if self_time else []) + [str(trace)]
        text = subprocess.run(cmd, check=True, capture_output=True, text=True).stdout
    zones: dict[str, dict] = {}
    lines = text.splitlines()
    header = lines[0].split(SEP)
    for line in lines[1:]:
        if not line:
            continue
        row = dict(zip(header, line.split(SEP)))
        z = zones.setdefault(row["name"], {"count": 0, "total_ns": 0,
                                           "min_ns": None, "max_ns": None})
        z["count"] += int(row["counts"])
        z["total_ns"] += int(float(row["total_ns"]))
        mn, mx = int(float(row["min_ns"])), int(float(row["max_ns"]))
        z["min_ns"] = mn if z["min_ns"] is None else min(z["min_ns"], mn)
        z["max_ns"] = mx if z["max_ns"] is None else max(z["max_ns"], mx)
    return zones


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    parser.add_argument("run_dir", type=Path)
    parser.add_argument("--tools-dir", type=Path, default=REPO / "build/tracy-tools/bin")
    parser.add_argument("--chrono-dir", type=Path, default=None,
                        help="no-Tracy run dir of the same queries: fit the per-zone "
                             "overhead from it and add *_corr columns")
    parser.add_argument("--zone-overhead-ns", type=float, default=None,
                        help="use this per-zone overhead instead of fitting one")
    args = parser.parse_args()

    csvexport = args.tools_dir / "tracy-csvexport"
    if not csvexport.exists():
        sys.exit(f"no tracy-csvexport in {args.tools_dir}; run scripts/build_tracy_tools.sh")
    runs = [r for r in csv.DictReader(open(args.run_dir / "manifest.csv"))
            if r.get("trace") and r.get("status", "ok") == "ok"]
    if not runs:
        sys.exit("no traced runs in the manifest")

    traced, traced_self = {}, {}
    for run in runs:
        trace = args.run_dir / run["trace"]
        traced[run["trace"]] = export(csvexport, trace, self_time=False)
        traced_self[run["trace"]] = export(csvexport, trace, self_time=True)

    zone_overhead_ns = args.zone_overhead_ns
    if zone_overhead_ns is None and args.chrono_dir is not None:
        zone_overhead_ns, r2 = fit_zone_overhead(runs, traced, args.chrono_dir)
        print(f"Fitted zone overhead: {zone_overhead_ns:.2f} ns per zone (R^2 = {r2:.3f})")
    corrected = zone_overhead_ns is not None

    zone_rows, phase_rows, problems = [], [], []
    for run in runs:
        inclusive = traced[run["trace"]]
        self_t = traced_self[run["trace"]]
        label = f"{run['dataset']}/{run['query']} {run['strategy']} r{run['repeat']}"

        phase_self = defaultdict(int)
        phase_corr = defaultdict(float)
        for name, z in inclusive.items():
            phase = ZONE_PHASES.get(name, "unmapped")
            if phase == "unmapped":
                problems.append(f"{label}: unmapped zone {name!r}")
            s = self_t.get(name, {}).get("total_ns", 0)
            phase_self[phase] += s
            row = {
                "dataset": run["dataset"], "query": run["query"],
                "strategy": run["strategy"], "repeat": run["repeat"],
                "event_count": run["event_count"], "matches": run["matches"],
                "profile_level": run["profile_level"],
                "zone": name, "phase": phase, "count": z["count"],
                "total_ns": z["total_ns"], "self_ns": s,
                "mean_ns": round(z["total_ns"] / z["count"], 2) if z["count"] else 0,
                "min_ns": z["min_ns"], "max_ns": z["max_ns"],
            }
            if corrected:
                corr = s - zone_overhead_ns * z["count"]
                phase_corr[phase] += corr
                row["self_corr_ns"] = round(corr, 1)
            zone_rows.append(row)

        def incl_s(name: str) -> float:
            return inclusive.get(name, {}).get("total_ns", 0) / 1e9

        event_loop_s = incl_s("Bench::event")
        partition_s = sum(phase_self[p] for p in EVENT_PHASES) / 1e9
        closure_error = (abs(partition_s - event_loop_s) / event_loop_s
                         if event_loop_s else 0.0)
        if closure_error > CLOSURE_TOLERANCE:
            problems.append(f"{label}: closure error {closure_error:.2%} "
                            f"(phases {partition_s:.6f}s vs Bench::event {event_loop_s:.6f}s)")

        ph = {p: phase_self[p] / 1e9 for p in EVENT_PHASES}
        row = {
            "dataset": run["dataset"], "query": run["query"],
            "strategy": run["strategy"], "repeat": run["repeat"],
            "event_count": run["event_count"],
            "compile_time_s": incl_s("Bench::add_query"),
            "predicate_time_s": incl_s("PredicateEvaluator::operator()"),
            "automaton_time_s": ph["transition"] + ph["tecs"] + ph["window"]
                                + ph["evaluator_other"],
            "total_time_s": incl_s("Interface::SingleEvaluator::process_event"),
            "matches": run["matches"],
            "profile_level": run["profile_level"],
            "compile_tree_build_s": incl_s("MintermTree::build"),
            "predicate_tree_walk_s": incl_s("MintermTree::eval"),
            "transition_s": ph["transition"], "tecs_s": ph["tecs"],
            "window_s": ph["window"], "output_s": ph["output"],
            "evaluator_other_s": ph["evaluator_other"], "harness_s": ph["harness"],
            "unmapped_s": ph["unmapped"], "event_loop_s": event_loop_s,
            "closure_error": round(closure_error, 6),
            "chrono_compile_s": run["compile_s"], "chrono_loop_s": run["loop_s"],
        }
        if corrected:
            pc = {p: phase_corr[p] / 1e9 for p in EVENT_PHASES}
            row["zone_overhead_ns"] = round(zone_overhead_ns, 3)
            for p in EVENT_PHASES:
                row[f"{p}_corr_s"] = pc[p]
            row["automaton_corr_s"] = (pc["transition"] + pc["tecs"] + pc["window"]
                                       + pc["evaluator_other"])
            row["event_loop_corr_s"] = sum(pc.values())
        phase_rows.append(row)
        print(f"{label}: loop {event_loop_s:.3f}s, predicate {ph['predicate']:.3f}s, "
              f"closure {closure_error:.3%}", flush=True)

    for name, rows in [("tracy_zones.csv", zone_rows), ("tracy_phases.csv", phase_rows)]:
        with open(args.run_dir / name, "w", newline="") as f:
            writer = csv.DictWriter(f, fieldnames=list(rows[0].keys()))
            writer.writeheader()
            writer.writerows(rows)
        print(f"Wrote {args.run_dir / name}")

    if problems:
        print(f"\n{len(problems)} problem(s):", file=sys.stderr)
        for p in sorted(set(problems)):
            print(f"  {p}", file=sys.stderr)
        sys.exit(2)


if __name__ == "__main__":
    main()
