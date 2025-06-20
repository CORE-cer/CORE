import asyncio

import _pycore
from coinbase.ticker import TickerStreamer


async def do():
    client = _pycore.PyClient("tcp://localhost", 5001)

    streamer = _pycore.PyStreamer("tcp://localhost", 5002)

    ticker_streamer = TickerStreamer(client, streamer)

    tasks = []
    tasks.append(ticker_streamer.start())
    await asyncio.gather(*tasks)


def do2():
    asyncio.run(do())


do2()
