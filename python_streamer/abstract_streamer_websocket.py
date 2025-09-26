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
        process_message_tasks = set()
        while True:
            try:
                async with asyncio.timeout(10):
                    message = await websocket.recv()
                    if not isinstance(message, str):
                        raise ValueError("Received message is not a string")
                    task = asyncio.create_task(
                        self.process_message_no_error(message, stream_id)
                    )
                    process_message_tasks.add(task)
                    task.add_done_callback(process_message_tasks.discard)
                    print(len(process_message_tasks), " messages being processed")

            except KeyboardInterrupt as e:
                raise e
            except websockets.exceptions.WebSocketException as e:
                raise e
            except Exception as e:
                print("Error parsing message, skipping.. ", e)
                continue

    async def process_message_no_error(self, message: str, stream_id: int):
        try:
            await self.process_message(message, stream_id)
        except Exception as e:
            print(f"Error processing message, skipping.. {e}. This should not happen.")
            return
