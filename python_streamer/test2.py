import asyncio

import websockets

url = "wss://jetstream2.us-west.bsky.network/subscribe"


async def receive_messages():
    async with websockets.connect(url) as websocket:
        while True:
            async for message in websocket:
                print("Received message:", message)


asyncio.run(receive_messages())
