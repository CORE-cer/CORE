import pathlib
from typing import Optional, final

import _pycore
from abstract_streamer_offline import AbstractStreamerOffline
from coinbase.models.ticker import TickerModel


@final
class TickerStreamerOffline(AbstractStreamerOffline[TickerModel]):
    @property
    def name(self) -> str:
        return "TickerStreamer"

    @property
    def stream_declaration(self) -> str:
        return """CREATE STREAM TICKER 
                { \n
                EVENT Buy { product_id:string, price:double, open24h:double, volume_24h:double, low_24h:double, \
                high_24h:double, volume_30d:double, best_bid:double, best_bid_size:double, best_ask:double, best_ask_size:double, \
                last_size:double, time:primary_time } \n,
                EVENT Sell { product_id:string, price:double, open24h:double, volume_24h:double, low_24h:double, \
                high_24h:double, volume_30d:double, best_bid:double, best_bid_size:double, best_ask:double, best_ask_size:double, \
                last_size:double, time:primary_time } \n
                }
                """

    @property
    def option_declaration(self) -> Optional[str]:
        return """
                    CREATE QUARANTINE 
                    { \n
                    FIXED_TIME 2 seconds {TICKER} \n
                    }
                    """

    @property
    def text_file_path(self) -> pathlib.Path:
        current_path = pathlib.Path(__file__).parent
        print(current_path / "ticker.txt")
        return current_path / "ticker.txt"

    def parse_message_json(self, message: str) -> TickerModel:
        return TickerModel.model_validate_json(message)

    def get_event_id_from_model(self, model: TickerModel) -> int:
        event_dict = {"buy": 0, "sell": 1}
        event_id = event_dict[model.side.lower()]
        return event_id

    def create_event(self, model: TickerModel):
        product_id = _pycore.PyStringValue(model.product_id)
        price = _pycore.PyDoubleValue(model.price)
        open_24h = _pycore.PyDoubleValue(model.open_24h)
        volume_24h = _pycore.PyDoubleValue(model.volume_24h)
        low_24h = _pycore.PyDoubleValue(model.low_24h)
        high_24h = _pycore.PyDoubleValue(model.high_24h)
        volume_30d = _pycore.PyDoubleValue(model.volume_30d)
        best_bid = _pycore.PyDoubleValue(model.best_bid)
        best_bid_size = _pycore.PyDoubleValue(model.best_bid_size)
        best_ask = _pycore.PyDoubleValue(model.best_ask)
        best_ask_size = _pycore.PyDoubleValue(model.best_ask_size)
        last_size = _pycore.PyDoubleValue(model.last_size)
        time = _pycore.PyIntValue(int(model.time.timestamp() * 1e9))
        attributes = [
            product_id,
            price,
            open_24h,
            volume_24h,
            low_24h,
            high_24h,
            volume_30d,
            best_bid,
            best_bid_size,
            best_ask,
            best_ask_size,
            last_size,
            time,
        ]
        event_id = self.get_event_id_from_model(model)

        event = _pycore.PyEvent(event_id, attributes, time)

        return event
