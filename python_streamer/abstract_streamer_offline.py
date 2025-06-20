import asyncio
from abc import abstractmethod
from pathlib import Path
from typing import TypeVar

from pydantic import BaseModel

from abstract_streamer import AbstractStreamer

T = TypeVar("T", bound=BaseModel)


class AbstractStreamerOffline(AbstractStreamer[T]):
    @property
    @abstractmethod
    def text_file_path(self) -> Path:
        pass

    async def setup_and_receive(self, stream_id: int):
        with open(self.text_file_path) as f:
            for line in f.readlines():
                self.process_message(line, stream_id)
                await asyncio.sleep(0.10)
