#pragma once

#include <memory>

#include "visitors/filter_visitor.hpp"

namespace InternalCORECEQL {
class Filter {
 public:
  virtual ~Filter() = default;
  virtual std::unique_ptr<Filter> clone() const = 0;
  virtual void accept_visitor(FilterVisitor&) = 0;
  virtual std::string to_string() const = 0;
};
}  // namespace InternalCORECEQL