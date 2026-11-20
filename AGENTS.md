C++20 complex event recognition engine. Built with CMake + Ninja, dependencies managed via vcpkg.

## Build Scripts

All scripts are in `scripts/` and accept common flags defined in `scripts/common.sh`.

- **To compile only** (e.g. checking that edits build): use `scripts/build.sh`
- **To compile and run tests**: use `scripts/build_and_test.sh`

Key scripts:

| Script | Purpose |
|--------|---------|
| `scripts/build.sh` | Configure and build |
| `scripts/build_and_test.sh` | Build + run unit tests and third-party tests |
| `scripts/build_and_test_optimizations.sh` | Build (with the optional optimizations, see below) + run only the optimization unit tests (Catch2 tag `[Optimizations]`) |
| `scripts/run_optimization_tests.sh` | Run only the optimization unit tests against an already-built binary (no build step) |
| `scripts/build_and_valgrind.sh` | Build + run valgrind memory checks |
| `scripts/clang_format_all_files.sh` | Run clang-format on all source files (exits 1 if files changed) |
| `scripts/clang_tidy_check_all_files.sh` | Run clang-tidy static analysis (slow) |
| `scripts/install_dependencies.sh` | Install vcpkg dependencies |
| `scripts/build_grammar.sh` | Regenerate ANTLR grammar files |
| `scripts/build_pycer.sh` | Build Python bindings wheel (supports `-s address`/`-s thread` for sanitizers, `-b Debug`/`-b Release` for build type, `-o` for the optional optimizations) |
| `scripts/build_tracy_tools.sh` | Build `tracy-capture` / `tracy-csvexport` into `build/tracy-tools/bin/` (needed for Tracy phase timing, see below) |

Build-script flags shared by all of the above (`scripts/common.sh`): `-b <type>`, `-o`, `-s <sanitizer>`, `--profiling` / `--profiling-fine` (Tracy zones; these builds go to their own `build/<type>[-opt]-prof[-fine]` dir), `--target <cmake target>` (build only that target, e.g. `core_bench`).

### C++ phase timing (Tracy, `core_bench`)

`core_bench` (`src/tests/benchmarking/`) runs one query with one strategy through the real evaluation path, one run per process. With a `--profiling` build, `tests/benchmark/tracy_runs.py` captures each run as a `.tracy` file and `tests/benchmark/tracy_to_csv.py` turns them into per-zone and per-phase CSVs (compile, predicate evaluation, transitions, tECS, window, output). **Benchmark Release builds only** — Debug here is `-O0`. A profiling binary waits at exit for a profiler (`TRACY_NO_EXIT`), so always run it through `tracy_runs.py`. `core_bench --dump-tree` only builds the query and prints its minterm trees; `tests/benchmark/path_length_model.py` calls it once per query. `core_bench --method predicate|replay|replay_no_out|calib` (with `plain`) are the replay decomposition: predicate bitsets are recorded once and replayed into the automaton through `Evaluator::next_with_bitset`, so plain chrono yields compile / predicate / automaton / output per query. Run with `tracy_runs.py --methods plain predicate replay replay_no_out calib`, analyse with `tests/benchmark/decompose.py`, which also runs the correctness checks. Reference: `guides/benchmarking/09-replay-decomposition.md`; replay faithfulness is unit-tested by `[Replay]`. **Machine state matters on this laptop** (the clock follows the package temperature; a hot night was ~8% slower): `run_overnight.sh` sets the `performance` platform profile and checks it, cools down to <60 °C before each stage, logs the machine state, and runs a fixed reference probe first and last (`probe_start/`, `probe_end/`); `tracy_runs.py` records `cpu_mhz`/`pkg_temp_c_*` per run; compare absolute numbers across nights only through the probe ratio, which `decompose.py` applies to `vs_chrono`. Full reference: `guides/benchmarking/08-tracy-profiling.md`.

### Optional optimizations (minterm tree)

Experimental optimizations live in `src/core_server/internal/optimizations/` and are OFF by default, so the normal build, CI and wheels are unaffected.

- **Build flag**: `-o` / `--optimizations` (any build script) sets the CMake option `CORE_ENABLE_MINTERM_OPTIMIZATION`, which pulls in Z3 as a vcpkg feature (the first build compiles Z3 from source and is slow). Builds with `-o` use their own directory, `build/<type>-opt`.
- **Runtime switch**: a server chooses how predicates are evaluated with `EngineOptions.predicate_evaluation` (C++) or `pycer.PyOfflineServer(predicate_evaluation=pycer.PyPredicateEvaluation.MINTERM_TREE)` / `PyOnlineServer(..., predicate_evaluation=...)` (Python). The default is the original evaluation; requesting `MINTERM_TREE` on a build without `-o` raises an error at server construction.
- **Optional atom ordering**: when `MINTERM_TREE` is active, `EngineOptions.atom_ordering` (C++) or the `atom_ordering=` kwarg (`pycer.PyAtomOrdering.MOST_SHARED_ATTRIBUTES_FIRST`, Python) can reorder atoms before a tree is built, which can shrink the tree once combined with the leaf-merging pass below. Defaults to `AS_DISCOVERED` (today's order, no behaviour change).
- **Optional string-literal equality**: when `MINTERM_TREE` is active, `EngineOptions.string_equality` (C++) or the `string_equality=` kwarg (`pycer.PyStringEquality.INTERNED_EQUALITY`, Python) models `attr = "literal"`/`attr != "literal"` by interning each distinct literal to an integer, instead of leaving it opaque. Exact, not approximate; `LIKE`/regex and string ordering comparisons stay opaque either way. Defaults to `OPAQUE` (today's behaviour, no change).
- **Re-run the tests through the optimization**: C++ unit suite with `CORE_TEST_PREDICATE_EVALUATION=minterm_tree ./build/Debug-opt/tests`; e2e with `.venv/bin/pytest tests/e2e/ -v --server-mode offline --predicate-evaluation minterm_tree` (pycer built with `scripts/build_pycer.sh -o`) — **use `.venv/bin/pytest`, not `uv run pytest`: the latter silently rebuilds pycer without `-o`** (see tier 4 below for why). Results must be identical to the default.
- `scripts/clang_tidy_check_all_files.sh` skips the Z3-dependent files unless `-o` is passed.
- **What the `[Optimizations]` tests cover** (`src/tests/unit_tests/core_server/internal/optimizations/`). The baseline `PredicateEvaluator` is the ground truth everywhere: the tests compare the optimized output with it, or check an internal property of the module.
  - `tree_core/` (no Z3, so it also runs in the default build): the generic tree, algebra defaults, atom extractor and the speculation-safety analysis, checked against a toy algebra whose answers are brute-forced.
  - `minterm_tree/` (needs `-o`): `translator` (each atom's Z3 formula against native evaluation), `hazards` (evaluation-order problems such as a guarded division), `structure` (leaf counts, edge cases, leaf cap fallback, and that sibling leaves with the same bitset are merged), `semantics` and the original evaluator tests, `fallback` (the Z3 algebra), and `fuzz` (seeded random predicates and events).
  - `tree_core/reduce.cpp` (no Z3, so it also runs in the default build) tests `MintermTreeNode::reduce()`: the pass that collapses a node into a leaf once both of its children agree on every bit, run once after a tree's leaves get their bitsets.
  - `tree_core/atom_ordering.cpp` (no Z3) tests `order_by_shared_keys`, the generic ordering logic; `minterm_tree/minterm_tree_atom_ordering.cpp` (needs `-o`) tests `PhysicalPredicateZ3Translator::attribute_keys_of` and the whole atom-ordering feature end to end, including a worked example that measurably shrinks the post-`reduce()` tree.
  - String-literal equality (`StringEqualityStrategy::InternedEquality`): `formula_translation.cpp`/`weak_formula_translation.cpp` (no Z3) test `RecordingBuilder`'s `model_string_equality` flag; `minterm_tree_structure.cpp` and `minterm_tree_translator.cpp` (needs `-o`) test the worked example and native-evaluation fidelity (match, mismatch, and an unlisted literal) for both strongly and weakly typed string equality.
  - `optimized_engine_parity.cpp` and `optimized_predicate_evaluator_factory.cpp`: the option reaching real queries and servers. They are skipped or adapted when the optimization is not built.
  - `tree_core/formula_translation.cpp` (no Z3) tests the `translate` methods that physical predicates use to describe themselves through the Z3-free `FormulaBuilder` (`evaluation/physical_predicate/formula_builder.hpp`); the Z3 side is `PhysicalPredicateZ3Translator`.
  - Weakly typed predicates (aliases such as `X[...]`, whose attributes `NonStronglyTypedAttribute` resolves by name per event) are translated per event type, exactly like strongly typed ones. `tree_core/weak_formula_translation.cpp` (no Z3, tag `[Weak]`) tests their formulas on real CEQL, through `CompiledFilter` in the Z3-free `ceql_fixture.hpp`; the `[Weak]` cases in `translator`, `structure` and `fuzz` check them against native evaluation, leaf counts and the baseline.
  - To see the trees the optimization builds, run any test with `CORE_PRINT_MINTERM_TREES=1` (or call `MintermTreeEvaluator::trees_to_string()`); `tree_print.cpp` and `minterm_tree_print.cpp` test the printer.
  - The fuzz test runs 100 seeds by default; set `CORE_MINTERM_FUZZ_SEEDS=1000` (or more) to hunt for bugs. A failure prints its seed and predicates.

## Testing

### Testing tiers

Use the appropriate tier based on where you are in the development cycle:

**During development — run these for fast feedback on every change:**

1. **Compile + unit tests**: `scripts/build_and_test.sh`
   Builds the project and runs Catch2 unit tests + third-party tests. Fast once compiled.

2. **Unit tests with sanitizers**: `scripts/build_and_test.sh -s address` (or `-s thread`)
   Same as above but catches memory/threading bugs. Still fast.

**When validating broader correctness:**

3. **E2E integration query tests** (via pycer Python bindings):
   ```
   uv run pytest tests/e2e/ -v
   ```
   Runs all query datasets (stocks, unordered_stocks, smart_homes, taxis, ordered_bluesky, unordered_bluesky) via `PyOfflineServer` and compares output against expected results. Requires pycer to be built first (`scripts/build_pycer.sh`).
   - **Memory:** the test holds a query's whole expected and actual output in Python, at roughly 4-5x the size of its expected-output file. `smart_homes/q3_none`/`q4_none` (7.9 / 6.7 GB) are already excluded in `tests/e2e/conftest.py`. On a machine with less than ~32 GB, also deselect the queries above 1 GB, and cap pytest's memory so an overrun kills only pytest. Their match counts are still checked in pure C++ by `core_bench` (`tracy_runs.py --query-dir queries`). The queries to deselect:
     ```
     systemd-run --user --scope -p MemoryMax=8G .venv/bin/pytest tests/e2e/ -q \
       --deselect 'tests/e2e/test_queries.py::test_query[stocks/q3_none.txt]' \
       --deselect 'tests/e2e/test_queries.py::test_query[stocks/other-q4_none.txt]' \
       --deselect 'tests/e2e/test_queries.py::test_query[unordered_stocks/q3_none.txt]' \
       --deselect 'tests/e2e/test_queries.py::test_query[unordered_stocks/other-q4_none.txt]' \
       --deselect 'tests/e2e/test_queries.py::test_query[smart_homes/q2_none.txt]' \
       --deselect 'tests/e2e/test_queries.py::test_query[smart_homes/q5_none.txt]'
     ```

4. **E2E tests with sanitizers** (run alongside tier 3):
   Build pycer with sanitizers and run e2e tests. Example with address sanitizer:
   ```
   scripts/build_pycer.sh -s address
   LD_PRELOAD=$(clang -print-file-name=libclang_rt.asan-x86_64.so) .venv/bin/pytest tests/e2e/ -v -s -k "q1_ or q10_"
   ```
   Runs e2e query tests through sanitizer-instrumented C++ code for memory/threading bug detection.
   - Do NOT use `uv run` — it silently re-runs `uv sync` with no config settings at all, rebuilding pycer with neither sanitizer flags nor `-o` (confirmed by direct test: this produced 98/98 e2e failures reading as "the minterm-tree optimization was requested but this build does not include it", not a real regression). Use `.venv/bin/pytest` directly instead, for sanitizers *and* whenever `-o`/`--predicate-evaluation minterm_tree` is involved.
   - `LD_PRELOAD` is needed because clang doesn't add the sanitizer runtime to pycer.so's NEEDED entries. It force-loads the runtime before Python loads the extension.
   - Use `-k "q1_ or q10_"` to run a subset of tests — sanitizer e2e tests are slow.
   - **Pass suppression files** when running e2e tests — `scripts/common.sh` sets these for build scripts, but pytest runs outside that shell. Use `TSAN_OPTIONS="suppressions=tsan_suppressions.txt"` or `ASAN_OPTIONS="suppressions=asan_suppressions.txt"` as needed.

5. **Online mode E2E tests** (exercises ZMQ/serialization path):
   ```
   uv run pytest tests/e2e/ -v -s --server-mode online
   ```
   Runs all queries per dataset through `PyOnlineServer` (ZMQ transport, Cereal serialization). No output comparison — tests pass if no crash. Useful for catching data races, serialization bugs, and use-after-free issues under sanitizers.

**Before finishing a task:**

6. **Code quality checks**:
   - `scripts/clang_format_all_files.sh` — fast, run first
   - `scripts/clang_tidy_check_all_files.sh` — slow, runs clang-tidy across all files

### Writing tests

- Tests live in `src/tests/unit_tests/`, mirroring the source tree structure
- Framework: Catch2
- Namespace convention: `CORE::Internal::<Module>::UnitTests::<TestSuite>`
- Test files are `.cpp` and correspond to the module they test
- Use `// NOLINTBEGIN(...)` / `// NOLINTEND(...)` for clang-tidy suppressions in test code where needed

## Code Quality

Formatting and static analysis rules are defined in `.clang-format` and `.clang-tidy` at the repo root. Do not duplicate those rules — run the scripts to check compliance:
- `scripts/clang_format_all_files.sh`
- `scripts/clang_tidy_check_all_files.sh`

## Git Workflow

- **Conventional commits**: Use `feat:`, `fix:`, `chore:`, `docs:`, `test:`, `refactor:` prefixes
- **Never push directly to main** — always use feature branches and pull requests
- **Before committing**: Run `scripts/clang_format_all_files.sh` and `scripts/build_and_test.sh` at minimum

## Python Bindings

Build the `pycer` Python package using the build script:

```
scripts/build_pycer.sh
```

This compiles the C++ bindings via scikit-build-core/CMake and installs the `pycer` package. Supports `-s address`/`-s thread` for sanitizers.

## Dependencies

- Managed via vcpkg (`vcpkg.json`). Install with `scripts/install_dependencies.sh`
- ANTLR grammars: Regenerate with `scripts/build_grammar.sh` if `.g4` files change
