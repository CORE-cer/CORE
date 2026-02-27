"""E2E integration tests: run queries via PyOfflineServer and compare output."""

import time
from pathlib import Path

import pycer

from tests.e2e.csv_parser import parse_csv


def test_query(dataset: dict, query_file: Path, expected_file: Path) -> None:
    server = pycer.PyOfflineServer()

    declaration = (dataset["base"] / dataset["declaration"]).read_text()
    stream_info = server.declare_stream(declaration)

    if dataset["options"]:
        options = (dataset["base"] / dataset["options"]).read_text()
        server.declare_option(options)

    query = query_file.read_text()
    server.add_query(query)

    csv_path = dataset["base"] / dataset["csv"]
    events, delays_ns = parse_csv(csv_path, stream_info)

    has_timing = any(d > 0 for d in delays_ns)
    if has_timing:
        # Replay events with inter-event timing delays to match offline
        # binary behavior (required for quarantine policies like
        # BOUNDED_TIME that depend on wall-clock event arrival spacing).
        for event, delay_ns in zip(events, delays_ns):
            if delay_ns > 0:
                time.sleep(delay_ns / 1e9)
            server.send_event(event)
    else:
        server.send_events(events)

    engine_output = server.get_output()

    # Prepend header matching offline binary output format
    header = f"Query: {query}\nRead events {len(events)}\n"
    actual_output = header + engine_output

    expected = expected_file.read_text()
    assert actual_output == expected, (
        f"Output mismatch for {dataset['name']}/{query_file.name}"
    )
