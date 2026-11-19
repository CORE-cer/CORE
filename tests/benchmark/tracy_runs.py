"""Run core_bench once per (query, strategy, repeat), each in its own process.

Two modes:
  --mode tracy   (default) each run is captured by tracy-capture into its own
                 .tracy file. Needs a core_bench built with --profiling or
                 --profiling-fine, and scripts/build_tracy_tools.sh.
  --mode chrono  plain runs, no capture: core_bench's own chrono timings only
                 (and, with --method split, the old copy-and-subtract split).

Built for long unattended runs:
  * resumable: runs already in manifest.csv are skipped, and every finished run
    is appended to manifest.csv immediately;
  * hang watchdog: if core_bench has printed its summary but does not exit within
    --exit-grace seconds (a profiling build waits at exit until tracy-capture has
    the data - after a suspend, capture was seen never to connect), both are
    killed and the run is retried (--retries), then recorded as failed;
  * --export-and-delete: right after capture, the trace is exported with
    tracy-csvexport (<run>.incl.csv, <run>.self.csv) and the .tracy deleted, so
    full-size runs do not fill the disk; tracy_to_csv.py reads either form;
  * --query-dir queries: the repo's real queries; every run's match count is
    checked against the dataset's expected_results (lines starting with '['),
    counted once per dataset and cached.

--queries items are dataset/pattern, e.g. smart_homes/rep_eq_24.txt or
smart_homes/* (globbed inside --query-dir). See
guides/benchmarking/08-tracy-profiling.md.
"""

import argparse
import csv
import json
import subprocess
import sys
import tarfile
import time
from datetime import datetime
from pathlib import Path

REPO = Path(__file__).resolve().parent.parent.parent
EXPERIMENTS = REPO / "src/targets/experiments"
EXPECTED_CACHE = REPO / "tests/benchmark/results/expected_counts"

FIELDS = ["label", "dataset", "query", "query_dir", "strategy", "method", "repeat",
          "status", "reason", "dataset_events", "event_count", "matches",
          "expected_matches", "verified", "compile_s", "loop_s",
          "split_predicate_s", "split_process_s", "bitset_checksum", "profile_level", "mode", "build_dir",
          "trace", "process_wall_s", "attempts"]


def run_label(dataset, query, strategy, method, repeat) -> str:
    m = "" if method == "plain" else f"__{method}"
    return f"{dataset}__{Path(query).stem}__{strategy}{m}__r{repeat}"


def expand_queries(items, query_dir, exclude) -> list[tuple[str, str]]:
    out = []
    for item in items:
        dataset, _, pattern = item.partition("/")
        if not pattern:
            sys.exit(f"--queries items must look like dataset/pattern, got {item!r}")
        matches = sorted((EXPERIMENTS / dataset / query_dir).glob(pattern))
        if not matches:
            sys.exit(f"no query matches {item!r} in {query_dir}/")
        out += [(dataset, p.name) for p in matches if f"{dataset}/{p.name}" not in exclude]
    return out


def expected_counts(dataset: str) -> dict[str, int]:
    """Query file name -> complex events in expected_results (one pass, cached)."""
    cache = EXPECTED_CACHE / f"{dataset}.json"
    if cache.exists():
        return json.loads(cache.read_text())
    archive = EXPERIMENTS / dataset / "expected_results.tar.xz"
    if not archive.exists():
        return {}
    print(f"Counting expected results for {dataset} (one-time)...", flush=True)
    counts = {}
    with tarfile.open(archive) as tf:
        for member in tf:
            if not member.isfile():
                continue
            n = 0
            with tf.extractfile(member) as f:
                for line in f:
                    if line.startswith(b"["):
                        n += 1
            counts[Path(member.name).name] = n
    EXPECTED_CACHE.mkdir(parents=True, exist_ok=True)
    cache.write_text(json.dumps(counts, indent=1))
    return counts


def kill(proc):
    if proc is not None and proc.poll() is None:
        proc.kill()
        proc.wait()


def export_trace(tools_dir: Path, trace: Path) -> None:
    for suffix, extra in [(".incl.csv", []), (".self.csv", ["-e"])]:
        with open(trace.with_suffix(suffix), "w") as out:
            subprocess.run([str(tools_dir / "tracy-csvexport"), "-s", ";", *extra, str(trace)],
                           stdout=out, check=True)


def attempt(args, out_dir, dataset, query, strategy, repeat, label):
    """One try. Returns (status, summary dict or None)."""
    json_path = out_dir / f"{label}.json"
    trace_path = out_dir / f"{label}.tracy"
    json_path.unlink(missing_ok=True)
    cmd = (["taskset", "-c", args.taskset] if args.taskset else []) + [
        str(args.build_dir / "core_bench"), "--dataset", dataset, "--query", query,
        "--query-dir", args.query_dir, "--strategy", strategy, "--method", args.method,
        "--max-events", str(args.max_events), "--repeat", str(repeat),
        "--out-json", str(json_path)]
    with open(out_dir / f"{label}.log", "a") as log:
        capture = None
        if args.mode == "tracy":
            # tracy-capture keeps retrying until the client is up, so starting it
            # first guarantees nothing at the start of the run is lost.
            capture = subprocess.Popen(
                [str(args.tools_dir / "tracy-capture"), "-f", "-o", str(trace_path)],
                stdout=log, stderr=subprocess.STDOUT)
        bench = subprocess.Popen(cmd, stdout=log, stderr=subprocess.STDOUT)
        start = time.monotonic()
        summary_seen_at = None
        status = None
        while bench.poll() is None:
            time.sleep(0.2)
            now = time.monotonic()
            if summary_seen_at is None and json_path.exists():
                summary_seen_at = now
            if summary_seen_at is not None and now - summary_seen_at > args.exit_grace:
                status = "hung_at_exit"
                break
            if now - start > args.timeout:
                status = "timeout"
                break
        if status is not None:
            kill(bench)
            kill(capture)
            return status, None
        if capture is not None:
            try:
                capture.wait(timeout=args.exit_grace)
            except subprocess.TimeoutExpired:
                kill(capture)
                return "capture_hung", None

    if not json_path.exists():
        return f"exit_{bench.returncode}", None
    summary = json.loads(json_path.read_text())
    summary["process_wall_s"] = round(time.monotonic() - start, 3)
    if summary.get("status") == "ok" and args.mode == "tracy":
        if not trace_path.exists() or trace_path.stat().st_size == 0:
            return "no_trace", None
        if args.export_and_delete:
            export_trace(args.tools_dir, trace_path)
            trace_path.unlink()
            summary["trace"] = f"{label}.incl.csv"
        else:
            summary["trace"] = trace_path.name
    else:
        trace_path.unlink(missing_ok=True)
    return summary.get("status", "ok"), summary


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    parser.add_argument("--mode", choices=["tracy", "chrono"], default="tracy")
    parser.add_argument("--method", choices=["plain", "split", "predicate", "replay", "replay_no_out", "calib"], default="plain")
    parser.add_argument("--methods", nargs="+", choices=["plain", "split", "predicate", "replay", "replay_no_out", "calib"],
                        help="several methods per (query, strategy, repeat), in an order "
                             "rotated by repeat; overrides --method. The replay "
                             "decomposition uses: plain predicate replay replay_no_out calib")
    parser.add_argument("--build-dir", type=Path, default=REPO / "build/Release-opt-prof",
                        help="directory holding core_bench")
    parser.add_argument("--tools-dir", type=Path, default=REPO / "build/tracy-tools/bin")
    parser.add_argument("--queries", nargs="+", required=True,
                        help="dataset/pattern items, e.g. smart_homes/rep_eq_24.txt or taxis/*")
    parser.add_argument("--query-dir", default="queries_optimization",
                        choices=["queries_optimization", "queries"])
    parser.add_argument("--exclude", nargs="*", default=[],
                        help="dataset/query.txt items to skip")
    parser.add_argument("--strategies", nargs="+", default=["baseline", "minterm_tree"])
    parser.add_argument("--repeats", type=int, default=3)
    parser.add_argument("--max-events", type=int, default=0, help="0 = whole dataset")
    # Two physical cores, not one: core_bench also runs quill's backend thread
    # and (profiling builds) Tracy's serializer thread. Pinned to a single core
    # they preempt the benchmark thread - measured 2026-11-18: +25-40% on plain
    # chrono runs and ~45 instead of ~14 ns per Tracy zone, with the stolen time
    # landing inside whichever zone happened to be open.
    parser.add_argument("--taskset", default="2,4",
                        help="CPU list to pin core_bench to ('' disables pinning); "
                             "keep at least two physical cores")
    parser.add_argument("--timeout", type=float, default=1800, help="per-run seconds")
    parser.add_argument("--exit-grace", type=float, default=120,
                        help="seconds core_bench may take to exit after its summary")
    parser.add_argument("--retries", type=int, default=2)
    parser.add_argument("--export-and-delete", action="store_true",
                        help="export each trace to CSV right after capture, delete the .tracy")
    parser.add_argument("--out-dir", type=Path, default=None,
                        help="default: tests/benchmark/results/tracy/<timestamp>-<mode>")
    args = parser.parse_args()

    args.build_dir = args.build_dir.resolve()
    if not (args.build_dir / "core_bench").exists():
        sys.exit(f"no core_bench in {args.build_dir}")
    if args.mode == "tracy" and not (args.tools_dir / "tracy-capture").exists():
        sys.exit(f"no tracy-capture in {args.tools_dir}; run scripts/build_tracy_tools.sh")
    methods = args.methods or [args.method]
    if args.mode == "tracy" and any(m != "plain" for m in methods):
        sys.exit("only --method plain runs under Tracy; the others are chrono measurements")

    out_dir = args.out_dir or (
        REPO / "tests/benchmark/results/tracy"
        / f"{datetime.now().strftime('%Y%m%d_%H%M%S')}-{args.mode}")
    out_dir.mkdir(parents=True, exist_ok=True)
    manifest = out_dir / "manifest.csv"
    done = set()
    fields = FIELDS
    if manifest.exists():
        # Keep an existing manifest's own header (it may predate a newer
        # column), so resumed rows stay aligned with it.
        reader = csv.DictReader(open(manifest))
        fields = reader.fieldnames or FIELDS
        done = {r["label"] for r in reader}
    else:
        with open(manifest, "w", newline="") as f:
            csv.DictWriter(f, fieldnames=FIELDS).writeheader()

    items = expand_queries(args.queries, args.query_dir, set(args.exclude))
    expected = {}
    if args.query_dir == "queries":
        for dataset in sorted({d for d, _ in items}):
            expected[dataset] = expected_counts(dataset)

    # Repeat-major order: every query/strategy runs once before any runs twice,
    # so slow drift in machine state spreads over all of them instead of one.
    # With several methods, their order rotates with the repeat number, so no
    # method always runs first (cold) or last after the same neighbour.
    def rotated(r):
        k = (r - 1) % len(methods)
        return methods[k:] + methods[:k]

    plan = [(r, d, q, s, m) for r in range(1, args.repeats + 1)
            for d, q in items for s in args.strategies for m in rotated(r)]
    for i, (repeat, dataset, query, strategy, method) in enumerate(plan, 1):
        args.method = method  # read by attempt() and the manifest row
        label = run_label(dataset, query, strategy, method, repeat)
        if label in done:
            continue
        status, summary = "not_run", None
        for tries in range(1, args.retries + 2):
            status, summary = attempt(args, out_dir, dataset, query, strategy, repeat, label)
            if summary is not None or status == "timeout":
                break
            print(f"  {label}: {status}, retrying", flush=True)
        row = {"label": label, "dataset": dataset, "query": query,
               "query_dir": args.query_dir, "strategy": strategy, "method": args.method,
               "repeat": repeat, "status": status, "mode": args.mode,
               "build_dir": str(args.build_dir), "attempts": tries, "trace": ""}
        if summary is not None:
            row.update({k: v for k, v in summary.items() if k in FIELDS})
            row["status"] = status
        # expected_results holds full-file counts, so a run cut short with
        # --max-events cannot be checked against it.
        exp = expected.get(dataset, {}).get(query) if args.max_events == 0 else None
        if exp is not None and status == "ok" and row.get("matches") not in (None, ""):
            row["expected_matches"] = exp
            row["verified"] = str(int(row["matches"]) == exp)
        with open(manifest, "a", newline="") as f:
            csv.DictWriter(f, fieldnames=fields, extrasaction="ignore").writerow(row)
        flag = "" if row.get("verified", "True") == "True" else "  <-- MATCH MISMATCH"
        print(f"[{i}/{len(plan)}] {dataset}/{query} {strategy} r{repeat}: {status} "
              f"matches={row.get('matches', '')} loop={row.get('loop_s', '')}{flag}",
              flush=True)
    print(f"Done: {manifest}")


if __name__ == "__main__":
    main()
