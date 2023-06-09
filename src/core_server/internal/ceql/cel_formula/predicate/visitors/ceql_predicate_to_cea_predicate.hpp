#pragma once

#include "core_server/internal/cea/predicate/predicate_headers.hpp"
#include "core_server/internal/ceql/value/visitors/determine_value_type.hpp"
#include "core_server/internal/ceql/cel_formula/predicate/predicate_headers.hpp"
#include "predicate_visitor.hpp"

namespace InternalCORECEQL {

class CEQLPredicateToCEQPredicate final : public PredicateVisitor {
  private :
    DetermineValueType value_type_visitor;
    std::unique_ptr<InternalCORECEA::Predicate> predicate;
  public:
  void visit(AndPredicate& and_predicate) override {
    throw std::logic_error("visit AndPredicate not implemented.");
  }

  void visit(InPredicate& in_predicate) override {
    throw std::logic_error("visit InPredicate not implemented.");
  }

  void visit(InequalityPredicate& inequality_predicate) override {
    inequality_predicate.left->accept_visitor(value_type_visitor);
    auto first_val_type = value_type_visitor.value_type;
    inequality_predicate.right->accept_visitor(value_type_visitor);
    auto second_val_type = value_type_visitor.value_type;
    // TODO: Cases where they are not math_expr (general case)
    // math_expr
    // attribute y literal
    // const_expr <- simplifica
    // attribute vs attribute
    


  }

  void visit(LikePredicate& like_predicate) override {
    throw std::logic_error("visit LikePredicate not implemented.");
  }

  void visit(NotPredicate& not_predicate) override {
    throw std::logic_error("visit NotPredicate not implemented.");
  }

  void visit(OrPredicate& or_predicate) override {
    throw std::logic_error("visit OrPredicate not implemented.");
  }

  void visit(
      ConstantBooleanPredicate& constant_boolean_predicate) override {
    throw std::logic_error(
        "visit ConstantBooleanPredicate not implemented.");
  }
};
}  // namespace InternalCORECEQL
