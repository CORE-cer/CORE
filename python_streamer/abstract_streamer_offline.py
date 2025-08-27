import asyncio
import json
from abc import abstractmethod
from pathlib import Path
from typing import TypeVar

from pydantic import BaseModel
from tqdm import tqdm

from abstract_streamer import AbstractStreamer

T = TypeVar("T", bound=BaseModel)


class AbstractStreamerOffline(AbstractStreamer[T]):
    @property
    @abstractmethod
    def text_file_path(self) -> Path:
        pass

    async def setup_and_receive(self, stream_id: int):
        with open(self.text_file_path) as f:
            models = json.load(f)
            for model in tqdm(models):
                model["commit"]["record"]["$type"] = model["commit"]["record"][
                    "record_type"
                ]
                del model["commit"]["record"]["record_type"]
                self.process_message(model, stream_id)
                await asyncio.sleep(0.0001)
            # for line in f.readlines():
            #     self.process_message(line, stream_id)
            #     await asyncio.sleep(0.10)
