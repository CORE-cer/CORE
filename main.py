import asyncio
import base64
import csv
import datetime
import hashlib
import hmac
import json
import os
import time

import _pycore
import websockets
from dateutil.parser import isoparse

API_KEY = str(os.environ.get("API_KEY"))
PASSPHRASE = str(os.environ.get("PASSPHRASE"))
SECRET_KEY = str(os.environ.get("SECRET_KEY"))

URI = "wss://ws-feed.exchange.coinbase.com"
SIGNATURE_PATH = "/users/self/verify"

channel = "level2"
product_ids = "ETH-USD"


async def generate_signature():
    timestamp = str(time.time())
    message = f"{timestamp}GET{SIGNATURE_PATH}"
    hmac_key = base64.b64decode(SECRET_KEY)
    signature = hmac.new(
        hmac_key, message.encode("utf-8"), digestmod=hashlib.sha256
    ).digest()
    signature_b64 = base64.b64encode(signature).decode().rstrip("\n")
    return signature_b64, timestamp


level2_batch = {
    "type": "subscribe",
    "product_ids": ["ETH-USD", "BTC-USD"],
    "channels": ["level2_batch"],
}

heartbeat = {
    "type": "subscribe",
    "channels": [
        {
            "name": "heartbeat",
            "product_ids": [
                "ETH-EUR",
                "BTC-USD",
                "LTC-USD",
                "ANKR-USD",
                "DOT-EUR",
                "JASMY-USD",
                "IOTX-USD",
            ],
        }
    ],
}

rfq_matches = {
    "type": "subscribe",
    "channels": [
        {
            "name": "rfq_matches",
            "product_ids": [
                "",
            ],
        },
    ],
}

matches = {
    "type": "subscribe",
    "product_ids": [
        "ETH-USD",
        "BTC-USD",
        "LTC-USD",
        # "ANKR-USD",
        # "DOT-EUR",
        # "JASMY-USD",
        # "IOTX-USD"
    ],
    "channels": ["matches"],
}

ticker = {
    "type": "subscribe",
    "product_ids": [
        "ETH-USD",
        "BTC-USD",
        "LTC-USD",
        # "ANKR-USD",
        # "DOT-EUR",
        # "JASMY-USD",
        # "IOTX-USD"
    ],
    "channels": ["ticker"],
}


## Attributes creators with stream declarations
def create_ticker_attributes(json_response):
    sequence = _pycore.PyIntValue(json_response["sequence"])
    product_id = _pycore.PyStringValue(json_response["product_id"])
    price = _pycore.PyDoubleValue(float(json_response["price"]))
    open_24h = _pycore.PyDoubleValue(float(json_response["open_24h"]))
    volume_24h = _pycore.PyDoubleValue(float(json_response["volume_24h"]))
    low_24h = _pycore.PyDoubleValue(float(json_response["low_24h"]))
    high_24h = _pycore.PyDoubleValue(float(json_response["high_24h"]))
    volume_30d = _pycore.PyDoubleValue(float(json_response["volume_30d"]))
    best_bid = _pycore.PyDoubleValue(float(json_response["best_bid"]))
    best_bid_size = _pycore.PyDoubleValue(float(json_response["best_bid_size"]))
    best_ask = _pycore.PyDoubleValue(float(json_response["best_ask"]))
    best_ask_size = _pycore.PyDoubleValue(float(json_response["best_ask_size"]))
    time = _pycore.PyDateValue(int(isoparse(json_response["time"]).timestamp() * 1e9))
    trade_id = _pycore.PyIntValue(json_response["trade_id"])
    last_size = _pycore.PyDoubleValue(float(json_response["last_size"]))

    attributes = [
        sequence,
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
        time,
        trade_id,
        last_size,
    ]

    return attributes


ticker_stream_declaration = """DECLARE STREAM S { \n
                        EVENT BUY { sequence:int, product_id:string, price:double, open24h:double, volume_24h:float, low_24h:float, \
                        high_24h:float, volume_30d:float, best_bid:float, best_bid_size:float, best_ask:float, best_ask_size:float, \
                        time:date, trade_id:int, last_size:float } \n,
                        EVENT SELL { sequence:int, product_id:string, price:double, open24h:double, volume_24h:float, low_24h:float, \
                        high_24h:float, volume_30d:float, best_bid:float, best_bid_size:float, best_ask:float, best_ask_size:float, \
                        time:date, trade_id:int, last_size:float } \n
                        }
                        """


def create_matches_attributes(json_response):
    trade_id = _pycore.PyIntValue(json_response["trade_id"])
    sequence = _pycore.PyIntValue(json_response["sequence"])
    maker_order_id = _pycore.PyStringValue(json_response["maker_order_id"])
    taker_order_id = _pycore.PyStringValue(json_response["taker_order_id"])
    time = _pycore.PyDateValue(int(isoparse(json_response["time"]).timestamp() * 1e9))
    product_id = _pycore.PyStringValue(json_response["product_id"])
    size = _pycore.PyDoubleValue(float(json_response["size"]))
    price = _pycore.PyDoubleValue(float(json_response["price"]))

    attributes = [
        trade_id,
        maker_order_id,
        taker_order_id,
        size,
        price,
        product_id,
        sequence,
        time,
    ]

    return attributes


matches_stream_declaration = """DECLARE STREAM S2 { \n
                        EVENT BUY { trade_id:int, maker_order_id:string, taker_order_id:string, \
                        size:float, price:float, product_id:string, sequence:int, time:date } \n,
                        EVENT SELL { trade_id:int, maker_order_id:string, taker_order_id:string, \
                        size:float, price:float, product_id:string, sequence:int, time:date } \n
                        }
                        """


def create_rfq_attributes(json_response):
    maker_order_id = _pycore.PyStringValue(json_response["maker_order_id"])
    taker_order_id = _pycore.PyStringValue(json_response["taker_order_id"])
    time = _pycore.PyDateValue(int(isoparse(json_response["time"]).timestamp() * 1e9))
    trade_id = _pycore.PyIntValue(json_response["trade_id"])
    product_id = _pycore.PyStringValue(json_response["product_id"])
    size = _pycore.PyDoubleValue(float(json_response["size"]))
    price = _pycore.PyDoubleValue(float(json_response["price"]))

    attributes = [
        maker_order_id,
        taker_order_id,
        time,
        trade_id,
        product_id,
        size,
        price,
    ]

    return attributes


rfq_stream_declaration = """DECLARE STREAM S2 { \n
                        EVENT BUY { maker_order_id:string, taker_order_id:string, time:date, trade_id:int, product_id:string, \
                        size:float, price:float } \n,
                        EVENT SELL { maker_order_id:string, taker_order_id:string, time:date, trade_id:int, product_id:string, \
                        size:float, price:float } \n
                        }
                        """

## Options declaration
options = """DECLARE QUARANTINE { \n
            FIXED_TIME 2 seconds {S} \n
            }"""

# ## Queries
# query_1 = """SELECT * FROM S\n
#                 WHERE (SELL as T1; BUY as T2; BUY as T3)\n
#                 FILTER T1[product_id = 'at-USD'] AND T2[product_id = 'BTC-USD'] AND T3[product_id = 'LTC-USD'] \n
#                 WITHIN 50 EVENTS"""
#
# query_2 = """SELECT * FROM S2\n
#                 WHERE (BUY as T1)\n
#                 FILTER T1[product_id = 'BTC-USD']\n
#                 WITHIN 100 EVENTS"""


async def websocket_listener(json_input, streamer):
    subscribe_message = json.dumps(json_input)

    message_count = 0
    start_time = time.time()

    event_dict = {"buy": 0, "sell": 1}
    stream_dict = {"ticker": 0, "match": 1}

    while True:
        try:
            async with websockets.connect(URI, ping_interval=None) as websocket:
                await websocket.send(subscribe_message)
                not_first_message = False
                while True:
                    response = await websocket.recv()
                    json_response = json.loads(response)
                    if not_first_message:
                        if json_response["type"] == "ticker":
                            attributes = create_ticker_attributes(json_response)
                            stream_id = stream_dict.get((json_response["type"]).lower())
                            event_type_id = event_dict.get(
                                (json_response["side"]).lower()
                            )
                            event = _pycore.PyEvent(event_type_id, attributes)
                            streamer.send_stream(stream_id, event)
                        elif json_response["type"] == "match":
                            attributes = create_matches_attributes(json_response)
                            stream_id = stream_dict.get((json_response["type"]).lower())
                            event_type_id = (
                                event_dict.get((json_response["side"]).lower()) + 2
                            )
                            event = _pycore.PyEvent(event_type_id, attributes)
                            print("Sending event")
                            streamer.send_stream(stream_id, event)

                    not_first_message = True

                    message_count += 1
                    elapsed_time = time.time() - start_time
                    if elapsed_time >= 1:
                        messages_per_second = message_count / elapsed_time
                        # print(f"Messages per second: {messages_per_second:.2f}")
                        message_count = 0
                        start_time = time.time()

        except (
            websockets.exceptions.ConnectionClosedError,
            websockets.exceptions.ConnectionClosedOK,
        ):
            print("Connection closed, retrying..")
            await asyncio.sleep(1)


# Opción de tener mas de una subscripción
async def main(streamer):
    tasks = [websocket_listener(ticker, streamer)]
    # tasks = [websocket_listener(rfq_matches, streamer)]
    # tasks.append(websocket_listener(rfq_matches, streamer))
    tasks.append(websocket_listener(matches, streamer))
    await asyncio.gather(*tasks)


if __name__ == "__main__":
    try:
        # Aca van las configuraciones iniciales
        server_port = 5000

        client = _pycore.PyClient("tcp://localhost", server_port)

        stream_info = client.declare_stream(ticker_stream_declaration)

        print(f"Stream id: {stream_info.id}, Nombre: {stream_info.name}")
        for event in stream_info.events_info:
            print(event.name, event.id)

        second_stream_info = client.declare_stream(matches_stream_declaration)

        print(f"Stream id: {second_stream_info.id}, Nombre: {second_stream_info.name}")
        for event in second_stream_info.events_info:
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

        streamer_port = 5001
        streamer = _pycore.PyStreamer("tcp://localhost", streamer_port)

        # Comienza la conexión
        start_time_total = time.time()
        asyncio.run(main(streamer))
        print(f"Total time: {time.time() - start_time_total}")

    except KeyboardInterrupt:
        print("Exiting WebSocket..")
        print(f"Total time: {time.time() - start_time_total}")

    except:
        print("Exited unexpectedly..")
        print(f"Total time: {time.time() - start_time_total}")

