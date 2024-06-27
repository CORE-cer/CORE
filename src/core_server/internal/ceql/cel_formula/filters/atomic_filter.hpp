#pragma once

#include <memory>
#include <optional>
#include <string>
#include <utility>

#include "core_server/internal/ceql/cel_formula/filters/visitors/filter_visitor.hpp"
#include "core_server/internal/ceql/cel_formula/predicate/predicate.hpp"
#include "filter.hpp"

namespace CORE::Internal::CEQL {
/**
 * The atomic filter is the filter of type X[P]
 */
struct AtomicFilter : public Filter {
  std::optional<std::string> stream_name;
  std::string variable_name;
  std::unique_ptr<Predicate> predicate;

  AtomicFilter(std::string variable_name, std::unique_ptr<Predicate>&& predicate)
      : variable_name(variable_name), predicate(std::move(predicate)) {}

  AtomicFilter(std::optional<std::string> stream_name,
               std::string variable_name,
               std::unique_ptr<Predicate>&& predicate)
      : stream_name(stream_name),
        variable_name(variable_name),
        predicate(std::move(predicate)) {}

  ~AtomicFilter() override = default;

  std::unique_ptr<Filter> clone() const override {
    return std::make_unique<AtomicFilter>(stream_name, variable_name, predicate->clone());
  }

  std::unique_ptr<Filter> negate() const override {
    return std::make_unique<AtomicFilter>(stream_name, variable_name, predicate->negate());
  }

  bool operator==(const AtomicFilter& other) const {
    return stream_name == other.stream_name && variable_name == other.variable_name
           && predicate->equals(other.predicate.get());
  }

  bool equals(Filter* other) const override {
    if (auto other_filter = dynamic_cast<AtomicFilter*>(other)) {
      return *this == *other_filter;
    }
    return false;
  }

  void accept_visitor(FilterVisitor& visitor) override { visitor.visit(*this); }

  std::string to_string() const override {
    if (stream_name.has_value()) {
      return stream_name.value() + "->" + variable_name + "[" + predicate->to_string()
             + "]";
    }
    return variable_name + "[" + predicate->to_string() + "]";
  }
};
}  // namespace CORE::Internal::CEQL
