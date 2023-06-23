#pragma once

#include <set>

#include "core_server/internal/ceql/value/all_value_headers.hpp"
#include "core_server/internal/coordination/catalog.hpp"
#include "core_server/internal/stream/ring_tuple_queue/tuple.hpp"
#include "shared/datatypes/catalog/event_info.hpp"
#include "value_visitor.hpp"

namespace InternalCORECEQL {
class ObtainCompatibleEventTypes : public ValueVisitor {
 public:
  enum DataType {
    Integer,
    Double,
    String,
    Invalid,
    Date,
    Undetermined,
  };

 private:
  InternalCORE::Catalog& catalog;
  std::set<EventTypeId> compatible_event_types = {};
  bool has_added_an_event_type = false;

 public:
  ObtainCompatibleEventTypes(InternalCORE::Catalog& catalog)
      : catalog(catalog) {}

  std::set<uint64_t> get_compatible_event_types() const {
    return compatible_event_types;
  }

  void reset() {
    compatible_event_types = {};
    has_added_an_event_type = false;
  }

  void visit(Attribute& attribute) override {
    auto event_types = catalog.get_compatible_event_types(attribute.value);
    compatible_event_types = intersect(compatible_event_types, event_types);
  }

  // clang-format off
  void visit(BooleanLiteral&) override {}
  void visit(DoubleLiteral&)  override {}
  void visit(IntegerLiteral&) override {}
  void visit(LongLiteral&)    override {}
  void visit(StringLiteral&)  override {}
  void visit(RegexLiteral&)   override {}
  void visit(Undefined&)      override {}

  void visit(Negation& val) override { val.val->accept_visitor(*this); }

  void visit(Addition& val)      override {val.left->accept_visitor(*this);
                                           val.right->accept_visitor(*this);}
  void visit(Division& val)      override {val.left->accept_visitor(*this);
                                           val.right->accept_visitor(*this);}
  void visit(Modulo& val)        override {val.left->accept_visitor(*this);
                                           val.right->accept_visitor(*this);}
  void visit(Multiplication& val)override {val.left->accept_visitor(*this);
                                           val.right->accept_visitor(*this);}
  void visit(Subtraction& val)   override {val.left->accept_visitor(*this);
                                           val.right->accept_visitor(*this);}

  // clang-format on
  void visit(Sequence& sequence) override {
    for (auto& val : sequence.values) {
      val->accept_visitor(*this);
    }
  }

 private:
  static std::set<EventTypeId> intersect(std::set<EventTypeId> left,
                                         std::set<EventTypeId> right) {
    std::set<EventTypeId> out;
    for (auto& elem : left)
      if (right.contains(elem)) out.insert(elem);
    return out;
  }
};
}  // namespace InternalCORECEQL
