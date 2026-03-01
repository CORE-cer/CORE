"""Wrapper around pycer.PyClient that manages the CORE engine connection."""

from __future__ import annotations

import asyncio
import logging
from dataclasses import dataclass

import pycer

logger = logging.getLogger(__name__)


@dataclass
class QuerySubscription:
    query_id: int
    port: int
    query_string: str
    query_name: str
    handler: pycer.PyQueryResultHandler | None = None


class CoreEngine:
    """Manages connection to the C++ CORE server via ZMQ."""

    def __init__(self, host: str = "tcp://localhost", router_port: int = 5000) -> None:
        self._client = pycer.PyClient(host, router_port)
        self._subscriptions: dict[int, QuerySubscription] = {}
        self._next_query_id: int = 0
        self._result_queues: dict[int, list[asyncio.Queue]] = {}
        self._loop: asyncio.AbstractEventLoop | None = None

    def set_event_loop(self, loop: asyncio.AbstractEventLoop) -> None:
        self._loop = loop

    def declare_stream(self, declaration: str) -> pycer.PyStreamInfo:
        return self._client.declare_stream(declaration)

    def declare_option(self, option: str) -> None:
        self._client.declare_option(option)

    def add_query(self, query: str, query_name: str = "") -> int:
        port = self._client.add_query(query)
        query_id = self._next_query_id
        self._next_query_id += 1
        self._result_queues[query_id] = []

        handler = self._create_result_handler(query_id)
        self._subscriptions[query_id] = QuerySubscription(
            query_id=query_id,
            port=port,
            query_string=query,
            query_name=query_name,
            handler=handler,
        )
        self._client.subscribe_to_complex_event(handler, port)
        return query_id

    def inactivate_query(self, query_id: int) -> None:
        self._client.inactivate_query(query_id)

    def list_all_streams(self) -> list:
        return self._client.list_all_streams()

    def list_all_queries(self) -> list:
        return self._client.list_all_queries()

    def subscribe_client(self, query_id: int) -> asyncio.Queue:
        """Register a WebSocket client to receive results for a query."""
        queue: asyncio.Queue = asyncio.Queue()
        if query_id not in self._result_queues:
            self._result_queues[query_id] = []
        self._result_queues[query_id].append(queue)
        return queue

    def unsubscribe_client(self, query_id: int, queue: asyncio.Queue) -> None:
        """Unregister a WebSocket client."""
        if query_id in self._result_queues:
            try:
                self._result_queues[query_id].remove(queue)
            except ValueError:
                pass

    def _create_result_handler(self, query_id: int) -> pycer.PyQueryResultHandler:
        """Create a per-query result handler that forwards to asyncio queues."""

        def on_result(enumerator: pycer.PyEnumerator) -> None:
            result = self._enumerator_to_json(enumerator)
            if not result:
                return
            if self._loop and query_id in self._result_queues:
                for queue in self._result_queues[query_id]:
                    self._loop.call_soon_threadsafe(queue.put_nowait, result)

        return pycer.PyQueryResultHandler(on_result)

    @staticmethod
    def _enumerator_to_json(enumerator: pycer.PyEnumerator) -> list[dict]:
        results = []
        for ce in enumerator:
            results.append({
                "start": ce.start,
                "end": ce.end,
                "to_string": ce.to_string(),
            })
        return results
