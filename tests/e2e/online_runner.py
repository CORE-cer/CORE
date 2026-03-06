"""Run all queries for a dataset through PyOnlineServer (ZMQ path)."""

import threading
import time
from pathlib import Path

import pycer

from tests.e2e.csv_parser import parse_csv

_port_lock = threading.Lock()
_next_port = 10000


def _allocate_ports(stride: int = 100) -> int:
    global _next_port
    with _port_lock:
        base = _next_port
        _next_port += stride
    return base


def run_dataset_online(dataset: dict, query_files: list[Path]) -> None:
    """Start a PyOnlineServer, add all queries, send all events, wait, tear down.

    Raises on crash or exception — sanitizers will flag issues via exit code.
    """
    base_port = _allocate_ports()
    router_port = base_port
    stream_listener_port = base_port + 1
    starting_query_port = base_port + 2

    server = pycer.PyOnlineServer(router_port, stream_listener_port, starting_query_port)
    client = pycer.PyClient("tcp://localhost", router_port)

    declaration = (dataset["base"] / dataset["declaration"]).read_text()
    stream_info = client.declare_stream(declaration)

    if dataset["options"]:
        options = (dataset["base"] / dataset["options"]).read_text()
        client.declare_option(options)

    # Add all queries and subscribe to results
    results: dict[str, list] = {}
    handlers = []
    for query_file in query_files:
        query = query_file.read_text()
        port = client.add_query(query)

        collected = []
        results[query_file.name] = collected

        def on_result(enumerator, dest=collected):
            for ce in enumerator:
                dest.append(ce)

        handler = pycer.PyQueryResultHandler(on_result)
        handlers.append(handler)
        client.subscribe_to_complex_event(handler, port)

    # Let ZMQ subscribers connect
    time.sleep(0.2)

    # Parse and send events
    csv_path = dataset["base"] / dataset["csv"]
    events, delays_ns = parse_csv(csv_path, stream_info)

    streamer = pycer.PyStreamer("tcp://localhost", stream_listener_port)

    has_timing = any(d > 0 for d in delays_ns)
    total = len(events)
    for i, (event, delay_ns) in enumerate(zip(events, delays_ns)):
        if has_timing and delay_ns > 0:
            time.sleep(delay_ns / 1e9)
        streamer.send_stream(stream_info.id, event)
        if (i + 1) % 10000 == 0 or i + 1 == total:
            print(f"\r  Sent {i + 1}/{total} events", end="", flush=True)
    print()

    # Wait for processing to finish
    time.sleep(2.0)

    # Cleanup: shutdown client first (releases GIL to avoid deadlock with subscriber threads)
    del streamer
    client.shutdown()
    handlers.clear()
    del client
    del server
