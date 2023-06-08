#pragma once

namespace InternalCORECEQL {

enum struct ValueTypes {
  Undefined,
  Attribute,
  BooleanLiteral,
  DoubleLiteral,
  IntegerLiteral,
  LongLiteral,
  StringLiteral,
  RegexLiteral,
  Sequence,
  Negation,
  Addition,
  Subtraction,
  Multiplication,
  Division,
  Modulo,
};

struct ValueTypeHelpers {
  static bool is_composite_math_expr(ValueTypes type) {
    switch (type) {
      case ValueTypes::Addition:
      case ValueTypes::Subtraction:
      case ValueTypes::Multiplication:
      case ValueTypes::Division:
      case ValueTypes::Modulo:
        return true;
      default:
        return false;
    }
  }
};

}  // namespace InternalCORECEQL
