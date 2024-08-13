#pragma once

#include <cassert>
#include <memory>
#include <string>
#include <utility>

#include "core_server/internal/ceql/cel_formula/predicate/visitors/predicate_visitor.hpp"
#include "core_server/internal/ceql/value/value.hpp"
#include "predicate.hpp"

namespace CORE::Internal::CEQL {

struct InequalityPredicate : public Predicate {
  enum struct LogicalOperation {
    EQUALS,
    GREATER,
    GREATER_EQUALS,
    LESS_EQUALS,
    LESS,
    NOT_EQUALS,
  };  // TODO: Probably remove things like: like.

  std::unique_ptr<Value> left;
  LogicalOperation logical_op;
  std::unique_ptr<Value> right;

  InequalityPredicate(std::unique_ptr<Value>&& left,
                      LogicalOperation logical_op,
                      std::unique_ptr<Value>&& right)
      : left(std::move(left)), logical_op(logical_op), right(std::move(right)) {}

  ~InequalityPredicate() override = default;

  std::unique_ptr<Predicate> clone() const override {
    return std::make_unique<InequalityPredicate>(left->clone(), logical_op, right->clone());
  }

  bool is_constant() const override {
    return false;  // TODO, maybe a value visitor.
  }

  std::unique_ptr<Predicate> negate() const override {
    return std::make_unique<InequalityPredicate>(left->clone(),
                                                 negate(logical_op),
                                                 right->clone());
  }

  bool operator==(const InequalityPredicate& other) const {
    assert(other.left && other.right);
    return left->equals(other.left.get()) && logical_op == other.logical_op
           && right->equals(other.right.get());
  }

  bool equals(Predicate* other) const override {
    if (auto other_predicate = dynamic_cast<InequalityPredicate*>(other)) {
      return *this == *other_predicate;
    }
    return false;
  }

  std::string to_string() const override {
    return left->to_string() + " " + to_string(logical_op) + " " + right->to_string();
  }

  void accept_visitor(PredicateVisitor& visitor) override { visitor.visit(*this); }

 private:
  static LogicalOperation negate(LogicalOperation op) {
    switch (op) {
      case LogicalOperation::EQUALS:
        return LogicalOperation::NOT_EQUALS;
      case LogicalOperation::GREATER:
        return LogicalOperation::LESS_EQUALS;
      case LogicalOperation::GREATER_EQUALS:
        return LogicalOperation::LESS;
      case LogicalOperation::LESS_EQUALS:
        return LogicalOperation::GREATER;
      case LogicalOperation::LESS:
        return LogicalOperation::GREATER_EQUALS;
      case LogicalOperation::NOT_EQUALS:
        return LogicalOperation::EQUALS;
      default:
        assert(false && "this switch should cover all LogicalOperations.");
        return {};
    }
  }

  static std::string to_string(LogicalOperation op) {
    switch (op) {
      case LogicalOperation::EQUALS:
        return "==";
      case LogicalOperation::GREATER:
        return ">";
      case LogicalOperation::GREATER_EQUALS:
        return ">=";
      case LogicalOperation::LESS_EQUALS:
        return "<=";
      case LogicalOperation::LESS:
        return "<";
      case LogicalOperation::NOT_EQUALS:
        return "!=";
      default:
        assert(false && "this switch should cover all LogicalOperations.");
        return {};
    }
  }
};
}  // namespace CORE::Internal::CEQL
