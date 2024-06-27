#include "or_filter.hpp"

#include <memory>

#include "and_filter.hpp"

namespace CORE::Internal::CEQL {
std::unique_ptr<Filter> OrFilter::negate() const {
  return std::make_unique<AndFilter>(left->negate(), right->negate());
}
}  // namespace CORE::Internal::CEQL
