#include <memory>

#include "and_filter.hpp"
#include "or_filter.hpp"

namespace CORE::Internal::CEQL {
std::unique_ptr<Filter> AndFilter::negate() const {
  return std::make_unique<OrFilter>(left->negate(), right->negate());
}
}  // namespace CORE::Internal::CEQL