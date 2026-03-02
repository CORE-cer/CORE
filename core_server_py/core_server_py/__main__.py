"""CLI entry point for the CORE Python server."""

import argparse

import uvicorn

from .app import app


def main() -> None:
    parser = argparse.ArgumentParser(description="CORE Python Server")
    parser.add_argument("--host", default="0.0.0.0", help="Host to bind to")
    parser.add_argument("--port", type=int, default=8000, help="Port to listen on")
    parser.add_argument(
        "--core-host",
        default="tcp://localhost",
        help="C++ CORE server host (ZMQ address)",
    )
    parser.add_argument(
        "--core-router-port",
        type=int,
        default=5000,
        help="C++ CORE server router port",
    )
    args = parser.parse_args()

    app.state.core_host = args.core_host
    app.state.core_router_port = args.core_router_port

    uvicorn.run(app, host=args.host, port=args.port)


if __name__ == "__main__":
    main()
