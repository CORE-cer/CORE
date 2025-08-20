import json
from typing import Optional, final

import _pycore
from abstract_streamer_websocket import AbstractStreamerWebsocket
from bluesky.models.commit import CommitWrapperEventModel


@final
class CreatePostStreamer(AbstractStreamerWebsocket[CommitWrapperEventModel]):
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
                cid: string,
                operation: string,
                record_type: string,
                langs: string,
                text: string
            },
            EVENT CreateFollow {
                did: string,
                kind: string,
                time: primary_time,
                cid: string,
                operation: string,
                record_type: string,
                subject: string
            },
            EVENT CreateLike {
                did: string,
                kind: string,
                time: primary_time,
                cid: string,
                operation: string,
                record_type: string,
                subject_cid: string,
                subject_uri: string
            },
            EVENT CreateRepost {
                did: string,
                kind: string,
                time: primary_time,
                cid: string,
                operation: string,
                record_type: string,
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
                    FIXED_TIME 20 seconds {Bluesky} \n
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
            "app.bsky.feed.post": 0,
            "app.bsky.graph.follow": 1,
            "app.bsky.feed.like": 2,
            "app.bsky.feed.repost": 3,
        }

        event_id = event_dict.get(model.commit.record.record_type)

        assert (
            event_id is not None
        ), f"Unknown record type: {model.commit.record.record_type}"

        return event_id

    def common_event_attributes(self, model: CommitWrapperEventModel):
        did = _pycore.PyStringValue(model.did)
        kind = _pycore.PyStringValue(model.kind)
        time = _pycore.PyIntValue(int(model.commit.record.createdAt.timestamp() * 1e9))
        cid = _pycore.PyStringValue(model.commit.cid)
        operation = _pycore.PyStringValue(model.commit.operation)
        record_type = _pycore.PyStringValue(model.commit.record.record_type)

        attributes = [did, kind, time, cid, operation, record_type]

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

    def create_event(self, model: CommitWrapperEventModel):
        attributes = self.common_event_attributes(model)

        event_attributes = self.get_event_attributes(model)

        attributes.extend(event_attributes)

        event_id = self.get_event_id_from_model(model)

        time = _pycore.PyIntValue(int(model.commit.record.createdAt.timestamp() * 1e9))

        event = _pycore.PyEvent(event_id, attributes, time)

        return event
