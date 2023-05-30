#pragma once

#include <memory>
#include <string>

#include "core_server/internal/ceql/value/value.hpp"
#include "predicate.hpp"

namespace InternalCORECEQL {

class InequalityPredicate : public Predicate {
  enum class LogicalOperation {
    EQUALS,
    GREATER,
    GREATER_EQUALS,
    IN,
    LESS_EQUALS,
    LESS,
    NOT_EQUALS,
    NOT_IN,
  };  // TODO: Probably remove things like: like.

 private:
  std::unique_ptr<Value> left;
  LogicalOperation logical_op;
  std::unique_ptr<Value> right;

 public:
  InequalityPredicate(std::unique_ptr<Value>&& left,
                      LogicalOperation logical_op,
                      std::unique_ptr<Value>&& right)
      : left(std::move(left)),
        logical_op(logical_op),
        right(std::move(right)) {}

  std::unique_ptr<Predicate> clone() const override {
    return std::make_unique<InequalityPredicate>(left->clone(), logical_op,
                                                 right->clone());
  }

  ~InequalityPredicate();

  bool is_constant() const override {
    return false;  // TODO, maybe a value visitor.
  }

  std::unique_ptr<Predicate> negate() const override {
    return std::make_unique<InequalityPredicate>(
        left->clone(), negate(logical_op), right->clone());
  }

  std::string to_string() const override {
    return left->to_string() + " " + to_string(logical_op) + " " +
           left->to_string();
  }

  void accept_visitor(PredicateVisitor& visitor) override {
    visitor.visit(*this);
  }

  LogicalOperation get_logical_op() const { return logical_op; }

  const std::unique_ptr<Value>& get_left() const { return left; }

  const std::unique_ptr<Value>& get_right() const { return right; }

  static LogicalOperation negate(LogicalOperation op) {
    switch (op) {
      case LogicalOperation::EQUALS:
        return LogicalOperation::NOT_EQUALS;
      case LogicalOperation::GREATER:
        return LogicalOperation::LESS_EQUALS;
      case LogicalOperation::GREATER_EQUALS:
        return LogicalOperation::LESS;
      case LogicalOperation::IN:
        return LogicalOperation::NOT_IN;
      case LogicalOperation::LESS_EQUALS:
        return LogicalOperation::GREATER;
      case LogicalOperation::LESS:
        return LogicalOperation::GREATER_EQUALS;
      case LogicalOperation::NOT_EQUALS:
        return LogicalOperation::EQUALS;
      case LogicalOperation::NOT_IN:
        return LogicalOperation::IN;
    }
  }

  static std::string to_string(LogicalOperation op) {
    switch (op) {
      case LogicalOperation::EQUALS:
        return "==";
      case LogicalOperation::GREATER:
        return ">";
      case LogicalOperation::GREATER_EQUALS:
        return ">= EQUALS";
      case LogicalOperation::IN:
        return "In";
      case LogicalOperation::LESS_EQUALS:
        return "<=";
      case LogicalOperation::LESS:
        return "<";
      case LogicalOperation::NOT_EQUALS:
        return "!=";
      case LogicalOperation::NOT_IN:
        return "Not In";
    }
  }
};
}  // namespace InternalCORECEQL
