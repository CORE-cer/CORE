"""Fixtures for e2e integration tests."""

import re
import shutil
import tarfile
from pathlib import Path

import pytest

EXPERIMENTS_DIR = Path(__file__).parent.parent.parent / "src" / "targets" / "experiments"

DATASETS = [
    {
        "name": "stocks",
        "base": EXPERIMENTS_DIR / "stocks",
        "declaration": "declaration.core",
        "csv": "stock_data.csv",
        "options": None,
        "exclude": [],
    },
    {
        "name": "unordered_stocks",
        "base": EXPERIMENTS_DIR / "unordered_stocks",
        "declaration": "declaration.core",
        "csv": "stock_data.csv",
        "options": "quarantine_declaration.core",
        "exclude": [],
    },
    {
        "name": "smart_homes",
        "base": EXPERIMENTS_DIR / "smart_homes",
        "declaration": "declaration.core",
        "csv": "smart_homes_data.csv",
        "options": None,
        "exclude": ["q3_none.txt", "q4_none.txt"],
    },
    {
        "name": "taxis",
        "base": EXPERIMENTS_DIR / "taxis",
        "declaration": "declaration.core",
        "csv": "taxi_data.csv",
        "options": None,
        "exclude": [],
        "csv_archive": "taxi_data.tar.xz",
    },
    {
        "name": "ordered_bluesky",
        "base": EXPERIMENTS_DIR / "ordered_bluesky",
        "declaration": "declaration.core",
        "csv": "bluesky_ordered.corecsv",
        "options": "quarantine_declaration.core",
        "exclude": [],
    },
    {
        "name": "unordered_bluesky",
        "base": EXPERIMENTS_DIR / "unordered_bluesky",
        "declaration": "declaration.core",
        "csv": "bluesky_unordered.corecsv",
        "options": "quarantine_declaration.core",
        "exclude": [],
    },
]


def _extract_if_needed(base: Path, archive_name: str, target_check: str | Path) -> None:
    target = base / target_check if isinstance(target_check, str) else target_check
    archive = base / archive_name
    if not target.exists() and archive.exists():
        with tarfile.open(archive) as tf:
            tf.extractall(path=base, filter="data")


def _version_sort_key(path: Path) -> list[int | str]:
    parts = re.split(r"(\d+)", path.name)
    return [int(p) if p.isdigit() else p for p in parts]


def discover_test_cases() -> list[tuple[dict, Path, Path]]:
    cases = []
    for dataset in DATASETS:
        base = dataset["base"]
        queries_dir = base / "queries"
        expected_dir = base / "expected_results"

        if not queries_dir.exists():
            continue

        query_files = sorted(queries_dir.iterdir(), key=_version_sort_key)
        for query_file in query_files:
            if query_file.name in dataset["exclude"]:
                continue
            expected_file = expected_dir / query_file.name
            cases.append((dataset, query_file, expected_file))
    return cases


def _case_id(case: tuple[dict, Path, Path]) -> str:
    dataset, query_file, _ = case
    return f"{dataset['name']}/{query_file.name}"


@pytest.fixture(scope="session", autouse=True)
def extract_archives():
    for dataset in DATASETS:
        base = dataset["base"]
        _extract_if_needed(base, "expected_results.tar.xz", "expected_results")
        if "csv_archive" in dataset:
            _extract_if_needed(base, dataset["csv_archive"], dataset["csv"])
    yield
    for dataset in DATASETS:
        expected_dir = dataset["base"] / "expected_results"
        if expected_dir.exists():
            shutil.rmtree(expected_dir)


def pytest_generate_tests(metafunc):
    if "dataset" in metafunc.fixturenames:
        cases = discover_test_cases()
        metafunc.parametrize(
            "dataset,query_file,expected_file",
            cases,
            ids=[_case_id(c) for c in cases],
        )
