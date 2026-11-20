"""Turn a replay-decomposition run into four numbers per query, and check them.

Input: a tracy_runs.py output directory made with
  --mode chrono --methods plain predicate replay replay_no_out calib
(one core_bench process per method; see
guides/benchmarking/09-replay-decomposition.md for the full derivation).

Per (dataset, query, strategy), from the median over repeats of each method's
loop time in ns/event:

  E        = plain          (w + p + a + o, the real path end to end)
  P        = predicate      (w + p + s)
  A_out    = replay         (w + r + a + o)
  A_no_out = replay_no_out  (w + r + a + d)
  H        = calib          (w + r + s)

  compile     = median compile_s over every run (ms)
  predicate   = P - H
  automaton   = A_no_out - H
  output      = A_out - A_no_out
  interaction = E - (predicate + automaton + output + H) = E - P - A_out + H

Each derived number gets a spread: the sum of its two terms' ranges (max - min
over repeats), a conservative bound on how much noise alone can move it. A
number is "resolved" when its magnitude exceeds its spread, otherwise it is
"~0 within noise".

Then every check from guide 09 is run and graded PASS / WARN / FAIL:
  checksum           bitset checksum identical across methods, repeats and
                     strategies of a query (predicates are a pure function)
  replay_matches     replay matches == plain matches (and no verified=False)
  complete           all five methods present
  interaction        |interaction| <= 5% of E (WARN above)
  automaton_strategy automaton agrees across strategies of a query
  output_strategy    output agrees across strategies of a query
  output_zero        output ~ 0 when the query has no matches
  calib              H is small and agrees across strategies
  vs_chrono          E vs an earlier plain run (--compare-chrono); only when both
                     runs read the same events (skipped for --max-events slices).
                     When both nights have a reference probe (run_overnight.sh's
                     probe_start/probe_end, found next to each results dir), the
                     earlier E is first scaled by the probe ratio, so machine
                     drift between nights is not mistaken for a code change
  vs_tracy_pred      predicate vs Tracy predicate_corr_s (--compare-tracy)
  vs_tracy_out       output vs Tracy output_corr_s (--compare-tracy)
  rep_eq_scaling     baseline predicate grows ~28 ns per predicate on rep_eq_n,
                     the tree's stays flat

Grading for "should agree" checks: PASS within the tolerance, WARN within
twice the tolerance, FAIL beyond. Tolerances are stated in each check's detail.
With a single repeat there is no spread estimate, so the noise-sensitive checks
(output_zero, *_strategy, calib across strategies) are capped at WARN. Use at
least 2 repeats (the overnight S5 uses 3) for a real verdict. The exact checks
(checksum, replay_matches, complete) can always FAIL.

Writes decomposition.csv, decomposition_checks.csv and DECOMPOSITION.md into
the results directory, and prints the markdown.

  .venv/bin/python tests/benchmark/decompose.py <results dir> \\
      [--compare-chrono <dir with manifest.csv>] [--compare-tracy <dir with tracy_phases.csv>]
"""
import argparse
import csv
import re
import statistics as st
from collections import Counter, defaultdict
from pathlib import Path

METHODS = {"plain": "E", "predicate": "P", "replay": "A_out", "replay_no_out": "A_no_out",
           "calib": "H"}
# Floors so that tiny numbers are not judged on a few ns of jitter.
MIN_TOL_NS = 10.0


def family(query):
    """rep_eq_24_guard.txt -> rep_eq (drops .txt, _guard and the size suffix)."""
    base = re.sub(r"\.txt$", "", query)
    base = re.sub(r"_guard$", "", base)
    return re.sub(r"_\d+$", "", base)


def grade(diff, tol):
    diff = abs(diff)
    return "PASS" if diff <= tol else ("WARN" if diff <= 2 * tol else "FAIL")


def load_manifest(path):
    rows = [r for r in csv.DictReader(open(path)) if r.get("status") == "ok"]
    return rows


def ns_per_event(row, key="loop_s"):
    return float(row[key]) / max(int(row["event_count"]), 1) * 1e9


def probe_speed(night_dir):
    """{(dataset, query, strategy): median ns/event} over a night's probe_start
    and probe_end (run_overnight.sh), or {} when it has none."""
    ns = defaultdict(list)
    for name in ("probe_start", "probe_end"):
        manifest = night_dir / name / "manifest.csv"
        if manifest.exists():
            for r in load_manifest(manifest):
                ns[(r["dataset"], r["query"], r["strategy"])].append(ns_per_event(r))
    return {k: st.median(v) for k, v in ns.items()}


def probe_ratio(now_dir, then_dir):
    """Median over the probe queries of now/then speed, or None without both
    probes (or when they read different events)."""
    now, then = probe_speed(now_dir), probe_speed(then_dir)
    common = [k for k in now if k in then]
    if not common:
        return None
    return st.median(now[k] / then[k] for k in common)


def summarize(rows):
    """{(dataset, query, strategy): {letter: [ns...], 'compile': [s...], ...}}"""
    groups = defaultdict(lambda: {"ns": defaultdict(list), "compile": [], "matches": {},
                                  "checksums": set(), "verified": set(), "events": None})
    for r in rows:
        if r["method"] not in METHODS:
            continue
        g = groups[(r["dataset"], r["query"], r["strategy"])]
        letter = METHODS[r["method"]]
        g["ns"][letter].append(ns_per_event(r))
        if r.get("compile_s"):
            g["compile"].append(float(r["compile_s"]))
        if r.get("matches") not in (None, ""):
            g["matches"].setdefault(letter, set()).add(int(r["matches"]))
        if r.get("bitset_checksum"):
            g["checksums"].add(r["bitset_checksum"])
        if r.get("verified") not in (None, ""):
            g["verified"].add(r["verified"])
        g["events"] = int(r["event_count"])
    return groups


def derive(g):
    """The four numbers, interaction and spreads for one group (None if incomplete)."""
    ns = g["ns"]
    if any(letter not in ns for letter in METHODS.values()):
        return None
    med = {k: st.median(v) for k, v in ns.items()}
    rng = {k: max(v) - min(v) for k, v in ns.items()}
    out = {f"{k}_ns": med[k] for k in med}
    out["repeats"] = min(len(v) for v in ns.values())
    out["compile_ms"] = st.median(g["compile"]) * 1e3 if g["compile"] else float("nan")
    out["predicate_ns"] = med["P"] - med["H"]
    out["automaton_ns"] = med["A_no_out"] - med["H"]
    out["output_ns"] = med["A_out"] - med["A_no_out"]
    out["interaction_ns"] = med["E"] - med["P"] - med["A_out"] + med["H"]
    out["interaction_pct"] = 100 * out["interaction_ns"] / med["E"] if med["E"] else 0.0
    out["predicate_spread"] = rng["P"] + rng["H"]
    out["automaton_spread"] = rng["A_no_out"] + rng["H"]
    out["output_spread"] = rng["A_out"] + rng["A_no_out"]
    out["interaction_spread"] = rng["E"] + rng["P"] + rng["A_out"] + rng["H"]
    for part in ("predicate", "automaton", "output"):
        out[f"{part}_resolved"] = abs(out[f"{part}_ns"]) > out[f"{part}_spread"]
        out[f"{part}_share_pct"] = 100 * out[f"{part}_ns"] / med["E"] if med["E"] else 0.0
    plain_matches = g["matches"].get("E", set())
    out["matches"] = next(iter(plain_matches)) if len(plain_matches) == 1 else ""
    out["events"] = g["events"]
    return out


def main():
    parser = argparse.ArgumentParser(description=__doc__.split("\n")[0])
    parser.add_argument("results_dir", type=Path)
    parser.add_argument("--compare-chrono", type=Path,
                        help="a plain-chrono tracy_runs.py dir (manifest.csv) to compare E with")
    parser.add_argument("--compare-tracy", type=Path,
                        help="a converted Tracy dir (tracy_phases.csv) to compare predicate/output with")
    args = parser.parse_args()
    results = args.results_dir

    groups = summarize(load_manifest(results / "manifest.csv"))
    derived = {k: derive(g) for k, g in groups.items()}
    checks = []

    def check(name, key, status, detail, single_repeat=False):
        # With one repeat there is no spread estimate, so a noise-sensitive
        # check cannot tell noise from a real disagreement: at most WARN.
        if single_repeat and status == "FAIL":
            status, detail = "WARN", detail + " [1 repeat: no spread estimate, FAIL downgraded]"
        d, q, s = key
        checks.append({"check": name, "dataset": d, "query": q, "strategy": s,
                       "status": status, "detail": detail})

    # ---- per (dataset, query, strategy) ----
    for key, g in sorted(groups.items()):
        res = derived[key]
        missing = [m for m, letter in METHODS.items() if letter not in g["ns"]]
        check("complete", key, "FAIL" if missing else "PASS",
              f"missing methods: {missing}" if missing else "all five methods present")
        e_matches = g["matches"].get("E", set())
        r_matches = g["matches"].get("A_out", set())
        if e_matches or r_matches:
            same = len(e_matches) == 1 and e_matches == r_matches
            check("replay_matches", key, "PASS" if same else "FAIL",
                  f"plain {sorted(e_matches)} vs replay {sorted(r_matches)}")
        if "False" in g["verified"]:
            check("replay_matches", key, "FAIL", "a run disagrees with expected_results")
        if res is None:
            continue
        e = res["E_ns"]
        check("interaction", key,
              "PASS" if abs(res["interaction_pct"]) <= 5 else "WARN",
              f"{res['interaction_ns']:+.1f} ns = {res['interaction_pct']:+.1f}% of E "
              f"(spread {res['interaction_spread']:.1f} ns; additive if within 5%)")
        if res["matches"] == 0:
            tol = max(res["output_spread"], MIN_TOL_NS)
            check("output_zero", key, grade(res["output_ns"], tol),
                  f"output {res['output_ns']:+.1f} ns with 0 matches (tol {tol:.1f} ns)",
                  single_repeat=res["repeats"] < 2)
        h_tol = 150.0
        check("calib", key, "PASS" if res["H_ns"] <= h_tol else "WARN",
              f"H = {res['H_ns']:.1f} ns/event (expected tens of ns, WARN above {h_tol:.0f})")

    # ---- per (dataset, query): across strategies ----
    by_query = defaultdict(dict)
    for (d, q, s), g in groups.items():
        by_query[(d, q)][s] = g
    for (d, q), per_strategy in sorted(by_query.items()):
        key = (d, q, "*")
        sums = set().union(*(g["checksums"] for g in per_strategy.values()))
        if sums:
            check("checksum", key, "PASS" if len(sums) == 1 else "FAIL",
                  f"{len(sums)} distinct checksum(s) over "
                  f"{len(per_strategy)} strategies: {sorted(sums)[:3]}")
        ok = {s: derived[(d, q, s)] for s in per_strategy if derived.get((d, q, s))}
        if len(ok) < 2:
            continue
        for part in ("automaton", "output"):
            vals = [r[f"{part}_ns"] for r in ok.values()]
            spread = max(r[f"{part}_spread"] for r in ok.values())
            tol = max(spread, 0.05 * max(abs(v) for v in vals), MIN_TOL_NS)
            check(f"{part}_strategy", key, grade(max(vals) - min(vals), tol),
                  ", ".join(f"{s} {r[f'{part}_ns']:.1f}" for s, r in sorted(ok.items()))
                  + f" ns (tol {tol:.1f} ns: max of spread, 5%, {MIN_TOL_NS:.0f} ns)",
                  single_repeat=min(r["repeats"] for r in ok.values()) < 2)
        hs = [r["H_ns"] for r in ok.values()]
        tol = max(0.2 * st.mean(hs), MIN_TOL_NS)
        check("calib", key, grade(max(hs) - min(hs), tol),
              f"H across strategies {', '.join(f'{h:.1f}' for h in hs)} ns (tol {tol:.1f} ns)",
              single_repeat=min(r["repeats"] for r in ok.values()) < 2)

    # ---- comparisons with earlier runs ----
    if args.compare_chrono:
        # Only comparable when both runs read the same events (a --max-events
        # run sees a different slice than a full-file one).
        earlier = defaultdict(list)
        earlier_events = {}
        for r in load_manifest(args.compare_chrono / "manifest.csv"):
            if r.get("method", "plain") in ("plain", ""):
                k = (r["dataset"], r["query"], r["strategy"])
                earlier[k].append(ns_per_event(r))
                earlier_events[k] = int(r["event_count"])
        # Machine drift between the two nights, from their reference probes.
        drift = probe_ratio(results.parent, args.compare_chrono.parent)
        drift_note = (f", earlier scaled by probe ratio {drift:.3f}" if drift
                      else ", no probe on both nights: unscaled")
        for key, res in sorted(derived.items()):
            if res and key in earlier and earlier_events[key] == res["events"]:
                then = st.median(earlier[key]) * (drift or 1.0)
                tol = max(0.10 * then, MIN_TOL_NS)
                check("vs_chrono", key, grade(res["E_ns"] - then, tol),
                      f"E {res['E_ns']:.1f} vs earlier plain {then:.1f} ns "
                      f"(tol 10%: the engine split must not slow next() down{drift_note})")
    if args.compare_tracy:
        tracy = defaultdict(lambda: defaultdict(list))
        tracy_events = {}
        for r in csv.DictReader(open(args.compare_tracy / "tracy_phases.csv")):
            k = (r["dataset"], r["query"], r["strategy"])
            n = max(int(r["event_count"]), 1)
            tracy[k]["pred"].append(float(r["predicate_corr_s"]) / n * 1e9)
            tracy[k]["out"].append(float(r["output_corr_s"]) / n * 1e9)
            tracy_events[k] = n
        for key, res in sorted(derived.items()):
            if res and key in tracy and tracy_events[key] == res["events"]:
                for part, col, name in (("predicate", "pred", "vs_tracy_pred"),
                                        ("output", "out", "vs_tracy_out")):
                    then = st.median(tracy[key][col])
                    tol = 30.0 + 0.10 * abs(then)
                    check(name, key, grade(res[f"{part}_ns"] - then, tol),
                          f"{part} {res[f'{part}_ns']:.1f} vs Tracy {then:.1f} ns "
                          f"(tol 30 ns + 10%: Tracy's known residual error)")

    # ---- rep_eq scaling (known answer from the path-length model) ----
    for strategy, expect, lo, hi in (("baseline", "~28 ns/predicate", 14.0, 56.0),
                                     ("minterm_tree", "flat", -5.0, 5.0)):
        pts = []
        for (d, q, s), res in derived.items():
            m = re.match(r"^rep_eq_(\d+)\.txt$", q)
            if res and s == strategy and d == "smart_homes" and m:
                pts.append((int(m.group(1)), res["predicate_ns"]))
        if len(pts) >= 3:
            xs, ys = zip(*sorted(pts))
            mx, my = st.mean(xs), st.mean(ys)
            slope = sum((x - mx) * (y - my) for x, y in pts) / sum((x - mx) ** 2 for x in xs)
            check("rep_eq_scaling", ("smart_homes", "rep_eq_*", strategy),
                  "PASS" if lo <= slope <= hi else "WARN",
                  f"predicate slope {slope:.2f} ns per predicate over n={list(xs)} "
                  f"(expected {expect}, PASS in [{lo}, {hi}])")

    # ---- outputs ----
    fields = ["dataset", "query", "strategy", "events", "matches", "repeats", "compile_ms",
              "E_ns", "P_ns", "A_out_ns", "A_no_out_ns", "H_ns",
              "predicate_ns", "automaton_ns", "output_ns", "interaction_ns", "interaction_pct",
              "predicate_spread", "automaton_spread", "output_spread", "interaction_spread",
              "predicate_resolved", "automaton_resolved", "output_resolved",
              "predicate_share_pct", "automaton_share_pct", "output_share_pct"]
    with open(results / "decomposition.csv", "w", newline="") as f:
        w = csv.DictWriter(f, fieldnames=fields, extrasaction="ignore")
        w.writeheader()
        for (d, q, s), res in sorted(derived.items()):
            if res:
                w.writerow({"dataset": d, "query": q, "strategy": s, **res})
    with open(results / "decomposition_checks.csv", "w", newline="") as f:
        w = csv.DictWriter(f, fieldnames=["check", "dataset", "query", "strategy", "status",
                                          "detail"])
        w.writeheader()
        w.writerows(checks)

    md = [f"# Replay decomposition - {results.name}\n",
          "Definitions and checks: guides/benchmarking/09-replay-decomposition.md. "
          "All numbers ns/event (compile in ms), medians over repeats.\n"]
    # Machine state over the night, when tracy_runs.py recorded it.
    all_rows = load_manifest(results / "manifest.csv")
    mhz = [float(r["cpu_mhz"]) for r in all_rows if r.get("cpu_mhz")]
    temps = [float(r["pkg_temp_c_max"]) for r in all_rows if r.get("pkg_temp_c_max")]
    if mhz:
        mhz.sort()
        md.append(f"Machine: busy-core clock median {st.median(mhz):.0f} MHz "
                  f"(p10 {mhz[len(mhz) // 10]:.0f}), package max temperature median "
                  f"{st.median(temps):.0f} C over {len(mhz)} runs.\n")
    if args.compare_chrono:
        md.append(f"Probe ratio vs {args.compare_chrono.parent.name}: "
                  + (f"{drift:.3f} (this night / that night)" if drift else "n/a") + "\n")
    md += [
          "## Checks\n", "| check | PASS | WARN | FAIL |", "|---|---|---|---|"]
    tally = defaultdict(Counter)
    for c in checks:
        tally[c["check"]][c["status"]] += 1
    for name in sorted(tally):
        t = tally[name]
        md.append(f"| {name} | {t['PASS']} | {t['WARN']} | {t['FAIL']} |")
    bad = [c for c in checks if c["status"] != "PASS"]
    if bad:
        md.append("\nNot passing (first 40; all in decomposition_checks.csv):\n")
        for c in sorted(bad, key=lambda c: (c["status"] != "FAIL", c["check"]))[:40]:
            md.append(f"- **{c['status']}** {c['check']} {c['dataset']}/{c['query']} "
                      f"{c['strategy']}: {c['detail']}")
    md += ["\n## Per family (medians over the family's queries)\n",
           "| family | strategy | queries | compile ms | E | predicate | automaton | output | "
           "pred % | auto % | out % | interaction % |",
           "|---|---|---|---|---|---|---|---|---|---|---|---|"]
    fam = defaultdict(list)
    for (d, q, s), res in derived.items():
        if res:
            fam[(f"{d}/{family(q)}", s)].append(res)
    for (f_name, s), rs in sorted(fam.items()):
        m = {k: st.median(r[k] for r in rs) for k in
             ("compile_ms", "E_ns", "predicate_ns", "automaton_ns", "output_ns",
              "predicate_share_pct", "automaton_share_pct", "output_share_pct",
              "interaction_pct")}
        md.append(f"| {f_name} | {s} | {len(rs)} | {m['compile_ms']:.1f} | {m['E_ns']:.0f} | "
                  f"{m['predicate_ns']:.0f} | {m['automaton_ns']:.0f} | {m['output_ns']:.0f} | "
                  f"{m['predicate_share_pct']:.0f}% | {m['automaton_share_pct']:.0f}% | "
                  f"{m['output_share_pct']:.0f}% | {m['interaction_pct']:+.1f}% |")
    text = "\n".join(md) + "\n"
    (results / "DECOMPOSITION.md").write_text(text)
    print(text)


if __name__ == "__main__":
    main()
