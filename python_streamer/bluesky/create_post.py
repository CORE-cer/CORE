import json
from typing import Optional, final

import aiohttp
from aiolimiter import AsyncLimiter
from async_lru import alru_cache

import _pycore
from abstract_streamer_websocket import AbstractStreamerWebsocket
from bluesky.models.commit import CommitWrapperEventModel

rate_limit = AsyncLimiter(100, 10)


@alru_cache(maxsize=None)
async def get_user_handle(did: str):
    url = f"https://bsky.social/xrpc/com.atproto.repo.describeRepo?repo={did}"

    user_handle: str

    async with rate_limit:
        async with aiohttp.ClientSession() as session:
            async with session.get(url) as response:
                if response.status == 200:
                    data = await response.json()
                    user_handle = data.get("handle", "unknown")
                else:
                    print(response.status, await response.text())
                    print(response.headers)
                    user_handle = "unknown"

    return user_handle


@final
class CreatePostStreamer(AbstractStreamerWebsocket[CommitWrapperEventModel]):
    # N_CACHED = 200_000
    # cached_models: List[CommitWrapperEventModel] = []
    # pbar: tqdm = tqdm(total=N_CACHED)

    def __init__(self, *args, **kwargs):
        super().__init__(*args, **kwargs)

    @property
    def name(self) -> str:
        return "CreatePostStreamer"

    @property
    def stream_declaration(self) -> str:
        return """
        CREATE STREAM Bluesky {
            EVENT CreatePost {
                did: string,
                kind: string,
                time: primary_time,
                time_str: string,
                cid: string,
                operation: string,
                record_type: string,
                user_handle: string,
                langs: string,
                text: string
            },
            EVENT CreateFollow {
                did: string,
                kind: string,
                time: primary_time,
                time_str: string,
                cid: string,
                operation: string,
                record_type: string,
                user_handle: string,
                subject: string
            },
            EVENT CreateLike {
                did: string,
                kind: string,
                time: primary_time,
                time_str: string,
                cid: string,
                operation: string,
                record_type: string,
                user_handle: string,
                subject_cid: string,
                subject_uri: string
            },
            EVENT CreateRepost {
                did: string,
                kind: string,
                time: primary_time,
                time_str: string,
                cid: string,
                operation: string,
                record_type: string,
                user_handle: string,
                subject_cid: string,
                subject_uri: string
            }
        }
        """

    @property
    def option_declaration(self) -> Optional[str]:
        return """
                    CREATE QUARANTINE 
                    { \n
                    BOUNDED_TIME 10 seconds {Bluesky} \n
                    }
                    """

    @property
    def URI(self) -> str:
        return "wss://jetstream2.us-west.bsky.network/subscribe"

    @property
    def subscribe_message_json(self) -> str:
        return ""

    def parse_message_json(self, message: str) -> Optional[CommitWrapperEventModel]:
        try:
            message_json = json.loads(message)
            model = CommitWrapperEventModel.model_validate(message_json)
            return model
        except Exception:
            return None

    def get_event_id_from_model(self, model: CommitWrapperEventModel) -> int:
        event_dict = {
            "app.bsky.feed.post": "CreatePost",
            "app.bsky.graph.follow": "CreateFollow",
            "app.bsky.feed.like": "CreateLike",
            "app.bsky.feed.repost": "CreateRepost",
        }

        event_name = event_dict.get(model.commit.record.record_type)

        assert (
            event_name is not None
        ), f"Unknown record type: {model.commit.record.record_type}"

        event_id = self.event_name_to_unique_id.get(event_name)

        assert event_id is not None, f"Unique ID for event {event_name} not found"

        return event_id

    def common_event_attributes(self, model: CommitWrapperEventModel):
        did = _pycore.PyStringValue(model.did)
        kind = _pycore.PyStringValue(model.kind)
        time = _pycore.PyIntValue(int(model.commit.record.createdAt.timestamp() * 1e9))
        time_str = _pycore.PyStringValue(model.commit.record.createdAt.__str__())
        cid = _pycore.PyStringValue(model.commit.cid)
        operation = _pycore.PyStringValue(model.commit.operation)
        record_type = _pycore.PyStringValue(model.commit.record.record_type)

        attributes = [did, kind, time, time_str, cid, operation, record_type]

        return attributes

    def record_subject_attribute(self, model: CommitWrapperEventModel):
        assert (
            model.commit.record.record_type == "app.bsky.feed.like"
            or model.commit.record.record_type == "app.bsky.feed.repost"
        )
        subject_cid = _pycore.PyStringValue(model.commit.record.subject.cid)
        subject_uri = _pycore.PyStringValue(model.commit.record.subject.uri)

        attributes = [subject_cid, subject_uri]

        return attributes

    def event_post_attributes(self, model: CommitWrapperEventModel):
        assert model.commit.record.record_type == "app.bsky.feed.post"
        langs = _pycore.PyStringValue(", ".join(model.commit.record.langs))
        text = _pycore.PyStringValue(model.commit.record.text)

        attributes = [langs, text]

        return attributes

    def event_follow_attributes(self, model: CommitWrapperEventModel):
        assert model.commit.record.record_type == "app.bsky.graph.follow"
        subject = _pycore.PyStringValue(model.commit.record.subject)

        attributes = [subject]

        return attributes

    def event_like_attribute(self, model: CommitWrapperEventModel):
        assert model.commit.record.record_type == "app.bsky.feed.like"

        attributes = []

        subject_attributes = self.record_subject_attribute(model)

        attributes.extend(subject_attributes)

        return attributes

    def event_repost_attribute(self, model: CommitWrapperEventModel):
        assert model.commit.record.record_type == "app.bsky.feed.repost"

        attributes = []

        subject_attributes = self.record_subject_attribute(model)

        attributes.extend(subject_attributes)

        return attributes

    def get_event_attributes(self, model: CommitWrapperEventModel):
        switcher = {
            "app.bsky.feed.post": self.event_post_attributes,
            "app.bsky.graph.follow": self.event_follow_attributes,
            "app.bsky.feed.like": self.event_like_attribute,
            "app.bsky.feed.repost": self.event_repost_attribute,
        }
        attributes = switcher.get(model.commit.record.record_type)

        assert (
            attributes is not None
        ), f"unknown record type: {model.commit.record.record_type}"

        return attributes(model)

    async def get_user_handle_attribute(self, did: str):
        user_handle = await get_user_handle(did)
        print(get_user_handle.cache_info())
        # user_handle = "unknown"
        print("User handle for DID", did, "is", user_handle)
        return _pycore.PyStringValue(user_handle)

    async def create_event(self, model: CommitWrapperEventModel):
        # print("Creating event for model:", model)
        attributes = self.common_event_attributes(model)

        user_handle = await self.get_user_handle_attribute(model.did)

        attributes.append(user_handle)

        event_attributes = self.get_event_attributes(model)

        attributes.extend(event_attributes)

        event_id = self.get_event_id_from_model(model)

        time = _pycore.PyIntValue(int(model.commit.record.createdAt.timestamp() * 1e9))

        event = _pycore.PyEvent(event_id, attributes, time)

        # self.cached_models.append(model)

        # self.pbar.update(1)
        # if len(self.cached_models) % self.N_CACHED == 0:
        #     print("Clearing cached models")
        #     model_jsons = [
        #         cached_model.model_dump_json() for cached_model in self.cached_models
        #     ]
        #     with open("bluesky_cached_models.json", "w") as f:
        #         f.write("[\n")
        #         f.write(",\n".join(model_jsons))
        #         f.write("\n]")
        #     self.pbar.reset()

        return event
