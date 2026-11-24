#!/usr/bin/env python3
"""ASLR on vs off: does address-space layout randomization cause the
per-process bimodal timings? (backlog item 34)

Reads two tracy_runs.py output directories holding the same runs, one launched
normally (ASLR on) and one with --no-aslr (setarch -R, ASLR off), and prints,
per (dataset, query, strategy, method), the per-event time's median, min and
max and its spread (max - min) / median, for each mode, plus:

  bimodal   the sorted per-event times have a gap larger than half their whole
            range: two clusters rather than one spread-out cloud.
  off_at    where ASLR off's median sits within ASLR on's [min, max] range, as a
            fraction (0 = at ASLR on's fastest run, 1 = at its slowest). Turning
            ASLR off fixes ONE layout; this says whether that layout happens to
            be a fast or a slow one, which matters when comparing with nights
            run with ASLR on.

The verdict (ASLR is the cause) needs both: the bimodal flags disappear with
ASLR off, and the median spread drops to at most a third of ASLR on's.

  aslr_ab.py ON_DIR OFF_DIR
"""
import argparse
import csv
import statistics as st
from collections import defaultdict
from pathlib import Path


def load(path: Path) -> dict:
    """(dataset, query, strategy, method) -> [ns per event, one per run]."""
    out = defaultdict(list)
    for r in csv.DictReader(open(path / "manifest.csv")):
        if r["status"] == "ok":
            key = (r["dataset"], r["query"], r["strategy"], r["method"] or "plain")
            out[key].append(float(r["loop_s"]) / int(r["event_count"]) * 1e9)
    return out


def describe(values: list[float]) -> dict:
    values = sorted(values)
    med = st.median(values)
    spread = (values[-1] - values[0]) / med if med else 0.0
    gaps = [b - a for a, b in zip(values, values[1:])]
    whole = values[-1] - values[0]
    # Needs a few values: with 2 or 3, one gap is always "most of the range".
    bimodal = len(values) >= 4 and whole > 0 and max(gaps) > whole / 2
    return {"n": len(values), "med": med, "min": values[0], "max": values[-1],
            "spread": spread, "bimodal": bimodal}


def main():
    ap = argparse.ArgumentParser(description=__doc__.split("\n")[0])
    ap.add_argument("on_dir", type=Path)
    ap.add_argument("off_dir", type=Path)
    args = ap.parse_args()
    on, off = load(args.on_dir), load(args.off_dir)
    keys = sorted(set(on) & set(off))

    print(f"{'query':18}{'strategy':14}{'method':10}"
          f"{'ON med':>9}{'min':>8}{'max':>8}{'spr':>7}{'bi':>4}"
          f"{'OFF med':>10}{'min':>8}{'max':>8}{'spr':>7}{'bi':>4}{'off_at':>8}")
    spreads_on, spreads_off, bi_on, bi_off, off_at = [], [], 0, 0, []
    for key in keys:
        a, b = describe(on[key]), describe(off[key])
        spreads_on.append(a["spread"])
        spreads_off.append(b["spread"])
        bi_on += a["bimodal"]
        bi_off += b["bimodal"]
        rng = a["max"] - a["min"]
        at = (b["med"] - a["min"]) / rng if rng > 0 else 0.0
        off_at.append(at)
        print(f"{key[1][:-4][:17]:18}{key[2][:13]:14}{key[3]:10}"
              f"{a['med']:9.1f}{a['min']:8.1f}{a['max']:8.1f}{a['spread'] * 100:6.1f}%"
              f"{'*' if a['bimodal'] else '':>4}"
              f"{b['med']:10.1f}{b['min']:8.1f}{b['max']:8.1f}{b['spread'] * 100:6.1f}%"
              f"{'*' if b['bimodal'] else '':>4}{at:8.2f}")

    med_on, med_off = st.median(spreads_on), st.median(spreads_off)
    print(f"\n{len(keys)} (query, strategy, method) cells, "
          f"{st.median([describe(on[k])['n'] for k in keys]):.0f} runs each per mode")
    print(f"median spread: ASLR on {med_on * 100:.1f}%, ASLR off {med_off * 100:.1f}% "
          f"(ratio {med_off / med_on if med_on else float('nan'):.2f})")
    print(f"bimodal cells: ASLR on {bi_on}, ASLR off {bi_off}")
    print(f"ASLR off's median within ASLR on's range (0 = fastest, 1 = slowest): "
          f"median {st.median(off_at):.2f}, min {min(off_at):.2f}, max {max(off_at):.2f}")
    confirmed = bi_off < max(1, bi_on // 4) and med_off <= med_on / 3
    print("verdict: " + ("ASLR CONFIRMED as the cause of the bimodality" if confirmed
                         else "ASLR NOT confirmed (bimodality or spread remain with ASLR off)"))


if __name__ == "__main__":
    main()
