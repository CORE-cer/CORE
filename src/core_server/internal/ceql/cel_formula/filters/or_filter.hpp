#pragma once

#include <memory>
#include <string>

#include "filter.hpp"

namespace CORE::Internal::CEQL {

struct OrFilter : public Filter {
  std::unique_ptr<Filter> left;
  std::unique_ptr<Filter> right;

  OrFilter(std::unique_ptr<Filter>&& left, std::unique_ptr<Filter>&& right)
      : left(std::move(left)), right(std::move(right)) {}

  ~OrFilter() override = default;

  std::unique_ptr<Filter> clone() const override {
    return std::make_unique<OrFilter>(left->clone(), right->clone());
  }

  std::unique_ptr<Filter> negate() const override;

  bool operator==(const OrFilter& other) const {
    return left->equals(other.left.get()) && right->equals(other.right.get());
  }

  bool equals(Filter* other) const override {
    if (auto other_filter = dynamic_cast<OrFilter*>(other)) {
      return *this == *other_filter;
    }
    return false;
  }

  void accept_visitor(FilterVisitor& visitor) override { visitor.visit(*this); }

  std::string to_string() const override {
    return left->to_string() + " OR " + right->to_string();
  }
};
}  // namespace CORE::Internal::CEQL
