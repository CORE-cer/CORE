#pragma once

#include <stdexcept>

#include "core_server/internal/ceql/value/attribute.hpp"
#include "core_server/internal/ceql/value/operations/addition.hpp"
#include "core_server/internal/ceql/value/operations/division.hpp"
#include "core_server/internal/ceql/value/operations/modulo.hpp"
#include "core_server/internal/ceql/value/operations/multiplication.hpp"
#include "core_server/internal/ceql/value/operations/negation.hpp"
#include "core_server/internal/ceql/value/operations/subtraction.hpp"
#include "core_server/internal/ceql/value/sequence.hpp"
#include "core_server/internal/coordination/query_catalog.hpp"
#include "shared/datatypes/catalog/datatypes.hpp"
#include "value_visitor.hpp"

namespace CORE::Internal::CEQL {
class DetermineFinalValueDataTypeWithCatalog : public ValueVisitor {
 public:
  enum DataType {
    Integer,
    Double,
    String,
    Invalid,
    Date,
    Undetermined,
  };

 public:
  DataType final_value_datatype = Undetermined;

 private:
  const QueryCatalog& query_catalog;

 public:
  DetermineFinalValueDataTypeWithCatalog(const QueryCatalog& query_catalog)
      : query_catalog(query_catalog) {}

  DataType get_final_data_type() {
    auto out = final_value_datatype;
    final_value_datatype = Undetermined;
    return out;
  }

  void reset() { final_value_datatype = Undetermined; }

  void visit(Attribute& attribute) override {
    auto value_types = query_catalog.get_possible_attribute_types(attribute.value);
    if (value_types.size() == 0) {
      final_value_datatype = Invalid;
    }
    for (auto type : value_types) {
      DataType event_type = attribute_info_type_convertor(type);
      update_final_data_type(event_type);
    }
  }

  // clang-format off
  void visit(BooleanLiteral&) override { update_final_data_type(Integer); }
  void visit(DoubleLiteral&)  override { update_final_data_type(Double); }
  void visit(IntegerLiteral&) override { update_final_data_type(Integer); }
  void visit(LongLiteral&)    override { update_final_data_type(Integer); }
  void visit(StringLiteral&)  override { update_final_data_type(String); }
  void visit(RegexLiteral&)   override { update_final_data_type(String); }
  void visit(Undefined&)      override { final_value_datatype = Invalid; }

  // clang-format on

  void visit(Addition& val) override {
    val.left->accept_visitor(*this);
    val.right->accept_visitor(*this);
  }

  void visit(Division& val) override {
    val.left->accept_visitor(*this);
    val.right->accept_visitor(*this);
  }

  void visit(Modulo& val) override {
    val.left->accept_visitor(*this);
    val.right->accept_visitor(*this);
  }

  void visit(Multiplication& val) override {
    val.left->accept_visitor(*this);
    val.right->accept_visitor(*this);
  }

  void visit(Negation& val) override { val.val->accept_visitor(*this); }

  void visit(Subtraction& val) override {
    val.left->accept_visitor(*this);
    val.right->accept_visitor(*this);
  }

  void visit(Sequence& sequence) override {
    for (auto& val : sequence.values) {
      val->accept_visitor(*this);
    }
  }

 private:
  static DataType attribute_info_type_convertor(Types::ValueTypes type) {
    switch (type) {
      case Types::ValueTypes::DOUBLE:
        return Double;
      case Types::ValueTypes::INT64:
      case Types::ValueTypes::BOOL:
        return Integer;
      case Types::ValueTypes::STRING_VIEW:
        return String;
      case Types::ValueTypes::DATE:
        return Date;
      default:
        throw std::logic_error(
          "A COREType is not considered inside of "
          "attribute_info_type_convertor at "
          "determine_final_value_data_type.hpp");
    }
  }

  // clang-format off
  constexpr static DataType type_composition_map[6][6] = {
    //Integer  Double   String   Invalid  Date     Undetermined
    {Integer,  Double,  Invalid, Invalid, Invalid, Integer},      // Integer
    {Double,   Double,  Invalid, Invalid, Invalid, Double},       // Double
    {Invalid,  Invalid, String,  Invalid, Invalid, String},       // String
    {Invalid,  Invalid, Invalid, Invalid, Invalid, Invalid},      // Invalid
    {Invalid,  Invalid, Invalid, Invalid, Date,    Date},         // Date
    {Integer,  Double,  String,  Invalid, Date,    Undetermined}, // Undetermined
  };

  void update_final_data_type(DataType type) {
    final_value_datatype = type_composition_map[type][final_value_datatype];
  }

  // clang-format on
};
}  // namespace CORE::Internal::CEQL
