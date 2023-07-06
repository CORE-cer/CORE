#pragma once

#include "core_server/internal/ceql/value/all_value_headers.hpp"
#include "value_visitor.hpp"

namespace CORE::Internal::CEQL {
class DetermineValueType : public ValueVisitor {
 private:
  ValueTypes value_type;

 public:
  ValueTypes get_value_type() { return value_type; }

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
