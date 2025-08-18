#pragma once

#include <optional>
#include <stdexcept>
#include "core_server/internal/ceql/value/attribute.hpp"
#include "core_server/internal/ceql/value/boolean_literal.hpp"
#include "core_server/internal/ceql/value/double_literal.hpp"
#include "core_server/internal/ceql/value/integer_literal.hpp"
#include "core_server/internal/ceql/value/long_literal.hpp"
#include "core_server/internal/ceql/value/operations/addition.hpp"
#include "core_server/internal/ceql/value/operations/division.hpp"
#include "core_server/internal/ceql/value/operations/modulo.hpp"
#include "core_server/internal/ceql/value/operations/multiplication.hpp"
#include "core_server/internal/ceql/value/operations/negation.hpp"
#include "core_server/internal/ceql/value/operations/subtraction.hpp"
#include "core_server/internal/ceql/value/regex_literal.hpp"
#include "core_server/internal/ceql/value/sequence.hpp"
#include "core_server/internal/ceql/value/string_literal.hpp"
#include "core_server/internal/ceql/value/undefined.hpp"
#include "core_server/internal/ceql/value/value_types.hpp"
#include "value_visitor.hpp"

namespace CORE::Internal::CEQL {
class DetermineValueType : public ValueVisitor {
 private:
  std::optional<ValueTypes> value_type;

 public:
  ValueTypes get_value_type() { if (!value_type.has_value()){
    throw std::logic_error("Value type has not been set yet.");
  }
    return value_type.value();
  }

  // clang-format off
  void visit(Attribute&)      override {value_type = ValueTypes::Attribute;}
  void visit(BooleanLiteral&) override {value_type = ValueTypes::BooleanLiteral;}
  void visit(DoubleLiteral&)  override {value_type = ValueTypes::DoubleLiteral;}
  void visit(IntegerLiteral&) override {value_type = ValueTypes::IntegerLiteral;}
  void visit(LongLiteral&)    override {value_type = ValueTypes::LongLiteral;}
  void visit(StringLiteral&)  override {value_type = ValueTypes::StringLiteral;}
  void visit(RegexLiteral&)   override {value_type = ValueTypes::RegexLiteral;}
  void visit(Sequence&)       override {value_type = ValueTypes::Sequence;}
  void visit(Undefined&)      override {value_type = ValueTypes::Undefined;}
  void visit(Addition&)       override {value_type = ValueTypes::Addition;}
  void visit(Division&)       override {value_type = ValueTypes::Division;}
  void visit(Modulo&)         override {value_type = ValueTypes::Modulo;}
  void visit(Multiplication&) override {value_type = ValueTypes::Multiplication;}
  void visit(Negation&)       override {value_type = ValueTypes::Negation;}
  void visit(Subtraction&)    override {value_type = ValueTypes::Subtraction;}

  // clang-format on
};
}  // namespace CORE::Internal::CEQL
