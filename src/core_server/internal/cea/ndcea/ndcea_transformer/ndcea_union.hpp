#pragma once
#include "core_server/internal/cea/ndcea/ndcea.hpp"
#include "ndcea_transformer.hpp"

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
  NDCEA operator()(NDCEA& left, NDCEA& right) { return eval(left, right); }

  NDCEA eval(NDCEA& left, NDCEA& right) { return union_of(left, right); }
};

}  // namespace CEA
}  // namespace Internal
}  // namespace CORE
