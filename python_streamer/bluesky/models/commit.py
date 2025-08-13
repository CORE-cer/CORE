from typing import List, Literal

from pydantic import BaseModel


class PostRecordModel(BaseModel):
    langs: List[str]
    text: str


class CreatePostCommitModel(BaseModel):
    operation: Literal["create"]
    collection: Literal["app.bsky.feed.post"]
    record: PostRecordModel


class EventCommitCreatePostModel(BaseModel):
    kind: Literal["commit"]
    time_us: int
    commit: CreatePostCommitModel
