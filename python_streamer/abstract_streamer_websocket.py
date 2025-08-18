import asyncio
from abc import abstractmethod
from typing import TypeVar

import websockets
from pydantic import BaseModel

from abstract_streamer import AbstractStreamer

T = TypeVar("T", bound=BaseModel)


class AbstractStreamerWebsocket(AbstractStreamer[T]):
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

    async def setup_and_receive(self, stream_id: int):
        while True:
            try:
                async with websockets.connect(self.URI) as websocket:
                    if self.subscribe_message_json:
                        await websocket.send(self.subscribe_message_json)
                    await self.receive_loop(websocket, stream_id)

            except KeyboardInterrupt as e:
                raise e
            except websockets.exceptions.WebSocketException as e:
                print("Connection exception, retrying..: ", e)
                await asyncio.sleep(1)

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
