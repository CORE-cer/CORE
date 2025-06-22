from abc import ABC, abstractmethod
from typing import Any, Generic, Optional, TypeVar

from pydantic import BaseModel

import _pycore

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
        event_id = self.get_event_id_from_model(model)
        if model.product_id != "ETH-USD":
            return
        print(f"Processing message in {self.name}: {message}")
        print(f"Event ID: {event_id}, Model: {model}")
        event = self.create_event(model)
        self.py_streamer.send_stream(stream_id, event)
        print("Sent event to streamer")

    @abstractmethod
    async def setup_and_receive(self, stream_id: int):
        pass

    async def start(self):
        try:
            stream_info = self.py_client.declare_stream(self.stream_declaration)
            # if self.option_declaration:
            #     self.py_client.declare_option(self.option_declaration)
            stream_id = stream_info.id
        except _pycore.PyStreamNameAlreadyDeclaredException:
            stream_id = 0

        print(f"Starting streamer: {self.name}")
        await self.setup_and_receive(stream_id)

    def stop(self):
        print(f"Stopping streamer: {self.name}")
