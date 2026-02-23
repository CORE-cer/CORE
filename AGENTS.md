C++23 complex event recognition engine. Built with CMake + Ninja, dependencies managed via vcpkg.

## Build Scripts

All scripts are in `scripts/` and accept common flags defined in `scripts/common.sh`.

- **To compile only** (e.g. checking that edits build): use `scripts/build.sh`
- **To compile and run tests**: use `scripts/build_and_test.sh`

Key scripts:

| Script | Purpose |
|--------|---------|
| `scripts/build.sh` | Configure and build |
| `scripts/build_and_test.sh` | Build + run unit tests and third-party tests |
| `scripts/build_and_valgrind.sh` | Build + run valgrind memory checks |
| `scripts/clang_format_all_files.sh` | Run clang-format on all source files (exits 1 if files changed) |
| `scripts/clang_tidy_check_all_files.sh` | Run clang-tidy static analysis (slow) |
| `scripts/install_dependencies.sh` | Install vcpkg dependencies |
| `scripts/build_grammar.sh` | Regenerate ANTLR grammar files |
| `scripts/build_pycer.sh` | Build Python bindings wheel |

## Testing

### Testing tiers

Use the appropriate tier based on where you are in the development cycle:

**During development — run these for fast feedback on every change:**

1. **Compile + unit tests**: `scripts/build_and_test.sh`
   Builds the project and runs Catch2 unit tests + third-party tests. Fast once compiled.

2. **Unit tests with sanitizers**: `scripts/build_and_test.sh -s address` (or `-s thread`)
   Same as above but catches memory/threading bugs. Still fast.

**When validating broader correctness — run these together:**

3. **Integration query tests**: Scripts in `scripts/queries/base_queries/`
   - `build_and_test_stock_queries.sh` (ordered)
   - `build_and_test_stock_queries_unordered.sh`
   - `build_and_test_smart_homes_queries.sh`
   - `build_and_test_taxi_queries.sh`
   - `build_and_test_bluesky_ordered.sh`
   - `build_and_test_bluesky_unordered.sh`

   These are slow. They compare output against expected results with `diff`. Note: output order differences can cause false failures even when the answer is correct.

4. **Sanitizers + targeted integration** (run alongside tier 3):
   Run only the stock queries with `CORE_TEST_FIRST_QUERY_ONLY=1` to keep it feasible:
   ```
   CORE_TEST_FIRST_QUERY_ONLY=1 scripts/queries/base_queries/build_and_test_stock_queries.sh -s address
   CORE_TEST_FIRST_QUERY_ONLY=1 scripts/queries/base_queries/build_and_test_stock_queries_unordered.sh -s address
   ```

**Before finishing a task:**

5. **Code quality checks**:
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

## Dependencies

- Managed via vcpkg (`vcpkg.json`). Install with `scripts/install_dependencies.sh`
- ANTLR grammars: Regenerate with `scripts/build_grammar.sh` if `.g4` files change
