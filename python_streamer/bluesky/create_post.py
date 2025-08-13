import json
from typing import Optional, final

import _pycore
from abstract_streamer_websocket import AbstractStreamerWebsocket
from bluesky.models.commit import EventCommitCreatePostModel


@final
class CreatePostStreamer(AbstractStreamerWebsocket[EventCommitCreatePostModel]):
    @property
    def name(self) -> str:
        return "CreatePostStreamer"

    @property
    def stream_declaration(self) -> str:
        return """
        CREATE STREAM Bluesky {
            EVENT CreatePost {
                kind: string,
                time: primary_time,
                text: string
            }
        }
        """

    @property
    def option_declaration(self) -> Optional[str]:
        return """
                    CREATE QUARANTINE 
                    { \n
                    FIXED_TIME 2 seconds {Bluesky} \n
                    }
                    """

    @property
    def URI(self) -> str:
        return "wss://jetstream2.us-west.bsky.network/subscribe"

    @property
    def subscribe_message_json(self) -> str:
        return ""

    def parse_message_json(self, message: str) -> Optional[EventCommitCreatePostModel]:
        try:

            m = json.loads(message)
            print(m)
            # model = EventCommitCreatePostModel.model_validate_json(message)
            return None
        except Exception:
            return None

    def get_event_id_from_model(self, model: EventCommitCreatePostModel) -> int:
        return 0

    def create_event(self, model: EventCommitCreatePostModel):
        kind = _pycore.PyStringValue(model.kind)
        time = _pycore.PyIntValue(int(model.time_us * 1e3))
        text = _pycore.PyStringValue(model.commit.record.text)

        attributes = [kind, time, text]

        event_id = self.get_event_id_from_model(model)

        event = _pycore.PyEvent(event_id, attributes, time)

        return event
        # product_id = _pycore.PyStringValue(model.product_id)
        # price = _pycore.PyDoubleValue(model.price)
        # open_24h = _pycore.PyDoubleValue(model.open_24h)
        # volume_24h = _pycore.PyDoubleValue(model.volume_24h)
        # low_24h = _pycore.PyDoubleValue(model.low_24h)
        # high_24h = _pycore.PyDoubleValue(model.high_24h)
        # volume_30d = _pycore.PyDoubleValue(model.volume_30d)
        # best_bid = _pycore.PyDoubleValue(model.best_bid)
        # best_bid_size = _pycore.PyDoubleValue(model.best_bid_size)
        # best_ask = _pycore.PyDoubleValue(model.best_ask)
        # best_ask_size = _pycore.PyDoubleValue(model.best_ask_size)
        # last_size = _pycore.PyDoubleValue(model.last_size)
        # time = _pycore.PyIntValue(int(model.time.timestamp() * 1e9))
        # attributes = [
        #     product_id,
        #     price,
        #     open_24h,
        #     volume_24h,
        #     low_24h,
        #     high_24h,
        #     volume_30d,
        #     best_bid,
        #     best_bid_size,
        #     best_ask,
        #     best_ask_size,
        #     last_size,
        #     time,
        # ]
        # event_id = self.get_event_id_from_model(model)
        #
        # event = _pycore.PyEvent(event_id, attributes, time)
        #
        # return event
