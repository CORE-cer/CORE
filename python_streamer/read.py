# event_type,product_id,price,open24h,volume_24h,low_24h,high_24h,volume_30d,best_bid,best_bid_size,best_ask,best_ask_size,last_size,time

from coinbase.models.ticker import TickerModel


def get_ticker_attributes(model: TickerModel) -> list:
    event_type = model.side.lower().capitalize()
    product_id = model.product_id
    price = model.price
    open_24h = model.open_24h
    volume_24h = model.volume_24h
    low_24h = model.low_24h
    high_24h = model.high_24h
    volume_30d = model.volume_30d
    best_bid = model.best_bid
    best_bid_size = model.best_bid_size
    best_ask = model.best_ask
    best_ask_size = model.best_ask_size
    last_size = model.last_size
    time = int(model.time.timestamp() * 1e9)  # Convert to nanoseconds

    return [
        event_type,
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


inputs = []
with open("test.txt") as f:
    lines = f.read().splitlines()
    for line in lines:
        if line[0] != "P":
            continue
        a = line.find("{")
        b = line.find("}") + 1
        model = TickerModel.model_validate_json(line[a:b])
        attributes = get_ticker_attributes(model)
        attrs = ",".join(map(lambda x: str(x), attributes))
        inputs.append(attrs)

with open("ticker.txt", "w") as f:
    for line in inputs:
        f.write(line + "\n")
