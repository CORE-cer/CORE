"""FastAPI application that replaces the C++ HTTP/WebSocket server."""

from __future__ import annotations

import asyncio
import logging
from contextlib import asynccontextmanager

from fastapi import FastAPI, WebSocket, WebSocketDisconnect, Request, Response
from fastapi.middleware.cors import CORSMiddleware
from fastapi.responses import PlainTextResponse

from .engine import CoreEngine

logger = logging.getLogger(__name__)

engine: CoreEngine | None = None


@asynccontextmanager
async def lifespan(app: FastAPI):
    global engine
    host = app.state.core_host
    port = app.state.core_router_port
    engine = CoreEngine(host=host, router_port=port)
    engine.set_event_loop(asyncio.get_running_loop())
    yield
    engine = None


app = FastAPI(lifespan=lifespan)

app.add_middleware(
    CORSMiddleware,
    allow_origins=["*"],
    allow_methods=["GET", "POST", "PUT", "DELETE", "OPTIONS"],
    allow_headers=["origin", "content-type", "accept", "x-requested-with"],
    max_age=3600,
)


@app.get("/all-streams-info")
async def all_streams_info():
    streams = engine.list_all_streams()
    result = []
    for s in streams:
        events = []
        for e in s.events_info:
            events.append({"id": e.id, "name": e.name})
        result.append({"id": s.id, "name": s.name, "events_info": events})
    return result


@app.get("/all-queries-info")
async def all_queries_info():
    queries = engine.list_all_queries()
    result = []
    for q in queries:
        result.append({
            "result_handler_identifier": q.result_handler_identifier,
            "query_string": q.query_string,
            "query_name": q.query_name,
            "active": q.active,
        })
    return result


@app.post("/declare-stream")
async def declare_stream(request: Request):
    body = await request.json()
    declaration = body.get("declaration", "")
    try:
        stream_info = engine.declare_stream(declaration)
        events = []
        for e in stream_info.events_info:
            events.append({"id": e.id, "name": e.name})
        return {"id": stream_info.id, "name": stream_info.name, "events_info": events}
    except Exception as e:
        return PlainTextResponse(str(e), status_code=400)


@app.post("/add-query")
async def add_query(request: Request):
    body = await request.json()
    query = body.get("query", "")
    query_name = body.get("query_name", "")
    try:
        query_id = engine.add_query(query, query_name)
        return PlainTextResponse(str(query_id))
    except Exception as e:
        return PlainTextResponse(str(e), status_code=400)


@app.delete("/inactivate-query/{query_id}")
async def inactivate_query(query_id: int):
    engine.inactivate_query(query_id)
    return Response(status_code=200)


@app.websocket("/{query_id}")
async def websocket_endpoint(websocket: WebSocket, query_id: int):
    await websocket.accept()
    queue = engine.subscribe_client(query_id)
    try:
        while True:
            result = await queue.get()
            await websocket.send_json(result)
    except WebSocketDisconnect:
        pass
    finally:
        engine.unsubscribe_client(query_id, queue)


@app.get("/{path:path}")
async def fallback(path: str):
    return PlainTextResponse("Hello world!")
