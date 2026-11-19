"""Summarize a run_overnight.sh results directory into SUMMARY.md.

Reads each stage's manifest.csv / tracy_phases.csv / tracy_zones.csv under the
given directory (e.g. tests/benchmark/results/overnight/20261118) and writes
SUMMARY.md there, also printing it. Sections:

  Completeness  run counts and statuses per stage, plus any failed runs
  S1            synthetic corpus, plain chrono: baseline/minterm_tree speedups
                overall and per family, top wins/losses, taxis interned vs
                opaque strings, slowest tree builds
  S2            Tracy coarse, corrected: each phase's share of the event loop,
                per family and strategy
  S3            Tracy fine: operation counts per event (baseline strategy)
  S4            the repo's real queries: verification against expected_results,
                unsupported (PARTITION BY) queries, speedups per dataset

Stages that are missing are skipped, so it also works on a partial run.

  .venv/bin/python tests/benchmark/summarize_overnight.py \\
      tests/benchmark/results/overnight/20261118
"""
import argparse
import csv
import re
import statistics as st
from collections import Counter, defaultdict
from pathlib import Path

parser = argparse.ArgumentParser(description=__doc__.split("\n")[0])
parser.add_argument("results_dir", type=Path)
R = parser.parse_args().results_dir
out = []
P = out.append


def manifest(stage):
    p = R / stage / "manifest.csv"
    return list(csv.DictReader(open(p))) if p.exists() else []


def phases(stage):
    p = R / stage / "tracy_phases.csv"
    return list(csv.DictReader(open(p))) if p.exists() else []


def family(q):
    """rep_eq_24_guard.txt -> rep_eq (drops .txt, _guard and the size suffix)."""
    b = re.sub(r"\.txt$", "", q)
    b = re.sub(r"_guard$", "", b)
    return re.sub(r"_\d+$", "", b)


def med_ns(rows, key):
    """{(dataset, query, strategy): median of key in ns/event} over ok runs."""
    d = defaultdict(list)
    for r in rows:
        if r.get("status", "ok") != "ok" or r.get(key) in ("", None):
            continue
        d[(r["dataset"], r["query"], r["strategy"])].append(
            float(r[key]) / max(int(r["event_count"]), 1) * 1e9)
    return {k: st.median(v) for k, v in d.items()}


def med_raw(rows, key):
    """{(dataset, query, strategy): median of key, unscaled} over ok runs."""
    d = defaultdict(list)
    for r in rows:
        if r.get("status", "ok") == "ok" and r.get(key) not in ("", None):
            d[(r["dataset"], r["query"], r["strategy"])].append(float(r[key]))
    return {k: st.median(v) for k, v in d.items()}


P(f"# Overnight run summary - {R.name}\n")
info = (R / "run_info.txt").read_text() if (R / "run_info.txt").exists() else ""
P("```\n" + "\n".join(l for l in info.splitlines() if l.startswith(("===", "commit", "started")))
  + "\n```\n")

P("## Completeness\n")
P("| stage | runs | statuses |\n|---|---|---|")
for s in ["s0_item23_chrono", "s0_item23_split", "s0_item23_tracy", "s1_corpus_chrono",
          "s2_corpus_tracy", "s3_corpus_fine", "s4_real_chrono", "s4_real_tracy"]:
    m = manifest(s)
    P(f"| {s} | {len(m)} | {dict(Counter(r['status'] for r in m))} |")
bad = [r for s in ["s1_corpus_chrono", "s2_corpus_tracy", "s3_corpus_fine", "s4_real_chrono",
                   "s4_real_tracy"]
       for r in manifest(s) if r["status"] not in ("ok", "unsupported")]
if bad:
    P("\nFailed/timed-out runs: " + ", ".join(f"{r['label']} ({r['status']})" for r in bad[:30]))
P("")

# ---- S1 ----
s1 = manifest("s1_corpus_chrono")
if s1:
    loop = med_ns(s1, "loop_s")
    comp = med_raw(s1, "compile_s")
    rows = []
    for (d, q, s), v in loop.items():
        if s == "baseline":
            continue
        b = loop.get((d, q, "baseline"))
        if b:
            rows.append((d, q, s, b, v, b / v))
    P("## S1 - synthetic corpus, plain chrono (ground truth, full events, 5 repeats)\n")
    mt = [r for r in rows if r[2] == "minterm_tree"]
    P(f"minterm_tree vs baseline over {len(mt)} queries: median speedup "
      f"{st.median(r[5] for r in mt):.2f}x, "
      f"wins (>1.05x): {sum(r[5] > 1.05 for r in mt)}, "
      f"ties: {sum(0.95 <= r[5] <= 1.05 for r in mt)}, "
      f"losses (<0.95x): {sum(r[5] < 0.95 for r in mt)}\n")
    P("Per family (median speedup, min-max, minterm_tree):\n")
    P("| family | queries | median | min | max |\n|---|---|---|---|---|")
    fam = defaultdict(list)
    for r in mt:
        fam[(r[0], family(r[1]))].append(r[5])
    for (d, f), v in sorted(fam.items(), key=lambda kv: -st.median(kv[1])):
        P(f"| {d}/{f} | {len(v)} | {st.median(v):.2f}x | {min(v):.2f}x | {max(v):.2f}x |")
    P("\nTop 8 wins / bottom 5 (ns/event baseline -> tree):\n")
    for r in sorted(mt, key=lambda r: -r[5])[:8] + sorted(mt, key=lambda r: r[5])[:5]:
        P(f"- {r[0]}/{r[1]}: {r[3]:.0f} -> {r[4]:.0f} ns/event = **{r[5]:.2f}x**")
    it = [r for r in rows if r[2] == "minterm_tree_interned"]
    if it:
        P("\ntaxis, interned strings vs baseline / vs opaque tree:\n")
        for r in sorted(it, key=lambda r: r[1]):
            o = loop.get((r[0], r[1], "minterm_tree"))
            P(f"- {r[1]}: {r[5]:.2f}x vs baseline, {o / r[4]:.2f}x vs opaque tree")
    slow = sorted(((v, k) for k, v in comp.items() if k[2] != "baseline"), reverse=True)[:5]
    P("\nSlowest compiles (tree build): "
      + ", ".join(f"{k[1]} {k[2]} {v:.2f}s" for v, k in slow) + "\n")

# ---- S2 ----
s2 = phases("s2_corpus_tracy")
if s2:
    P("## S2 - phase breakdown (Tracy coarse, corrected; share of the event loop)\n")
    oh = sorted({r.get("zone_overhead_ns") for r in s2})
    P(f"zone overhead correction: {oh} ns/zone (see s2_convert.log for the fit's R^2)\n")
    keys = ["predicate_corr_s", "transition_corr_s", "tecs_corr_s", "window_corr_s",
            "output_corr_s", "evaluator_other_corr_s", "harness_corr_s"]
    agg = defaultdict(lambda: defaultdict(list))
    for r in s2:
        tot = float(r.get("event_loop_corr_s") or 0)
        if tot <= 0:
            continue
        for k in keys:
            agg[(r["dataset"], family(r["query"]), r["strategy"])][k].append(float(r[k]) / tot)
    P("| family | strategy | " + " | ".join(k.replace("_corr_s", "") for k in keys) + " |")
    P("|---|---|" + "---|" * len(keys))
    for (d, f, s), v in sorted(agg.items()):
        P(f"| {d}/{f} | {s} | " + " | ".join(f"{100 * st.median(v[k]):.0f}%" for k in keys) + " |")
    P("")

# ---- S3 ----
s3z = R / "s3_corpus_fine" / "tracy_zones.csv"
if s3z.exists():
    P("## S3 - exact per-event operation counts (Tracy fine, 200k events, baseline strategy)\n")
    want = ["Evaluator::exec_trans", "DetCEA::determinize", "tECS::new_extend", "tECS::merge",
            "tECS::insert", "NodeManager::alloc", "Evaluator::output"]
    per = defaultdict(dict)
    for r in csv.DictReader(open(s3z)):
        n = int(r["event_count"])
        if r["strategy"] == "baseline" and r["zone"] in want:
            per[(r["dataset"], r["query"])][r["zone"]] = int(r["count"]) / n
    top = sorted(per.items(), key=lambda kv: -kv[1].get("Evaluator::exec_trans", 0))[:10]
    P("Most automaton-heavy queries (per event):\n")
    P("| query | " + " | ".join(z.split("::")[1] for z in want) + " |\n|---|"
      + "---|" * len(want))
    for (d, q), z in top:
        P(f"| {d}/{q} | " + " | ".join(f"{z.get(w, 0):.3g}" for w in want) + " |")
    det = sorted(per.items(), key=lambda kv: -kv[1].get("DetCEA::determinize", 0))[:3]
    P("\nMost determinization misses per event: "
      + ", ".join(f"{q} {z.get('DetCEA::determinize', 0):.2g}" for (d, q), z in det) + "\n")

# ---- S4 ----
s4 = manifest("s4_real_chrono")
if s4:
    P("## S4 - the repo's real queries (core_bench, plain chrono)\n")
    ok = [r for r in s4 if r["status"] == "ok"]
    mism = sorted({(r["dataset"], r["query"], r["strategy"]) for r in ok
                   if r.get("verified") == "False"})
    unsup = sorted({(r["dataset"], r["query"]) for r in s4 if r["status"] == "unsupported"})
    ver = sum(r.get("verified") == "True" for r in ok)
    P(f"Runs verified against expected_results: {ver}; mismatches: {len(mism)}"
      + (" - " + ", ".join(f"{d}/{q} {s}" for d, q, s in mism[:15]) if mism else "") + "\n")
    P("Unsupported (PARTITION BY): " + ", ".join(f"{d}/{q}" for d, q in unsup) + "\n")
    loop = med_ns(ok, "loop_s")
    for ds in sorted({k[0] for k in loop}):
        sp = [(q, loop[(d, q, "baseline")] / loop[(d, q, s)], s) for (d, q, s) in loop
              if d == ds and s != "baseline" and (d, q, "baseline") in loop]
        mt = [x[1] for x in sp if x[2] == "minterm_tree"]
        it = [x[1] for x in sp if x[2] == "minterm_tree_interned"]
        if mt:
            P(f"- **{ds}**: minterm_tree median {st.median(mt):.2f}x "
              f"(min {min(mt):.2f}x, max {max(mt):.2f}x)"
              + (f"; interned median {st.median(it):.2f}x (min {min(it):.2f}x, "
                 f"max {max(it):.2f}x)" if it else ""))
    best = sorted(((loop[(d, q, "baseline")] / v, d, q, s) for (d, q, s), v in loop.items()
                   if s != "baseline" and (d, q, "baseline") in loop), reverse=True)
    P("\nBest: " + ", ".join(f"{d}/{q} {s} {x:.2f}x" for x, d, q, s in best[:5]))
    P("Worst: " + ", ".join(f"{d}/{q} {s} {x:.2f}x" for x, d, q, s in best[-5:]) + "\n")

text = "\n".join(out)
(R / "SUMMARY.md").write_text(text)
print(text)
