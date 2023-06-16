#pragma once

#include <memory>
#include <string>
#include "visitors/predicate_visitor.hpp"

namespace InternalCORECEQL {

struct Predicate {
  public:
    int physical_predicate_id = -1;
    virtual ~Predicate() = default;
    virtual std::unique_ptr<Predicate> clone() const = 0;
    virtual std::unique_ptr<Predicate> negate() const = 0;
    virtual bool is_constant() const = 0;
    virtual bool equals(Predicate* other) const = 0;
    virtual std::string to_string() const = 0;
    virtual void accept_visitor(PredicateVisitor&) = 0;
};
}
