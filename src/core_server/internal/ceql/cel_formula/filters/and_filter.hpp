#pragma once

#include <memory>

#include "filter.hpp"

namespace InternalCORECEQL {
class AndFilter : public Filter {
 private:
  std::unique_ptr<Filter> left;
  std::unique_ptr<Filter> right;

 public:
  AndFilter(std::unique_ptr<Filter>&& left,
            std::unique_ptr<Filter>&& right)
      : left(std::move(left)), right(std::move(right)) {}

  ~AndFilter() override = default;

  std::unique_ptr<Filter> clone() const override {
    return std::make_unique<AndFilter>(left->clone(), right->clone());
  }

  bool operator==(const AndFilter& other) const {
    return left->equals(other.left.get()) &&
           right->equals(other.right.get());
  }

  bool equals(Filter* other) const override {
    if (auto other_filter = dynamic_cast<AndFilter*>(other)) {
      return *this == *other_filter;
    }
    return false;
  }

  void accept_visitor(FilterVisitor& visitor) override {
    visitor.visit(*this);
  }

  std::string to_string() const override {
    return left->to_string() + " AND " + right->to_string();
  }
};
}  // namespace InternalCORECEQL
