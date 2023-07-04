#pragma once
#include "core_server/internal/cea/cea/cea.hpp"
#include "core_server/internal/cea/cea/transformations/cea_transformer.hpp"

namespace CORE {
namespace Internal {
namespace CEA {

/**
 * The union_of method is a helper function of the base class.
 */
class NDCEAUnion : public NDCEATransformer<NDCEAUnion> {
 public:
  // Because we're not using the && constructor we define our own,
  // the && initiation will still work though.
  CEA operator()(CEA& left, CEA& right) { return eval(left, right); }

  CEA eval(CEA& left, CEA& right) { return union_of(left, right); }
};

}  // namespace CEA
}  // namespace Internal
}  // namespace CORE
