import datetime
from typing import List, Literal

from pydantic import BaseModel, Field

# class PostRecordModel(BaseModel):
#     langs: List[str]
#     text: str
#
#
# class CreatePostCommitModel(BaseModel):
#     operation: Literal["create"]
#     collection: Literal["app.bsky.feed.post"]
#     record: PostRecordModel
#
#
# class EventCommitCreatePostModel(BaseModel):
#     kind: Literal["commit"]
#     time_us: int
#     commit: CreatePostCommitModel


class RecordSubjectModel(BaseModel):
    cid: str
    uri: str


class PostRecordModel(BaseModel):
    record_type: Literal["app.bsky.feed.post"] = Field(alias="$type")
    createdAt: datetime.datetime
    langs: List[str]
    text: str


class FollowRecordModel(BaseModel):
    record_type: Literal["app.bsky.graph.follow"] = Field(alias="$type")
    createdAt: datetime.datetime
    subject: str


class LikeRecordModel(BaseModel):
    record_type: Literal["app.bsky.feed.like"] = Field(alias="$type")
    createdAt: datetime.datetime
    subject: RecordSubjectModel


class RepostRecordModel(BaseModel):
    record_type: Literal["app.bsky.feed.repost"] = Field(alias="$type")
    createdAt: datetime.datetime
    subject: RecordSubjectModel


class CommitEventModel(BaseModel):
    cid: str
    operation: Literal["create"]
    record: (
        PostRecordModel | FollowRecordModel | LikeRecordModel | RepostRecordModel
    ) = Field(discriminator="record_type")


class CommitWrapperEventModel(BaseModel):
    did: str
    kind: Literal["commit"]
    time_us: int
    commit: CommitEventModel
    received_time: datetime.datetime = Field(
        default_factory=lambda: datetime.datetime.now(datetime.timezone.utc)
    )
