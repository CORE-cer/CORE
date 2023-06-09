#pragma once

#include "core_server/internal/ceql/value/all_value_headers.hpp"
#include "core_server/internal/stream/ring_tuple_queue/tuple.hpp"
#include "shared/datatypes/catalog/event_info.hpp"
#include "value_visitor.hpp"

namespace InternalCORECEQL {
class DetermineFinalValueDataType : public ValueVisitor {
  enum DataType {
    Integer,
    Double,
    String,
    Invalid,
    Undetermined,
  };

 public:
  DataType final_value_datatype = Undetermined;

  DetermineFinalValueDataType(CORETypes::EventInfo& event_info)
      : event_info(event_info) {}

 private:
  CORETypes::EventInfo& event_info;

 public:
  void visit(Attribute& attribute) override {
    auto attribute_id =
        event_info.attribute_names_to_ids.find(attribute.value);
    if (attribute_id == event_info.attribute_names_to_ids.end()) {
      throw std::runtime_error("Attribute " + attribute.value +
                               " does not exist in event " +
                               event_info.name);
    }
    size_t id = attribute_id->second;
    auto info = event_info.attributes_info[id];
    //switch (info.value_type) {
    //case ValueTypes::Undefined:
    //case ValueTypes::Attribute:
    //case ValueTypes::BooleanLiteral:
    //case ValueTypes::DoubleLiteral:
    //case ValueTypes::IntegerLiteral:
    //case ValueTypes::LongLiteral:
    //case ValueTypes::StringLiteral:
    //case ValueTypes::RegexLiteral:
    //case ValueTypes::Sequence:
    //case ValueTypes::Negation:
    //case ValueTypes::Addition:
    //case ValueTypes::Subtraction:
    //case ValueTypes::Multiplication:
    //case ValueTypes::Division:
    //case ValueTypes::Modulo:
  }

  // clang-format off

  //void visit(BooleanLiteral&) override {value_type = ValueTypes::BooleanLiteral;}
  //void visit(DoubleLiteral&)  override {value_type = ValueTypes::DoubleLiteral;}
  //void visit(IntegerLiteral&) override {value_type = ValueTypes::IntegerLiteral;}
  //void visit(LongLiteral&)    override {value_type = ValueTypes::LongLiteral;}
  //void visit(StringLiteral&)  override {value_type = ValueTypes::StringLiteral;}
  //void visit(RegexLiteral&)   override {value_type = ValueTypes::RegexLiteral;}
  //void visit(Undefined&)      override {value_type = ValueTypes::Undefined;}


  //void visit(Addition&       val) override {determine_binary_value_type(val);}
  //void visit(Division&       val) override {determine_binary_value_type(val);}
  //void visit(Modulo&         val) override {determine_binary_value_type(val);}
  //void visit(Multiplication& val) override {determine_binary_value_type(val);}
  //void visit(Negation&       val) override {determine_binary_value_type(val);}
  //void visit(Subtraction&    val) override {determine_binary_value_type(val);}

  //void visit(Sequence& val)       override {value_type = ValueTypes::Sequence;}

  // clang-format on
  //template <typename Type>
  //Type determine_binary_value_type(Type& value) {}
};
}  // namespace InternalCORECEQL
