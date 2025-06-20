import asyncio
from abc import ABC, abstractmethod
from typing import Any, Generic, Optional, TypeVar

import websockets
from pydantic import BaseModel

T = TypeVar("T", bound=BaseModel)


class AbstractStreamer(ABC, Generic[T]):
    """
    Base class for all streamers.
    """

    def __init__(self, py_client, py_streamer):
        self.py_client = py_client
        self.py_streamer = py_streamer

    def __repr__(self):
        return f"{self.__class__.__name__}(name={self.name})"

    def __str__(self):
        return self.name

    def __eq__(self, other):
        if isinstance(other, AbstractStreamer):
            return self.name == other.name
        return False

    @property
    @abstractmethod
    def name(self) -> str:
        """
        Abstract property to get the name of the streamer.
        Must be implemented by subclasses.
        """
        pass

    @property
    @abstractmethod
    def stream_declaration(self) -> str:
        """
        Abstract property to get the stream declaration.
        Must be implemented by subclasses.
        """
        pass

    @property
    def option_declaration(self) -> Optional[str]:
        """
        Property to get the stream declaration.
        Only needs to be implemented if options need to be specified
        """
        return None

    @property
    @abstractmethod
    def URI(self) -> str:
        """
        Abstract property to get the URI of the WebSocket.
        Must be implemented by subclasses.
        """
        pass

    @property
    @abstractmethod
    def subscribe_message_json(self) -> str:
        """
        Abstract property to get the subscription message in JSON format.
        Must be implemented by subclasses.
        """
        pass

    @abstractmethod
    def parse_message_json(self, message: str) -> T:
        """
        Abstract method to parse a message.
        Must be implemented by subclasses.
        Returns a Pydantic model.
        """
        pass

    @abstractmethod
    def get_event_id_from_model(self, model: T) -> int:
        """
        Abstract method to get event ID from a model.
        Must be implemented by subclasses.
        """
        pass

    @abstractmethod
    def create_event(self, model: T) -> Any:
        """
        Abstract method to create PyEvent from a model.
        """
        pass

    def process_message(self, message: str, stream_id: int) -> None:
        """
        Process a message. This method can be overridden by subclasses
        to implement specific message processing logic.
        """
        model = self.parse_message_json(message)
        print(f"Processing message in {self.name}: {message}")
        event_id = self.get_event_id_from_model(model)
        print(f"Event ID: {event_id}, Model: {model}")
        event = self.create_event(model)
        self.py_streamer.send_stream(stream_id, event)
        print("Sent event to streamer")

    async def receive_loop(
        self, websocket: websockets.ClientConnection, stream_id: int
    ):
        while True:
            try:
                async for message in websocket:
                    if not isinstance(message, str):
                        raise ValueError("Received message is not a string")
                    self.process_message(message, stream_id)

            except KeyboardInterrupt as e:
                raise e
            except websockets.exceptions.WebSocketException as e:
                raise e
            except Exception as e:
                print("Error parsing message, skipping.. ", e)
                continue

    async def connect_websocket_and_receive(self, stream_id: int):
        while True:
            try:
                async with websockets.connect(
                    self.URI, ping_interval=None
                ) as websocket:
                    await websocket.send(self.subscribe_message_json)
                    await self.receive_loop(websocket, stream_id)

            except KeyboardInterrupt as e:
                raise e
            except websockets.exceptions.WebSocketException as e:
                print("Connection exception, retrying..: ", e)
                await asyncio.sleep(1)

    async def start(self):
        stream_info = self.py_client.declare_stream(self.stream_declaration)
        if self.option_declaration:
            self.py_client.declare_option(self.option_declaration)

        print(f"Starting streamer: {self.name}")
        await self.connect_websocket_and_receive(stream_info.id)

    def stop(self):
        print(f"Stopping streamer: {self.name}")
