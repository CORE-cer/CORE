#pragma once

#include <cstdint>
#include <set>

#include "core_server/internal/ceql/value/attribute.hpp"
#include "core_server/internal/ceql/value/operations/addition.hpp"
#include "core_server/internal/ceql/value/operations/division.hpp"
#include "core_server/internal/ceql/value/operations/modulo.hpp"
#include "core_server/internal/ceql/value/operations/multiplication.hpp"
#include "core_server/internal/ceql/value/operations/negation.hpp"
#include "core_server/internal/ceql/value/operations/subtraction.hpp"
#include "core_server/internal/ceql/value/sequence.hpp"
#include "core_server/internal/coordination/query_catalog.hpp"
#include "shared/datatypes/aliases/event_type_id.hpp"
#include "value_visitor.hpp"

namespace CORE::Internal::CEQL {
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
  QueryCatalog& query_catalog;
  std::set<Types::UniqueEventTypeId> compatible_event_types = {};
  bool has_added_an_event_type = false;

 public:
  ObtainCompatibleEventTypes(QueryCatalog& query_catalog) : query_catalog(query_catalog) {
    for (Types::UniqueEventTypeId i = 0; i < query_catalog.number_of_events(); i++) {
      compatible_event_types.insert(i);
    }
  }

  std::set<uint64_t> get_compatible_event_types() const { return compatible_event_types; }

  void reset() {
    compatible_event_types = {};
    has_added_an_event_type = false;
  }

  void visit(Attribute& attribute) override {
    auto event_types = query_catalog.get_compatible_event_types(attribute.value);
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
  static std::set<Types::UniqueEventTypeId>
  intersect(std::set<Types::UniqueEventTypeId> left,
            std::set<Types::UniqueEventTypeId> right) {
    // TODO: Replace with std::set_intersection
    std::set<Types::UniqueEventTypeId> out;
    for (auto& elem : left)
      if (right.contains(elem)) out.insert(elem);
    return out;
  }
};
}  // namespace CORE::Internal::CEQL
