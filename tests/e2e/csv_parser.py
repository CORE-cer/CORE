"""Parse CSV/corecsv files into PyEvent objects using stream schema."""

from pathlib import Path

import pycer


def parse_csv(
    csv_path: str | Path, stream_info: pycer.PyStreamInfo
) -> tuple[list[pycer.PyEvent], list[int]]:
    """Parse a CSV file into PyEvent objects based on stream schema.

    The CSV must have a header row with the first column being 'event_type'.
    Remaining columns correspond to the event's attributes as defined in the
    stream declaration.

    Lines containing only a number (no commas) are inter-event timing delays
    in nanoseconds (common in .corecsv files).  These are collected and
    returned alongside the events so the caller can replay them faithfully.

    Returns (events, delays_ns) where delays_ns[i] is the nanosecond delay
    to apply *before* sending events[i].
    """
    event_lookup: dict[str, tuple[int, list[pycer.PyAttributeInfo]]] = {}
    for event_info in stream_info.events_info:
        event_lookup[event_info.name] = (event_info.id, event_info.attributes_info)

    events: list[pycer.PyEvent] = []
    delays_ns: list[int] = []
    pending_delay = 0

    with open(csv_path, newline="") as f:
        first_line = True
        for raw_line in f:
            stripped = raw_line.strip()
            if not stripped:
                continue
            if first_line:
                first_line = False
                continue
            # Timing line: no commas, just a number (nanoseconds)
            if "," not in stripped:
                pending_delay = int(stripped)
                continue

            delays_ns.append(pending_delay)
            pending_delay = 0

            parts = stripped.split(",")
            event_type_name = parts[0]

            if event_type_name not in event_lookup:
                raise ValueError(f"Unknown event type '{event_type_name}' in CSV")

            event_id, attrs = event_lookup[event_type_name]
            values: list[pycer.PyValue] = []
            primary_time = None

            for i, attr in enumerate(attrs):
                raw_val = parts[i + 1]
                vtype = attr.value_type

                if vtype == pycer.INT64:
                    values.append(pycer.PyIntValue(int(raw_val)))
                elif vtype == pycer.DOUBLE:
                    values.append(pycer.PyDoubleValue(float(raw_val)))
                elif vtype == pycer.STRING_VIEW:
                    values.append(pycer.PyStringValue(raw_val))
                elif vtype == pycer.BOOL:
                    values.append(pycer.PyBoolValue(raw_val.lower() == "true"))
                elif vtype == pycer.PRIMARY_TIME:
                    int_val = int(raw_val)
                    values.append(pycer.PyIntValue(int_val))
                    primary_time = pycer.PyIntValue(int_val)
                else:
                    raise ValueError(f"Unsupported value type: {vtype}")

            if primary_time is not None:
                events.append(pycer.PyEvent(event_id, values, primary_time))
            else:
                events.append(pycer.PyEvent(event_id, values))

    return events, delays_ns
