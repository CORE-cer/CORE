#pragma once

#include <stdexcept>

namespace InternalCORECEQL {

class AndFilter;
class AtomicFilter;
class OrFilter;

class FilterVisitor {
 public:
  virtual ~FilterVisitor() = default;

  // clang-format off
  virtual void visit(AndFilter&)    { throw std::logic_error("visit AndFilter not implemented"); }
  virtual void visit(AtomicFilter&) { throw std::logic_error("visit AtomicFilter not implemented"); }
  virtual void visit(OrFilter&)     { throw std::logic_error("visit OrFilter not implemented"); }

  // clang-format on
};
}  // namespace InternalCORECEQL
