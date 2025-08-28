import asyncio

import _pycore
from bluesky.create_post import CreatePostStreamer
from bluesky.create_post_offline import CreatePostStreamerOffline
from coinbase.ticker import TickerStreamer


async def do():
    client = _pycore.PyClient("tcp://localhost", 5000)

    streamer = _pycore.PyStreamer("tcp://localhost", 5001)

    ticker_streamer = TickerStreamer(client, streamer)
    # ticker_streamer = TickerStreamerOffline(client, streamer)

    bluesky_post_streamer = CreatePostStreamer(client, streamer)
    # bluesky_post_streamer = CreatePostStreamerOffline(client, streamer)

    tasks = []
    tasks.append(ticker_streamer.start())
    tasks.append(bluesky_post_streamer.start())
    await asyncio.gather(*tasks)


asyncio.run(do())
