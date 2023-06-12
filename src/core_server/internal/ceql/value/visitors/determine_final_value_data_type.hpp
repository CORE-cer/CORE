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
    Date,
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
    if (final_value_datatype == Invalid) {
      return;
    }
    auto attribute_id =
        event_info.attribute_names_to_ids.find(attribute.value);
    if (attribute_id == event_info.attribute_names_to_ids.end()) {
      throw std::runtime_error("Attribute " + attribute.value +
                               " does not exist in event " +
                               event_info.name);
    }
    size_t id = attribute_id->second;
    auto info = event_info.attributes_info[id];
    switch (final_value_datatype) {
      case Integer:
        switch (info.value_type) {
          case CORETypes::ValueTypes::DOUBLE:
            final_value_datatype = Double;
            break;
          case CORETypes::ValueTypes::INT64:
          case CORETypes::ValueTypes::BOOL:
            break;
          case CORETypes::ValueTypes::STRING_VIEW:
          case CORETypes::ValueTypes::DATE:
            final_value_datatype = Invalid;
        }
      case Double:
        switch (info.value_type) {
          case CORETypes::ValueTypes::DOUBLE:
          case CORETypes::ValueTypes::INT64:
          case CORETypes::ValueTypes::BOOL:
            break;
          case CORETypes::ValueTypes::STRING_VIEW:
          case CORETypes::ValueTypes::DATE:
            final_value_datatype = Invalid;
        }
      case String:
        switch (info.value_type) {
          case CORETypes::ValueTypes::STRING_VIEW:
            break;
          default:
            final_value_datatype = Invalid;
        }
      case Date:
        switch (info.value_type) {
          case CORETypes::ValueTypes::DATE:
            break;
          default:
            final_value_datatype = Invalid;
        }
      case Undetermined:
        switch (info.value_type) {
          case CORETypes::ValueTypes::DOUBLE:
            final_value_datatype = Double;
            break;
          case CORETypes::ValueTypes::INT64:
          case CORETypes::ValueTypes::BOOL:
            final_value_datatype = Integer;
            break;
          case CORETypes::ValueTypes::STRING_VIEW:
            final_value_datatype = String;
            break;
          case CORETypes::ValueTypes::DATE:
            final_value_datatype = Date;
            break;
        }
    }
  }

  void visit(BooleanLiteral&) override {
    if (final_value_datatype == Undetermined)
      final_value_datatype = Integer;
    if (final_value_datatype == String || final_value_datatype == Date) {
      final_value_datatype = Invalid;
    }
  }

  void visit(DoubleLiteral&) override {
    if (final_value_datatype == Undetermined ||
        final_value_datatype == Integer)
      final_value_datatype = Double;
    if (final_value_datatype == String || final_value_datatype == Date) {
      final_value_datatype = Invalid;
    }
  }

  void visit(IntegerLiteral&) override {
    if (final_value_datatype == Undetermined)
      final_value_datatype = Integer;
    if (final_value_datatype == String || final_value_datatype == Date) {
      final_value_datatype = Invalid;
    }
  }

  void visit(LongLiteral&) override {
    if (final_value_datatype == Undetermined)
      final_value_datatype = Integer;
    if (final_value_datatype == String || final_value_datatype == Date) {
      final_value_datatype = Invalid;
    }
  }

  void visit(StringLiteral&) override {
    if (final_value_datatype == Undetermined)
      final_value_datatype = String;
    if (final_value_datatype == Integer ||
        final_value_datatype == Double || final_value_datatype == Date) {
      final_value_datatype = Invalid;
    }
  }

  void visit(RegexLiteral&) override {
    if (final_value_datatype == Undetermined)
      final_value_datatype = String;
    if (final_value_datatype == Integer ||
        final_value_datatype == Double || final_value_datatype == Date) {
      final_value_datatype = Invalid;
    }
  }

  void visit(Undefined&) override { final_value_datatype = Invalid; }

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
};
}  // namespace InternalCORECEQL
