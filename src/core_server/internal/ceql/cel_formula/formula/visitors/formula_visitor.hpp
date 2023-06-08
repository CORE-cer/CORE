#pragma once

#include <stdexcept>

namespace InternalCORECEQL {

class EventTypeFormula;   //[[R]]
class AsFormula;          // AS
class FilterFormula;      // FILTER
class OrFormula;          // OR
class SequencingFormula;  // ;
class IterationFormula;   // +
class ProjectionFormula;  // π

class FormulaVisitor {
 public:
  virtual ~FormulaVisitor() = default;

  // clang-format off
  virtual void visit(EventTypeFormula&)  {throw std::logic_error("visit EventTypeFormula not implemented.");}
  virtual void visit(AsFormula&)         {throw std::logic_error("visit AsFormula not implemented.");}
  virtual void visit(FilterFormula&)     {throw std::logic_error("visit FilterFormula not implemented.");}
  virtual void visit(OrFormula&)         {throw std::logic_error("visit OrFormula not implemented.");}
  virtual void visit(SequencingFormula&) {throw std::logic_error("visit SequencingFormula not implemented.");}
  virtual void visit(IterationFormula&)  {throw std::logic_error("visit IterationFormula not implemented.");}
  virtual void visit(ProjectionFormula&) {throw std::logic_error("visit ProjectionFormula not implemented.");}

  // clang-format on
};
}  // namespace InternalCORECEQL
