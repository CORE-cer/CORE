import asyncio
import json
import time
from typing import Any

import _pycore
import websockets
from models import ticker

ticker_stream_declaration = """CREATE STREAM TICKER { \n
                        EVENT Buy { product_id:string, price:double, open24h:double, volume_24h:double, low_24h:double, \
                        high_24h:double, volume_30d:double, best_bid:double, best_bid_size:double, best_ask:double, best_ask_size:double, \
                        last_size:double, time:primary_time } \n,
                        EVENT Sell { product_id:string, price:double, open24h:double, volume_24h:double, low_24h:double, \
                        high_24h:double, volume_30d:double, best_bid:double, best_bid_size:double, best_ask:double, best_ask_size:double, \
                        last_size:double, time:primary_time } \n
}
                        """

options = """CREATE QUARANTINE { \n
            FIXED_TIME 2 seconds {S} \n
            }"""


def create_ticker_attributes(ticker_event: ticker.Ticker) -> list:
    product_id = _pycore.PyStringValue(ticker_event.product_id)
    price = _pycore.PyDoubleValue(ticker_event.price)
    open_24h = _pycore.PyDoubleValue(ticker_event.open_24h)
    volume_24h = _pycore.PyDoubleValue(ticker_event.volume_24h)
    low_24h = _pycore.PyDoubleValue(ticker_event.low_24h)
    high_24h = _pycore.PyDoubleValue(ticker_event.high_24h)
    volume_30d = _pycore.PyDoubleValue(ticker_event.volume_30d)
    best_bid = _pycore.PyDoubleValue(ticker_event.best_bid)
    best_bid_size = _pycore.PyDoubleValue(ticker_event.best_bid_size)
    best_ask = _pycore.PyDoubleValue(ticker_event.best_ask)
    best_ask_size = _pycore.PyDoubleValue(ticker_event.best_ask_size)
    last_size = _pycore.PyDoubleValue(ticker_event.last_size)
    time = _pycore.PyDateValue(int(ticker_event.time.timestamp() * 1e9))
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

    return attributes


URI = "wss://ws-feed.exchange.coinbase.com"


async def handle_responses(
    websocket: websockets.ClientConnection,
    streamer: _pycore.PyStreamer,
    event_dict: dict[Any, int],
):
    while True:
        try:
            async for message in websocket:
                ticker_event_model = ticker.Ticker.model_validate_json(message)
                attributes = create_ticker_attributes(ticker_event_model)
                stream_id = 0
                event_id = event_dict[ticker_event_model.side.lower()]
                event = _pycore.PyEvent(event_id, attributes)
                streamer.send_stream(stream_id, event)
                print("Sent event to streamer")

        except KeyboardInterrupt as e:
            raise e
        except websockets.exceptions.WebSocketException as e:
            raise e
        except Exception as e:
            print("Error parsing message, skipping.. ", e)
            continue


async def subscribe_and_handle(
    subscription_message: dict, streamer: _pycore.PyStreamer
):
    subscribe_message_json = json.dumps(subscription_message)
    event_dict = {"buy": 0, "sell": 1}
    while True:
        try:
            async with websockets.connect(URI, ping_interval=None) as websocket:
                await websocket.send(subscribe_message_json)
                await handle_responses(websocket, streamer, event_dict)

        except KeyboardInterrupt as e:
            raise e
        except websockets.exceptions.WebSocketException as e:
            print("Connection exception, retrying..: ", e)
            await asyncio.sleep(1)


async def websocket_tasks(streamer: _pycore.PyStreamer):
    tasks = []
    tasks.append(subscribe_and_handle(ticker.subscription_message, streamer))
    # tasks.append(subscribe_and_handle(matches, streamer))
    await asyncio.gather(*tasks)


def run_websocket(streamer_port: int):
    streamer = _pycore.PyStreamer("tcp://localhost", streamer_port)
    asyncio.run(websocket_tasks(streamer))


if __name__ == "__main__":
    start_time_total = time.time()
    try:
        # Aca van las configuraciones iniciales
        server_port = 5000

        client = _pycore.PyClient("tcp://localhost", server_port)

        stream_info = client.declare_stream(ticker_stream_declaration)

        print(f"Stream id: {stream_info.id}, Nombre: {stream_info.name}")
        for event in stream_info.events_info:
            print(event.name, event.id)

        client.declare_option(options)

        # client.add_query(query_1)
        # client.add_query(query_2)

        # initial_port_number = 5003
        # final_port_number = 5005
        #
        # handlers = _pycore.subscribe_to_queries(
        #     client, initial_port_number, final_port_number
        # )

        # def my_handler(enumerator):
        #     for complex_event in enumerator:
        #         print(f"Data query: {complex_event.to_string()}")
        #
        # for i in range(len(handlers)):
        #     handlers[i].set_event_handler(my_handler)

        while True:
            try:
                run_websocket(server_port + 1)

            except KeyboardInterrupt:
                print("Exiting WebSocket..")
                print(f"Total time: {time.time() - start_time_total}")
                exit()

    except Exception as e:
        print("Exited unexpectedly with error when declaring with error:", e)
        print(f"Total time: {time.time() - start_time_total}")
