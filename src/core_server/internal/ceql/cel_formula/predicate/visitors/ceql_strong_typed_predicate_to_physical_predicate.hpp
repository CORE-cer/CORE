#pragma once
#include <memory>

#include "core_server/internal/ceql/cel_formula/predicate/predicate_headers.hpp"
#include "core_server/internal/ceql/value/visitors/determine_final_value_data_type.hpp"
#include "core_server/internal/ceql/value/visitors/determine_value_type.hpp"
#include "core_server/internal/ceql/value/visitors/value_to_math_expr.hpp"
#include "core_server/internal/coordination/catalog.hpp"
#include "core_server/internal/evaluation/physical_predicate/in_range_predicate.hpp"
#include "core_server/internal/evaluation/physical_predicate/like_predicate/compare_with_regex_strongly_typed.hpp"
#include "core_server/internal/evaluation/physical_predicate/predicate_headers.hpp"
#include "predicate_visitor.hpp"

namespace CORE::Internal::CEQL {

class CEQLStrongTypedPredicateToPhysicalPredicate final
    : public PredicateVisitor {
 private:
  DetermineValueType value_type_visitor;
  DetermineFinalValueDataType final_data_type_visitor;
  Types::EventInfo event_info;

  using CEQLComparison = CEQL::InequalityPredicate::LogicalOperation;
  using CEAComparison = CEA::ComparisonType;
  using FinalType = CEQL::DetermineFinalValueDataType;

 public:
  std::unique_ptr<CEA::PhysicalPredicate> predicate;

  CEQLStrongTypedPredicateToPhysicalPredicate(Types::EventInfo event_info)
      : event_info(event_info), final_data_type_visitor(event_info) {}

  void visit(InPredicate& in_predicate) override {
    throw std::logic_error("visit InPredicate not implemented.");
  }

  /**
   * This visitor handles all different types of physical inequalities,
   * including: comparing an attribute with a constant, two attributes,
   * two math_exprs and TODO: a constant inequality expression that is
   * simplified to a boolean.
   */
  void visit(InequalityPredicate& inequality_predicate) override {
    inequality_predicate.left->accept_visitor(value_type_visitor);
    auto first_val_type = value_type_visitor.get_value_type();
    inequality_predicate.right->accept_visitor(value_type_visitor);
    auto second_val_type = value_type_visitor.get_value_type();

    inequality_predicate.left->accept_visitor(final_data_type_visitor);
    inequality_predicate.right->accept_visitor(final_data_type_visitor);
    auto combined_type = final_data_type_visitor.get_final_data_type();

    /**
     * This double switch case basically determines what type of comparison
     * is going to be used.
     *
     * Since special formatting is used for this double switch case that
     * determines which physical plan to use, clang-format is disabled.
     */
    // clang-format off
    switch (first_val_type) {
      case ValueTypes::Attribute: switch (second_val_type) {
        case ValueTypes::Attribute:
          predicate = compare_attributes(
            inequality_predicate.left,
            convert_op(inequality_predicate.logical_op),
            inequality_predicate.right);
          return;
        case ValueTypes::IntegerLiteral:
        case ValueTypes::BooleanLiteral:
        case ValueTypes::DoubleLiteral:
          predicate = compare_with_constant(
            inequality_predicate.left,
            convert_op(inequality_predicate.logical_op),
            inequality_predicate.right);
          return;
        default:
          break;
      }
      case ValueTypes::IntegerLiteral:
      case ValueTypes::DoubleLiteral:
      case ValueTypes::BooleanLiteral: switch (second_val_type) {
        case ValueTypes::Attribute:
          predicate = compare_with_constant(
              inequality_predicate.right,
              convert_op(inequality_predicate.logical_op, true),
              inequality_predicate.left);
          return;
        case ValueTypes::IntegerLiteral:
        case ValueTypes::DoubleLiteral:
        case ValueTypes::BooleanLiteral:
          predicate = create_constant(
            inequality_predicate.left,
            convert_op(inequality_predicate.logical_op),
            inequality_predicate.right);
          return;
        default:
          break;
      }
      default:
        break;
    }
    predicate = compare_math_exprs(
      inequality_predicate.left,
      convert_op(inequality_predicate.logical_op),
      inequality_predicate.right);
    // clang-format on
  }

  void visit(LikePredicate& like_predicate) override {
    like_predicate.left->accept_visitor(value_type_visitor);
    auto first_val_type = value_type_visitor.get_value_type();
    like_predicate.right->accept_visitor(value_type_visitor);
    auto second_val_type = value_type_visitor.get_value_type();

    if (first_val_type != ValueTypes::Attribute
        || second_val_type != ValueTypes::RegexLiteral) {
      throw std::logic_error(
        "Like predicate only supports attribute and string literal.");
    }

    predicate = compare_with_regex(like_predicate.left,
                                   like_predicate.right);
  }

  void visit(InRangePredicate& in_range_predicate) override {
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

  // std::make_unique<CEA::InRangePredicate<ValueType>> create_in_range_predicate<ValueType>(std::unique_ptr<CEQL::Value>&){

  // }

  void visit(NotPredicate& not_predicate) override {
    not_predicate.predicate->accept_visitor(*this);
    predicate = std::make_unique<CEA::NotPredicate>(event_info.id,
                                                    std::move(predicate));
  }

  void visit(OrPredicate& or_predicate) override {
    std::vector<std::unique_ptr<CEA::PhysicalPredicate>> predicates;
    for (auto& predicate_ : or_predicate.predicates) {
      predicate_->accept_visitor(*this);
      predicates.push_back(std::move(predicate));
    }
    predicate = std::make_unique<CEA::OrPredicate>(event_info.id,
                                                   std::move(predicates));
  }

  void visit(AndPredicate& and_predicate) override {
    std::vector<std::unique_ptr<CEA::PhysicalPredicate>> predicates;
    for (auto& predicate_ : and_predicate.predicates) {
      predicate_->accept_visitor(*this);
      predicates.push_back(std::move(predicate));
    }
    predicate = std::make_unique<CEA::AndPredicate>(event_info.id,
                                                    std::move(predicates));
  }

  void visit(ConstantBooleanPredicate& constant_boolean_predicate) override {
    throw std::logic_error(
      "visit ConstantBooleanPredicate not implemented.");
  }

 private:
  std::unique_ptr<CEA::PhysicalPredicate>
  compare_attributes(std::unique_ptr<CEQL::Value>& left,
                     CEAComparison op,
                     std::unique_ptr<CEQL::Value>& right) {
    left->accept_visitor(final_data_type_visitor);
    auto left_type = final_data_type_visitor.get_final_data_type();

    switch (left_type) {
      case FinalType::Integer:
        return compare_attributes<int64_t>(left, op, right);
      case FinalType::Double:
        return compare_attributes<double>(left, op, right);
      case FinalType::String:
        return compare_attributes<std::string_view>(left, op, right);
      case FinalType::Date:
        return compare_attributes<std::time_t>(left, op, right);
      case FinalType::Undetermined:
        throw std::runtime_error("No type was deduced from Value");
      case FinalType::Invalid:
      default:
        throw std::logic_error(
          "Non implemented combined_type in "
          "ceql_predicate_to_cea_predicate.hpp");
    }
    return {};
  }

  template <typename LeftValueType>
  std::unique_ptr<CEA::PhysicalPredicate>
  compare_attributes(std::unique_ptr<CEQL::Value>& left,
                     CEAComparison op,
                     std::unique_ptr<CEQL::Value>& right) {
    right->accept_visitor(final_data_type_visitor);
    auto right_type = final_data_type_visitor.get_final_data_type();

    assert(dynamic_cast<CEQL::Attribute*>(left.get()) != nullptr);
    assert(dynamic_cast<CEQL::Attribute*>(right.get()) != nullptr);

    auto left_ptr = static_cast<CEQL::Attribute*>(left.get());
    auto right_ptr = static_cast<CEQL::Attribute*>(right.get());
    size_t left_pos = get_pos_from_name(left_ptr->value);
    size_t right_pos = get_pos_from_name(right_ptr->value);

    switch (right_type) {
      case FinalType::Integer:
        return compare_attributes<LeftValueType, int64_t>(left_pos,
                                                          op,
                                                          right_pos);
      case FinalType::Double:
        return compare_attributes<LeftValueType, double>(left_pos,
                                                         op,
                                                         right_pos);
      case FinalType::String:
        return compare_attributes<LeftValueType, std::string_view>(left_pos,
                                                                   op,
                                                                   right_pos);
      case FinalType::Date:
        return compare_attributes<LeftValueType, std::time_t>(left_pos,
                                                              op,
                                                              right_pos);
      case FinalType::Undetermined:
        throw std::runtime_error("No type was deduced from Value");
      case FinalType::Invalid:
      default:
        throw std::logic_error(
          "Non implemented combined_type in "
          "ceql_predicate_to_cea_predicate.hpp");
    }
    return {};
  }

  template <typename LeftValueType, typename RightValueType>
  std::unique_ptr<CEA::PhysicalPredicate>
  compare_attributes(size_t left, CEAComparison op, size_t right) {
    switch (op) {
      case CEAComparison::EQUALS:
        return std::make_unique<CEA::CompareWithAttribute<CEAComparison::EQUALS,
                                                          LeftValueType,
                                                          RightValueType>>(
          event_info.id, left, right);
      case CEAComparison::GREATER:
        return std::make_unique<
          CEA::CompareWithAttribute<CEAComparison::GREATER,
                                    LeftValueType,
                                    RightValueType>>(event_info.id,
                                                     left,
                                                     right);
      case CEAComparison::GREATER_EQUALS:
        return std::make_unique<
          CEA::CompareWithAttribute<CEAComparison::GREATER_EQUALS,
                                    LeftValueType,
                                    RightValueType>>(event_info.id,
                                                     left,
                                                     right);
      case CEAComparison::LESS_EQUALS:
        return std::make_unique<
          CEA::CompareWithAttribute<CEAComparison::LESS_EQUALS,
                                    LeftValueType,
                                    RightValueType>>(event_info.id,
                                                     left,
                                                     right);
      case CEAComparison::LESS:
        return std::make_unique<CEA::CompareWithAttribute<CEAComparison::LESS,
                                                          LeftValueType,
                                                          RightValueType>>(
          event_info.id, left, right);
      case CEAComparison::NOT_EQUALS:
        return std::make_unique<
          CEA::CompareWithAttribute<CEAComparison::NOT_EQUALS,
                                    LeftValueType,
                                    RightValueType>>(event_info.id,
                                                     left,
                                                     right);
      default:
        throw std::logic_error(
          "Non implemented op in ceql_predicate_to_cea_predicate.hpp");
        return {};
    }
  }

  /**
   * For this function the left value MUST be an Attribute.
   */
  std::unique_ptr<CEA::PhysicalPredicate>
  compare_with_constant(std::unique_ptr<CEQL::Value>& left,
                        CEAComparison op,
                        std::unique_ptr<CEQL::Value>& right) {
    left->accept_visitor(final_data_type_visitor);
    auto attribute_type = final_data_type_visitor.get_final_data_type();

    assert(dynamic_cast<CEQL::Attribute*>(left.get()) != nullptr);
    auto left_ptr = static_cast<CEQL::Attribute*>(left.get());
    size_t left_pos = get_pos_from_name(left_ptr->value);

    switch (attribute_type) {
      case FinalType::Integer:
        return compare_with_constant<int64_t>(left_pos, op, right);
      case FinalType::Double:
        return compare_with_constant<double>(left_pos, op, right);
      case FinalType::String:
        return compare_with_constant<std::string_view>(left_pos, op, right);
      case FinalType::Date:
        return compare_with_constant<std::time_t>(left_pos, op, right);
      case FinalType::Undetermined:
        throw std::runtime_error("No type was deduced from Value");
      case FinalType::Invalid:
        throw std::runtime_error("Invalid mix of types in value");
      default:
        throw std::logic_error(
          "Non implemented combined_type in "
          "ceql_predicate_to_cea_predicate.hpp");
    }
    return {};
  }

  template <typename ValueType>
  ValueType get_val_from_literal(std::unique_ptr<CEQL::Value>& ptr) {
    ptr->accept_visitor(value_type_visitor);
    auto right_value_type = value_type_visitor.get_value_type();
    if constexpr (std::is_same_v<ValueType, std::string_view>) {
      switch (right_value_type) {
        case ValueTypes::StringLiteral:
          assert(dynamic_cast<CEQL::StringLiteral*>(ptr.get()) != nullptr);
          return static_cast<CEQL::StringLiteral*>(ptr.get())->value;
        case ValueTypes::RegexLiteral:
          assert(dynamic_cast<CEQL::RegexLiteral*>(ptr.get()) != nullptr);
          return static_cast<CEQL::RegexLiteral*>(ptr.get())->value;
        default:
          throw std::runtime_error("Invalid mix of types in value");
      }
    } else {
      switch (right_value_type) {
        case ValueTypes::IntegerLiteral:
          assert(dynamic_cast<CEQL::IntegerLiteral*>(ptr.get()) != nullptr);
          return static_cast<ValueType>(
            static_cast<CEQL::IntegerLiteral*>(ptr.get())->value);
        case ValueTypes::BooleanLiteral:
          assert(dynamic_cast<CEQL::BooleanLiteral*>(ptr.get()) != nullptr);
          return static_cast<ValueType>(
            static_cast<CEQL::BooleanLiteral*>(ptr.get())->value);
        case ValueTypes::DoubleLiteral:
          assert(dynamic_cast<CEQL::DoubleLiteral*>(ptr.get()) != nullptr);
          return static_cast<ValueType>(
            static_cast<CEQL::DoubleLiteral*>(ptr.get())->value);
        // TODO: Date type.
        default:
          assert(false &&
                 "Some non constant value was passed to "
                 "get_val_from_literal");
          return {};
      }
    }
  }

  template <typename ValueType>
  std::unique_ptr<CEA::PhysicalPredicate>
  compare_with_constant(size_t left_pos,
                        CEAComparison op,
                        std::unique_ptr<CEQL::Value>& right) {
    ValueType right_val = get_val_from_literal<ValueType>(right);

    switch (op) {
      case CEAComparison::EQUALS:
        return std::make_unique<
          CEA::CompareWithConstant<CEAComparison::EQUALS, ValueType>>(
          event_info.id, left_pos, right_val);
      case CEAComparison::GREATER:
        return std::make_unique<
          CEA::CompareWithConstant<CEAComparison::GREATER, ValueType>>(
          event_info.id, left_pos, right_val);
      case CEAComparison::GREATER_EQUALS:
        return std::make_unique<
          CEA::CompareWithConstant<CEAComparison::GREATER_EQUALS, ValueType>>(
          event_info.id, left_pos, right_val);
      case CEAComparison::LESS_EQUALS:
        return std::make_unique<
          CEA::CompareWithConstant<CEAComparison::LESS_EQUALS, ValueType>>(
          event_info.id, left_pos, right_val);
      case CEAComparison::LESS:
        return std::make_unique<
          CEA::CompareWithConstant<CEAComparison::LESS, ValueType>>(
          event_info.id, left_pos, right_val);
      case CEAComparison::NOT_EQUALS:
        return std::make_unique<
          CEA::CompareWithConstant<CEAComparison::NOT_EQUALS, ValueType>>(
          event_info.id, left_pos, right_val);
      default:
        throw std::logic_error(
          "Non implemented op in ceql_predicate_to_cea_predicate.hpp "
          "compare_with_constant");
        return {};
    }
  }

  size_t get_pos_from_name(std::string name) {
    auto attribute_id = event_info.attribute_names_to_ids.find(name);
    if (attribute_id == event_info.attribute_names_to_ids.end()) {
      throw std::runtime_error(
        "Attribute " + name + " does not exist in event " + event_info.name);
    }
    return attribute_id->second;
  }

  std::unique_ptr<CEA::PhysicalPredicate>
  create_constant(std::unique_ptr<CEQL::Value>& left,
                  CEAComparison op,
                  std::unique_ptr<CEQL::Value>& right) {
    // TODO
    throw std::logic_error(
      "Create_constant not implemented yet "
      "(ceql_predicate_to_ceae_predicate.hpp)");
  }

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
  std::unique_ptr<CEA::MathExpr<ValueType>>
  get_expr(std::unique_ptr<CEQL::Value>& val) {
    ValueToMathExpr<ValueType> convertor(event_info);
    val->accept_visitor(convertor);
    return std::move(convertor.math_expr);
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
          event_info.id, std::move(left_expr), std::move(right_expr));
      case CEAComparison::GREATER:
        return std::make_unique<
          CEA::CompareMathExprs<CEAComparison::GREATER, ValueType>>(
          event_info.id, std::move(left_expr), std::move(right_expr));
      case CEAComparison::GREATER_EQUALS:
        return std::make_unique<
          CEA::CompareMathExprs<CEAComparison::GREATER_EQUALS, ValueType>>(
          event_info.id, std::move(left_expr), std::move(right_expr));
      case CEAComparison::LESS_EQUALS:
        return std::make_unique<
          CEA::CompareMathExprs<CEAComparison::LESS_EQUALS, ValueType>>(
          event_info.id, std::move(left_expr), std::move(right_expr));
      case CEAComparison::LESS:
        return std::make_unique<
          CEA::CompareMathExprs<CEAComparison::LESS, ValueType>>(
          event_info.id, std::move(left_expr), std::move(right_expr));
      case CEAComparison::NOT_EQUALS:
        return std::make_unique<
          CEA::CompareMathExprs<CEAComparison::NOT_EQUALS, ValueType>>(
          event_info.id, std::move(left_expr), std::move(right_expr));
      default:
        throw std::logic_error(
          "Non implemented Comparison Type in "
          "ceql_predicate_to_cea_predicate.hpp "
          "compare_math_exprs");
        return {};
    }
  }

  CEAComparison convert_op(CEQLComparison op, bool reverse = false) {
    switch (op) {
      case CEQLComparison::EQUALS:
        return reverse ? CEAComparison::NOT_EQUALS : CEAComparison::EQUALS;
      case CEQLComparison::GREATER:
        return reverse ? CEAComparison::LESS_EQUALS : CEAComparison::GREATER;
      case CEQLComparison::GREATER_EQUALS:
        return reverse ? CEAComparison::LESS : CEAComparison::GREATER_EQUALS;
      case CEQLComparison::LESS_EQUALS:
        return reverse ? CEAComparison::GREATER : CEAComparison::LESS_EQUALS;
      case CEQLComparison::LESS:
        return reverse ? CEAComparison::GREATER_EQUALS : CEAComparison::LESS;
      case CEQLComparison::NOT_EQUALS:
        return reverse ? CEAComparison::EQUALS : CEAComparison::NOT_EQUALS;
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
    assert(dynamic_cast<CEQL::Attribute*>(left.get()) != nullptr);
    auto left_ptr = static_cast<CEQL::Attribute*>(left.get());
    size_t left_pos = get_pos_from_name(left_ptr->value);

    std::string_view right_str_view = get_val_from_literal<std::string_view>(
      right);

    // Pass in string as it is one time cost and CompareWithRegex should have the direct object
    std::string right_str(right_str_view);

    return std::make_unique<CEA::CompareWithRegexStronglyTyped>(
      event_info.id, left_pos, std::move(right_str));
  }

  template <typename ValueType>
  std::unique_ptr<CEA::InRangePredicate<ValueType>>
  create_in_range_predicate(std::unique_ptr<CEQL::Value>& left,
                            std::unique_ptr<CEQL::Value>& lower_bound,
                            std::unique_ptr<CEQL::Value>& upper_bound) {
    return std::make_unique<CEA::InRangePredicate<ValueType>>(
      event_info.id,
      std::move(get_expr<ValueType>(left)),
      std::move(get_expr<ValueType>(lower_bound)),
      std::move(get_expr<ValueType>(upper_bound)));
  }
};

}  // namespace CORE::Internal::CEQL
