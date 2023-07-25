#include "core_server/internal/evaluation/enumeration/tecs/tecs.hpp"

#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_vector.hpp>
#include <memory>

#include "core_server/internal/evaluation/enumeration/tecs/node.hpp"

namespace CORE::Internal::tECS::UnitTests {
TEST_CASE("new-ulist lifespan") {
  tECS ecs;
  // We first create 2048 nodes so that the first minipool is full.
  Node* first_node = nullptr;
  for (size_t i = 0; i < 2048; i++) {
    std::vector<Node*> ulist = ecs.new_ulist(ecs.new_bottom(1));
    REQUIRE(ulist.size() == 1);
    REQUIRE(ulist.back()->is_bottom());
    REQUIRE(ulist.back()->max() == 1);
    REQUIRE(ulist.back()->pos() == 1);
    if (first_node == nullptr) first_node = ulist.back();
    REQUIRE(first_node->is_bottom());
  }
  ecs.unpin_node(first_node);
  //REQUIRE(first_node->is_bottom());
  //REQUIRE(first_node->max() == 1);
  //REQUIRE(first_node->pos() == 1);
  std::vector<Node*> ulist = ecs.new_ulist(ecs.new_bottom(2));
  REQUIRE(ulist.back()->is_bottom());
  REQUIRE(ulist.back()->max() == 2);
  REQUIRE(ulist.back()->pos() == 2);
}

TEST_CASE("insert") {
  tECS ecs;
  std::vector<Node*> ulist = ecs.new_ulist(ecs.new_bottom(20));
  ulist = ecs.insert(std::move(ulist), ecs.new_bottom(1));
  ulist = ecs.insert(std::move(ulist),
                     ecs.new_bottom(1));  // This merges both bottoms.
  ulist = ecs.insert(std::move(ulist), ecs.new_extend(ecs.new_bottom(2), 3));
  REQUIRE(ulist.size() == 3);
  for (size_t i = 0; i < 3; i++) {
    REQUIRE(ulist[i] != nullptr);
  }
  REQUIRE(ulist[0]->pos() == 20);
  REQUIRE(ulist[1]->pos() == 3);
  REQUIRE(ulist[1]->left != nullptr);
  REQUIRE(ulist[1]->left->pos() == 2);
  REQUIRE(ulist[2]->left != nullptr);
  REQUIRE(ulist[2]->left->pos() == 1);
  REQUIRE(ulist[2]->right != nullptr);
  REQUIRE(ulist[2]->right->pos() == 1);
}

TEST_CASE("merge") {
  tECS ecs;
  std::vector<Node*> ulist = ecs.new_ulist(ecs.new_bottom(20));
  ulist = ecs.insert(std::move(ulist), ecs.new_bottom(1));
  ulist = ecs.insert(std::move(ulist),
                     ecs.new_bottom(1));  // This merges both bottoms.
  ulist = ecs.insert(std::move(ulist), ecs.new_extend(ecs.new_bottom(2), 3));
  Node* merged_node = ecs.merge(ulist);
  /*       _\|/_
            (o o)
    +----oOO-{_}-OOo-----------+
    |Topology:                 |
    |            u             |
    |    b(20)         u       |
    |           e(3)       u   |
    |           b(2)     1    1|
    +-------------------------*/
  REQUIRE(merged_node != nullptr);
  REQUIRE(merged_node->left != nullptr);
  REQUIRE(merged_node->right != nullptr);
  REQUIRE(merged_node->right->left != nullptr);
  REQUIRE(merged_node->right->left->left != nullptr);
  REQUIRE(merged_node->right->right != nullptr);
  REQUIRE(merged_node->right->right->left != nullptr);
  REQUIRE(merged_node->right->right->right != nullptr);

  REQUIRE(merged_node->left->pos() == 20);
  REQUIRE(merged_node->right->left->pos() == 3);
  REQUIRE(merged_node->right->left->left->pos() == 2);
  REQUIRE(merged_node->right->right->right->pos() == 1);
  REQUIRE(merged_node->right->right->left->pos() == 1);
}

}  // namespace CORE::Internal::tECS::UnitTests
