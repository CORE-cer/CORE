import asyncio

import _pycore
from bluesky.create_post import CreatePostStreamer
from coinbase.ticker import TickerStreamer
from coinbase.ticker_offline import TickerStreamerOffline


async def do():
    client = _pycore.PyClient("tcp://localhost", 5001)

    streamer = _pycore.PyStreamer("tcp://localhost", 5002)

    # ticker_streamer = TickerStreamer(client, streamer)
    # ticker_streamer = TickerStreamerOffline(client, streamer)

    bluesky_post_streamer = CreatePostStreamer(client, streamer)

    tasks = []
    # tasks.append(ticker_streamer.start())
    tasks.append(bluesky_post_streamer.start())
    await asyncio.gather(*tasks)


asyncio.run(do())
