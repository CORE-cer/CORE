#pragma once

#include <memory>

#include "filter.hpp"

namespace InternalCORECEQL {
class OrFilter : public Filter {
 private:
  std::unique_ptr<Filter> left;
  std::unique_ptr<Filter> right;

 public:
  OrFilter(std::unique_ptr<Filter>&& left, std::unique_ptr<Filter>&& right)
      : left(std::move(left)), right(std::move(right)) {}

  ~OrFilter() override = default;

  std::unique_ptr<Filter> clone() const override {
    return std::make_unique<OrFilter>(left->clone(), right->clone());
  }

  void accept_visitor(FilterVisitor& visitor) override {
    visitor.visit(*this);
  }

  std::string to_string() const override {
    return left->to_string() + " OR " + right->to_string();
  }
};
}  // namespace InternalCORECEQL
