import enum
from typing import List, Dict, Union, Optional, Callable, Any
from dataclasses import dataclass
import time

from _pycore import (
    subscribe_to_queries as _subscribe_to_queries,
    PyValueTypes,
    PyIntValue,
    PyStringValue,
    PyDoubleValue,
    PyBoolValue,
    PyDateValue,
    PyAttributeInfo,
    PyEvent,
    PyEventInfoParsed,
    PyEventInfo,
    PyStream,
    PyStreamInfoParsed,
    PyStreamInfo,
    PyStreamer,
    PyClient,
    PyComplexEvent,
    PyCallbckHandler,
    PyEnumerator,
    PyClientException,
    PyAttributeNameAlreadyDeclared,
    PyAttributeNotDefinedException,
    PyEventNameAlreadyDeclared,
    PyEventNotDefinedException,
    PyEventNotInStreamException,
    PyParsingSyntaxException,
    PyStreamNameAlreadyDeclaredException,
    PyStreamNotFoundException,
    PyWarningException
)

class ValueTypes(enum.Enum):
    INT64 = PyValueTypes.INT64
    DOUBLE = PyValueTypes.DOUBLE
    BOOL = PyValueTypes.BOOL
    STRING_VIEW = PyValueTypes.STRING_VIEW
    DATE = PyValueTypes.DATE
    PRIMARY_TIME = PyValueTypes.PRIMARY_TIME

@dataclass
class AttributeInfo:
    name: str
    value_type: ValueTypes

    @classmethod
    def from_cpp(cls, cpp_attr: PyAttributeInfo):
        return cls(
            name=cpp_attr.name,
            value_type=ValueTypes(cpp_attr.value_type)
        )

@dataclass
class EventInfoParsed:
    name: str
    attributes: List[AttributeInfo]

    @classmethod
    def from_cpp(cls, cpp_event_info: PyEventInfoParsed):
        return cls(
            name=cpp_event_info.name,
            attributes=[AttributeInfo.from_cpp(attr) for attr in cpp_event_info.attributes]
        )

@dataclass
class EventInfo:
    id: int
    name: str
    attributes: List[AttributeInfo]

    @classmethod
    def from_cpp(cls, cpp_event_info: PyEventInfo):
        return cls(
            id=cpp_event_info.id,
            name=cpp_event_info.name,
            attributes=[AttributeInfo.from_cpp(attr) for attr in cpp_event_info.attributes_info]
        )

class Value:
    """Base class for all value types"""
    pass

class IntValue(Value):
    def __init__(self, value: int):
        self._cpp_value = PyIntValue(value)

    @property
    def value(self) -> int:
        return self._cpp_value.value

class StringValue(Value):
    def __init__(self, value: str):
        self._cpp_value = PyStringValue(value)

    @property
    def value(self) -> str:
        return self._cpp_value.value

class DoubleValue(Value):
    def __init__(self, value: float):
        self._cpp_value = PyDoubleValue(value)

    @property
    def value(self) -> float:
        return self._cpp_value.value

class BoolValue(Value):
    def __init__(self, value: bool):
        self._cpp_value = PyBoolValue(value)

    @property
    def value(self) -> bool:
        return self._cpp_value.value

class DateValue(Value):
    def __init__(self, value: time.time):
        self._cpp_value = PyDateValue(value)

    @property
    def value(self) -> time.time:
        return self._cpp_value.value

class Event:
    def __init__(self, event_id: int, values: List[Value]):
        cpp_values = [v._cpp_value for v in values]
        self._cpp_event = PyEvent(event_id, cpp_values)

    @property
    def id(self) -> int:
        return self._cpp_event.id

    @property
    def values(self) -> List[Value]:
        return self._cpp_event.values

@dataclass
class StreamInfoParsed:
    name: str
    events_info: List[EventInfoParsed]

    @classmethod
    def from_cpp(cls, cpp_stream_info: PyStreamInfoParsed):
        return cls(
            name=cpp_stream_info.name,
            events_info=[EventInfoParsed.from_cpp(ei) for ei in cpp_stream_info.events_info]
        )

@dataclass
class StreamInfo:
    id: int
    name: str
    events_info: List[EventInfo]

    @classmethod
    def from_cpp(cls, cpp_stream_info: PyStreamInfo):
        return cls(
            id=cpp_stream_info.id,
            name=cpp_stream_info.name,
            events_info=[EventInfo.from_cpp(ei) for ei in cpp_stream_info.events_info]
        )

class Stream:
    def __init__(self, stream_id: int, events: List[Event]):
        cpp_events = [e._cpp_event for e in events]
        self._cpp_stream = PyStream(stream_id, cpp_events)

    @property
    def id(self) -> int:
        return self._cpp_stream.id

    @property
    def events(self) -> List[Event]:
        return self._cpp_stream.events

class Streamer:
    def __init__(self, ip: str, port: int):
        self._cpp_streamer = PyStreamer(ip, port)

    def send_stream(self, stream: Union[Stream, int, Event], event: Optional[Event] = None):
        if isinstance(stream, Stream):
            self._cpp_streamer.send_stream(stream._cpp_stream)
        elif isinstance(stream, int) and isinstance(event, Event):
            self._cpp_streamer.send_stream(stream, event._cpp_event)
        else:
            raise TypeError("Invalid arguments for send_stream")

class Client:
    def __init__(self, ip: str, port: int):
        self._cpp_client = PyClient(ip, port)

    def declare_stream(self, stream_name: str) -> StreamInfoParsed:
        cpp_stream_info = self._cpp_client.declare_stream(stream_name)
        return StreamInfoParsed.from_cpp(cpp_stream_info)

    def declare_option(self, option_name: str):
        self._cpp_client.declare_option(option_name)

    def add_query(self, query: str):
        self._cpp_client.add_query(query)

    def subscribe_to_queries(self, initial_port: int, final_port: int) -> List['CallbackHandler']:
        cpp_handlers = _subscribe_to_queries(self._cpp_client, initial_port, final_port)
        return [CallbackHandler(handler) for handler in cpp_handlers]

class ComplexEvent:
    def __init__(self, cpp_complex_event: PyComplexEvent):
        self._cpp_complex_event = cpp_complex_event

    @property
    def start(self):
        return self._cpp_complex_event.start

    @property
    def end(self):
        return self._cpp_complex_event.end

    @property
    def events(self):
        return self._cpp_complex_event.events

    def to_string(self) -> str:
        return self._cpp_complex_event.to_string()

    def __str__(self) -> str:
        return self.to_string()

class CallbackHandler:
    def __init__(self, cpp_handler: PyCallbckHandler):
        self._cpp_handler = cpp_handler

    @staticmethod
    def set_event_handler(handler: Callable[['Enumerator'], None]):
        def cpp_handler(cpp_enumerator: PyEnumerator):
            enumerator = Enumerator(cpp_enumerator)
            handler(enumerator)
        PyCallbckHandler.set_event_handler(cpp_handler)

class Enumerator:
    def __init__(self, cpp_enumerator: PyEnumerator):
        self._cpp_enumerator = cpp_enumerator

    @property
    def complex_events(self) -> List[ComplexEvent]:
        return [ComplexEvent(ce) for ce in self._cpp_enumerator.complex_events]

    def __iter__(self):
        return iter(self.complex_events)

# Exceptions
class CoreException(Exception):
    pass

class AttributeNameAlreadyDeclaredException(CoreException):
    pass

class AttributeNotDefinedException(CoreException):
    pass

class EventNameAlreadyDeclaredException(CoreException):
    pass

class EventNotDefinedException(CoreException):
    pass

class EventNotInStreamException(CoreException):
    pass

class ParsingSyntaxException(CoreException):
    pass

class StreamNameAlreadyDeclaredException(CoreException):
    pass

class StreamNotFoundException(CoreException):
    pass

class WarningException(CoreException):
    pass

# Map C++ exceptions to Python exceptions
_exception_map = {
    PyClientException: CoreException,
    PyAttributeNameAlreadyDeclared: AttributeNameAlreadyDeclaredException,
    PyAttributeNotDefinedException: AttributeNotDefinedException,
    PyEventNameAlreadyDeclared: EventNameAlreadyDeclaredException,
    PyEventNotDefinedException: EventNotDefinedException,
    PyEventNotInStreamException: EventNotInStreamException,
    PyParsingSyntaxException: ParsingSyntaxException,
    PyStreamNameAlreadyDeclaredException: StreamNameAlreadyDeclaredException,
    PyStreamNotFoundException: StreamNotFoundException,
    PyWarningException: WarningException,
}

def _handle_exception(e):
    """Convert C++ exceptions to Python exceptions"""
    for cpp_exc, py_exc in _exception_map.items():
        if isinstance(e, cpp_exc):
            raise py_exc(str(e)) from e
    raise e  # If not mapped, raise original