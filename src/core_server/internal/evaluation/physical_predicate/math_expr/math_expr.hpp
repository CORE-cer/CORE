#pragma once

#include <memory>
#include <string>

#include "core_server/internal/stream/ring_tuple_queue/tuple.hpp"

namespace CORE {
namespace Internal {
namespace CEA {

template <typename Type>
class MathExpr {
 public:
  virtual std::unique_ptr<MathExpr<Type>> clone() const = 0;
  virtual ~MathExpr() = default;
  virtual Type eval(RingTupleQueue::Tuple&) = 0;
  virtual std::string to_string() const = 0;
};
}  // namespace CEA
}  // namespace Internal
}  // namespace CORE
