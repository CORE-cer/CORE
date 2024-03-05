#pragma once
#include <stdint.h>

namespace CORE::Types {
// Unique id for each unique name. Events can share names, therefore NameEventTypeId can be shared between distinct events when they share their name.
using EventNameTypeId = uint64_t;
using UniqueEventTypeId = uint64_t;
}  // namespace CORE::Types
