#pragma once

#include <stdexcept>

namespace InternalCORECEQL {

class Attribute;
class BooleanLiteral;
class DoubleLiteral;
class IntegerLiteral;
class LongLiteral;
class StringLiteral;
class Undefined;
class Addition;
class Division;
class Modulo;
class Multiplication;
class Negation;
class Subtraction;

class ValueVisitor {
 public:
  virtual ~ValueVisitor() = default;

  // clang-format off
  virtual void visit(Attribute&)       {throw std::logic_error("visit Attribute not implemented.");}
  virtual void visit(BooleanLiteral&)  {throw std::logic_error("visit BooleanLiteral not implemented.");}
  virtual void visit(DoubleLiteral&)   {throw std::logic_error("visit DoubleLiteral  not implemented.");}
  virtual void visit(IntegerLiteral&)  {throw std::logic_error("visit IntegerLiteral not implemented.");}
  virtual void visit(LongLiteral&)     {throw std::logic_error("visit LongLiteral not implemented.");}
  virtual void visit(StringLiteral&)   {throw std::logic_error("visit StringLiteral not implemented.");}
  virtual void visit(Undefined&)       {throw std::logic_error("visit Undefined not implemented.");}
  virtual void visit(Addition&)        {throw std::logic_error("visit Addition not implemented.");}
  virtual void visit(Division&)        {throw std::logic_error("visit Division not implemented.");}
  virtual void visit(Modulo&)          {throw std::logic_error("visit Modulo not implemented.");}
  virtual void visit(Multiplication&)  {throw std::logic_error("visit Multiplication not implemented.");}
  virtual void visit(Negation&)        {throw std::logic_error("visit Negation not implemented.");}
  virtual void visit(Subtraction&)     {throw std::logic_error("visit Subtraction not implemented.");}

  // clang-format on
};
}  // namespace InternalCORECEQL
