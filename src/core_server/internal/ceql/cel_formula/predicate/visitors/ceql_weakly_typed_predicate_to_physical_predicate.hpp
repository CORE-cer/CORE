#pragma once

#include <cassert>

#include "core_server/internal/ceql/cel_formula/predicate/predicate_headers.hpp"
#include "core_server/internal/ceql/value/visitors/determine_final_value_data_type_with_catalog.hpp"
#include "core_server/internal/ceql/value/visitors/determine_value_type.hpp"
#include "core_server/internal/ceql/value/visitors/obtain_compatible_event_types.hpp"
#include "core_server/internal/ceql/value/visitors/weakly_typed_value_to_math_expr.hpp"
#include "core_server/internal/coordination/catalog.hpp"
#include "core_server/internal/evaluation/physical_predicate/in_range_predicate.hpp"
#include "core_server/internal/evaluation/physical_predicate/like_predicate/compare_with_regex_weakly_typed.hpp"
#include "core_server/internal/evaluation/physical_predicate/predicate_headers.hpp"
#include "predicate_visitor.hpp"

namespace CORE::Internal::CEQL {

class CEQLWeaklyTypedPredicateToCEAPredicate final
    : public PredicateVisitor {
 private:
  using CEQLComparison = CEQL::InequalityPredicate::LogicalOperation;
  using CEAComparison = CEA::ComparisonType;
  using FinalType = CEQL::DetermineFinalValueDataTypeWithCatalog;

 public:
  std::unique_ptr<CEA::PhysicalPredicate> predicate;

 private:
  // In the construction, we will determine the event types
  // that could satisfy all of the predicates that will be used.
  Catalog& catalog;
  std::set<Types::EventTypeId> admissible_event_types;
  bool has_added_admissible_event_types = false;
  DetermineFinalValueDataTypeWithCatalog final_data_type_visitor;

 public:
  CEQLWeaklyTypedPredicateToCEAPredicate(Catalog& catalog)
      : catalog(catalog), final_data_type_visitor(catalog) {}

  void visit(InPredicate& in_predicate) override {
    throw std::logic_error("visit InPredicate not implemented.");
  }

  void visit(InequalityPredicate& inequality_predicate) override {
    ObtainCompatibleEventTypes determine_event_types(catalog);
    inequality_predicate.left->accept_visitor(determine_event_types);
    inequality_predicate.right->accept_visitor(determine_event_types);
    std::set<Types::EventTypeId>
      compatible_event_types = determine_event_types
                                 .get_compatible_event_types();

    if (has_added_admissible_event_types) {
      admissible_event_types = intersect(admissible_event_types,
                                         compatible_event_types);
    } else {
      admissible_event_types = compatible_event_types;
      has_added_admissible_event_types = true;
    }

    predicate = compare_math_exprs(inequality_predicate.left,
                                   convert_op(
                                     inequality_predicate.logical_op),
                                   inequality_predicate.right);
  }

  void visit(LikePredicate& like_predicate) override {
    ObtainCompatibleEventTypes determine_event_types(catalog);
    like_predicate.left->accept_visitor(determine_event_types);
    like_predicate.right->accept_visitor(determine_event_types);
    std::set<Types::EventTypeId>
      compatible_event_types = determine_event_types
                                 .get_compatible_event_types();

    if (has_added_admissible_event_types) {
      admissible_event_types = intersect(admissible_event_types,
                                         compatible_event_types);
    } else {
      admissible_event_types = compatible_event_types;
      has_added_admissible_event_types = true;
    }

    predicate = compare_with_regex(like_predicate.left,
                                   like_predicate.right);
  }

  void visit(InRangePredicate& in_range_predicate) override {
    ObtainCompatibleEventTypes determine_event_types(catalog);
    in_range_predicate.left->accept_visitor(determine_event_types);
    in_range_predicate.lower_bound->accept_visitor(determine_event_types);
    in_range_predicate.upper_bound->accept_visitor(determine_event_types);
    std::set<Types::EventTypeId>
      compatible_event_types = determine_event_types
                                 .get_compatible_event_types();
    if (has_added_admissible_event_types) {
      admissible_event_types = intersect(admissible_event_types,
                                         compatible_event_types);
    } else {
      admissible_event_types = compatible_event_types;
      has_added_admissible_event_types = true;
    }
    final_data_type_visitor.reset();
    in_range_predicate.left->accept_visitor(final_data_type_visitor);
    in_range_predicate.lower_bound->accept_visitor(final_data_type_visitor);
    in_range_predicate.upper_bound->accept_visitor(final_data_type_visitor);
    auto combined_type = final_data_type_visitor.get_final_data_type();
    switch (combined_type) {
      case FinalType::Integer:
        predicate = create_in_range_predicate<int64_t>(
          in_range_predicate.left,
          in_range_predicate.lower_bound,
          in_range_predicate.upper_bound);
        break;
      case FinalType::Double:
        predicate = create_in_range_predicate<double>(
          in_range_predicate.left,
          in_range_predicate.lower_bound,
          in_range_predicate.upper_bound);
        break;
      case FinalType::Date:
        predicate = create_in_range_predicate<std::time_t>(
          in_range_predicate.left,
          in_range_predicate.lower_bound,
          in_range_predicate.upper_bound);
        break;
      case FinalType::String:
        throw std::runtime_error(
          "Invalid Value data type String for InRangePredicate");
      case FinalType::Undetermined:
        throw std::runtime_error("No type was deduced from Value");
      case FinalType::Invalid:
        throw std::runtime_error("Invalid mix of types in value");
      default:
        throw std::logic_error(
          "Non implemented Type in ceql_predicate_to_cea_predicate.hpp "
          "compare_math_exprs");
    }
  }

  void visit(NotPredicate& not_predicate) override {
    not_predicate.predicate->accept_visitor(*this);
    if (has_added_admissible_event_types)
      predicate = std::make_unique<CEA::NotPredicate>(admissible_event_types,
                                                      std::move(predicate));
    else
      predicate = std::make_unique<CEA::NotPredicate>(std::move(predicate));
  }

  void visit(OrPredicate& or_predicate) override {
    std::vector<std::unique_ptr<CEA::PhysicalPredicate>> predicates;
    // Since we want to show that the or_predicate can be evaluated
    // to true if a event type satisfied some of it's predicates,
    // we set as a condition that it must be some of the tuples that
    // previously were admissible.
    auto previous_admissible_event_types = admissible_event_types;
    auto new_admissible_event_types = admissible_event_types;
    bool old_has_added_event_types = has_added_admissible_event_types;
    bool new_has_added_event_types = has_added_admissible_event_types;
    for (auto& predicate_ : or_predicate.predicates) {
      predicate_->accept_visitor(*this);
      predicates.push_back(std::move(predicate));
      new_admissible_event_types = unite(std::move(
                                           new_admissible_event_types),
                                         std::move(admissible_event_types));
      admissible_event_types = previous_admissible_event_types;
      new_has_added_event_types = has_added_admissible_event_types
                                  || new_has_added_event_types;
      has_added_admissible_event_types = old_has_added_event_types;
    }
    if (has_added_admissible_event_types)
      predicate = std::make_unique<CEA::OrPredicate>(admissible_event_types,
                                                     std::move(predicates));
    else {
      predicate = std::make_unique<CEA::OrPredicate>(std::move(predicates));
    }
  }

  void visit(AndPredicate& and_predicate) override {
    std::vector<std::unique_ptr<CEA::PhysicalPredicate>> predicates;
    for (auto& predicate_ : and_predicate.predicates) {
      predicate_->accept_visitor(*this);
      predicates.push_back(std::move(predicate));
    }
    if (has_added_admissible_event_types)
      predicate = std::make_unique<CEA::AndPredicate>(admissible_event_types,
                                                      std::move(predicates));
    else
      predicate = std::make_unique<CEA::AndPredicate>(std::move(predicates));
  }

  void visit(ConstantBooleanPredicate& constant_boolean_predicate) override {
    throw std::logic_error(
      "visit ConstantBooleanPredicate not implemented.");
  }

 private:
  std::unique_ptr<CEA::PhysicalPredicate>
  compare_math_exprs(std::unique_ptr<CEQL::Value>& left,
                     CEAComparison op,
                     std::unique_ptr<CEQL::Value>& right) {
    left->accept_visitor(final_data_type_visitor);
    right->accept_visitor(final_data_type_visitor);
    auto combined_type = final_data_type_visitor.get_final_data_type();

    switch (combined_type) {
      case FinalType::Integer:
        return compare_math_exprs<int64_t>(left, op, right);
      case FinalType::Double:
        return compare_math_exprs<double>(left, op, right);
      case FinalType::String:
        return compare_math_exprs<std::string_view>(left, op, right);
      case FinalType::Date:
        return compare_math_exprs<std::time_t>(left, op, right);
      case FinalType::Undetermined:
        throw std::runtime_error("No type was deduced from Value");
      case FinalType::Invalid:
        throw std::runtime_error("Invalid mix of types in value");
      default:
        throw std::logic_error(
          "Non implemented Type in ceql_predicate_to_cea_predicate.hpp "
          "compare_math_exprs");
    }
    return {};
  }

  template <typename ValueType>
  std::unique_ptr<CEA::PhysicalPredicate>
  compare_math_exprs(std::unique_ptr<CEQL::Value>& left,
                     CEAComparison op,
                     std::unique_ptr<CEQL::Value>& right) {
    auto left_expr = get_expr<ValueType>(left);
    auto right_expr = get_expr<ValueType>(right);

    switch (op) {
      case CEAComparison::EQUALS:
        return std::make_unique<
          CEA::CompareMathExprs<CEAComparison::EQUALS, ValueType>>(
          admissible_event_types,
          std::move(left_expr),
          std::move(right_expr));
      case CEAComparison::GREATER:
        return std::make_unique<
          CEA::CompareMathExprs<CEAComparison::GREATER, ValueType>>(
          admissible_event_types,
          std::move(left_expr),
          std::move(right_expr));
      case CEAComparison::GREATER_EQUALS:
        return std::make_unique<
          CEA::CompareMathExprs<CEAComparison::GREATER_EQUALS, ValueType>>(
          admissible_event_types,
          std::move(left_expr),
          std::move(right_expr));
      case CEAComparison::LESS_EQUALS:
        return std::make_unique<
          CEA::CompareMathExprs<CEAComparison::LESS_EQUALS, ValueType>>(
          admissible_event_types,
          std::move(left_expr),
          std::move(right_expr));
      case CEAComparison::LESS:
        return std::make_unique<
          CEA::CompareMathExprs<CEAComparison::LESS, ValueType>>(
          admissible_event_types,
          std::move(left_expr),
          std::move(right_expr));
      case CEAComparison::NOT_EQUALS:
        return std::make_unique<
          CEA::CompareMathExprs<CEAComparison::NOT_EQUALS, ValueType>>(
          admissible_event_types,
          std::move(left_expr),
          std::move(right_expr));
      default:
        throw std::logic_error(
          "Non implemented Comparison Type in "
          "ceql_predicate_to_cea_predicate.hpp "
          "compare_math_exprs");
        return {};
    }
  }

  template <typename ValueType>
  std::unique_ptr<CEA::MathExpr<ValueType>>
  get_expr(std::unique_ptr<CEQL::Value>& val) {
    WeaklyTypedValueToMathExpr<ValueType> convertor(catalog);
    val->accept_visitor(convertor);
    return std::move(convertor.math_expr);
  }

  CEAComparison convert_op(CEQLComparison op) {
    switch (op) {
      case CEQLComparison::EQUALS:
        return CEAComparison::EQUALS;
      case CEQLComparison::GREATER:
        return CEAComparison::GREATER;
      case CEQLComparison::GREATER_EQUALS:
        return CEAComparison::GREATER_EQUALS;
      case CEQLComparison::LESS_EQUALS:
        return CEAComparison::LESS_EQUALS;
      case CEQLComparison::LESS:
        return CEAComparison::LESS;
      case CEQLComparison::NOT_EQUALS:
        return CEAComparison::NOT_EQUALS;
      default:
        throw std::logic_error(
          "Non implemented Comparison Type in "
          "ceql_predicate_to_cea_predicate.hpp "
          "compare_math_exprs");
        return {};
    }
  }

  std::unique_ptr<CEA::PhysicalPredicate>
  compare_with_regex(std::unique_ptr<CEQL::Value>& left,
                     std::unique_ptr<CEQL::Value>& right) {
    assert(dynamic_cast<const CEQL::Attribute*>(left.get()) != nullptr);
    auto left_value_attr = static_cast<CEQL::Attribute*>(left.get());
    auto left_expr_attr = std::make_unique<
      CEA::NonStronglyTypedAttribute<std::string_view>>(left_value_attr->value,
                                                        catalog);

    assert(dynamic_cast<CEQL::RegexLiteral*>(right.get()) != nullptr);
    auto right_value_string = static_cast<CEQL::RegexLiteral*>(right.get());
    std::string_view value = right_value_string->value;
    auto right_expr_string = std::make_unique<CEA::Literal<std::string_view>>(
      value);

    // Pass in string as it is one time cost and CompareWithRegex should have the direct object
    std::string regex_string(right_expr_string->val);

    return std::make_unique<CEA::CompareWithRegexWeaklyTyped>(
      admissible_event_types,
      std::move(left_expr_attr),
      std::move(regex_string));
  }

  template <typename ValueType>
  std::unique_ptr<CEA::InRangePredicate<ValueType>>
  create_in_range_predicate(std::unique_ptr<CEQL::Value>& left,
                            std::unique_ptr<CEQL::Value>& lower_bound,
                            std::unique_ptr<CEQL::Value>& upper_bound) {
    return std::make_unique<CEA::InRangePredicate<ValueType>>(
      admissible_event_types,
      std::move(get_expr<ValueType>(left)),
      std::move(get_expr<ValueType>(lower_bound)),
      std::move(get_expr<ValueType>(upper_bound)));
  }

  static std::set<Types::EventTypeId>
  intersect(std::set<Types::EventTypeId> left,
            std::set<Types::EventTypeId> right) {
    // TODO: Replace with std::set_intersection
    std::set<Types::EventTypeId> out;
    for (auto& elem : left)
      if (right.contains(elem)) out.insert(elem);
    return out;
  }

  static std::set<Types::EventTypeId>
  unite(std::set<Types::EventTypeId>&& left,
        std::set<Types::EventTypeId>&& right) {
    // TODO: Replace with std::set_intersection
    std::set<Types::EventTypeId> out = std::move(left);
    for (auto& elem : right) out.insert(elem);
    return out;
  }
};

}  // namespace CORE::Internal::CEQL
