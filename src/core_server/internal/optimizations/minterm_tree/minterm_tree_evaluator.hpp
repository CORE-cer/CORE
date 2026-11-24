#pragma once

#include <z3++.h>

#include <algorithm>
#include <array>
#include <cstddef>
#include <cstdint>
#include <map>
#include <memory>
#include <optional>
#include <ostream>
#include <set>
#include <sstream>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include "core_server/internal/evaluation/physical_predicate/or_predicate.hpp"
#include "core_server/internal/evaluation/physical_predicate/physical_predicate.hpp"
#include "core_server/internal/optimizations/atom_ordering_strategy.hpp"
#include "core_server/internal/optimizations/minterm_tree/atom_extractor.hpp"
#include "core_server/internal/optimizations/minterm_tree/atom_ordering.hpp"
#include "core_server/internal/optimizations/minterm_tree/minterm_generator.hpp"
#include "core_server/internal/optimizations/minterm_tree/physical_predicate_z3_algebra.hpp"
#include "core_server/internal/optimizations/minterm_tree/physical_predicate_z3_translator.hpp"
#include "core_server/internal/optimizations/minterm_tree/speculation_safety.hpp"
#include "core_server/internal/optimizations/minterm_tree/tree.hpp"
#include "core_server/internal/optimizations/minterm_tree/tree_printing.hpp"
#include "core_server/internal/optimizations/optimized_predicate_evaluator.hpp"
#include "core_server/internal/optimizations/string_equality_strategy.hpp"
#include "core_server/internal/optimizations/tree_layout_strategy.hpp"
#include "shared/datatypes/aliases/event_type_id.hpp"
#include "shared/datatypes/bitset.hpp"
#include "shared/datatypes/eventWrapper.hpp"
#include "shared/profiling/zones.hpp"

namespace CORE::Internal::Optimizations::MintermTree {

// A faster, drop-in alternative to Evaluation::PredicateEvaluator: for the
// same predicates and the same event it returns the same Bitset (bit i is set
// iff predicate i is satisfied).
//
// THE PROBLEM IT ADDRESSES. PredicateEvaluator evaluates every predicate
// independently for every event, even when they are related (if `price > 100`
// is true, `price > 50` must be true too).
//
// THE IDEA. Some vocabulary:
//   atom     an indivisible predicate such as `Integer1 > 100`. Each query
//            predicate is an atom or an And/Or/Not combination of atoms.
//   minterm  one region of the event space, defined by saying for each atom
//            whether it is true or false. Only regions that can actually occur
//            exist (`price > 100` together with `NOT price > 50` cannot).
//   opaque   an atom the solver cannot reason about (regex, strings, ...). It
//            is still evaluated natively, it just is not related to others.
// We build a binary tree whose nodes each test one atom and whose leaves are the
// minterms. Deciding which minterm an event falls in needs only the atoms on
// ONE root-to-leaf path, and each leaf already knows which of the query's
// predicates are true there.
//
// TWO PHASES.
//   Build (once per query; Z3 is used only here):
//     1. Split the predicates: those that admit any event type (stream/event
//        name checks), and those containing an atom that could fail if it were
//        evaluated when the baseline would have skipped it (integer division
//        by a possibly-zero divisor, see speculation_safety.hpp), are simply
//        evaluated directly at runtime. (An Or that admits any event type but
//        whose atoms all name theirs is not one of those, see
//        direct_evaluation_reason.)
//     2. For each event type, take the atoms that can apply to it and translate
//        them into Z3 formulas (see PhysicalPredicateZ3Translator).
//     3. Refine a tree with those atoms (one split per atom, only where both
//        outcomes are possible).
//     4. For every leaf, precompute a bitmask: predicate i is set iff the leaf's
//        region implies predicate i's formula.
//     5. Collapse any node whose two children turned out to agree on every bit
//        (MintermTreeNode::reduce): a split that decides nothing is neither
//        stored nor evaluated again.
//   Run (per event, Z3 is never touched):
//     1. Evaluate the directly-evaluated predicates.
//     2. Walk the event's type tree from the root, evaluating each node's atom
//        natively to choose left (true) or right (false), down to a leaf.
//     3. OR the leaf's precomputed bitmask into the result.
//   How the tree is stored and walked is TreeLayoutStrategy (backlog item 26):
//   today's pointer nodes by default, or a flat array copied from them after
//   step 5 (build step 6). Every layout reaches the same leaf for an event.
//
// EVALUATION ORDER. The baseline is lazy (And/Or stop early), the tree is not:
// it evaluates every atom on its path. That is only safe for atoms that cannot
// fail, which is why the ones that can are kept out of the trees (step 1).
//
// SAFETY NET. If building a type's tree fails (Z3 cannot decide a formula, the
// tree would exceed kMaxLeavesPerTree, or a predicate is shaped in a way the
// tree cannot reproduce), events of that type are evaluated directly like the
// baseline does, so results are always the same.
//
// CORRECTNESS INVARIANT. The Z3 model may be less precise than reality (leading
// to fewer shared decisions) but must never be *stricter*: it must not claim a
// relation between atoms that native evaluation can contradict.
class MintermTreeEvaluator : public OptimizedPredicateEvaluator {
 public:
  // Trees with more leaves than this are abandoned (n independent atoms give
  // up to 2^n leaves), and their event type is evaluated directly instead.
  static constexpr size_t kMaxLeavesPerTree = 4096;

  // TreeLayoutStrategy::ArrayImplicit stores every slot of a full binary tree
  // of the tree's depth: 2^(depth+1) - 1 slots of 16 bytes. Past this depth
  // (32 MB) the type's tree uses ArrayCompact instead, and describe() says so.
  static constexpr size_t kMaxImplicitTreeDepth = 20;

  // `predicates` is the query's full predicate list; bit i of the result
  // corresponds to predicates[i]. The evaluator co-owns the predicates, so it
  // stays valid however the caller's copies come and go. `max_leaves_per_tree`
  // is only lowered by tests, to exercise the fallback cheaply. `atom_ordering`
  // and `string_equality` default to today's behaviour, unchanged; each comes
  // after the previous parameters so no existing call site that only passes
  // some of them needs to change. `tree_layout` (backlog item 26) only changes
  // how a built tree is stored and walked, never what it computes.
  explicit MintermTreeEvaluator(
    const std::vector<std::shared_ptr<CEA::PhysicalPredicate>>& predicates,
    size_t max_leaves_per_tree = kMaxLeavesPerTree,
    AtomOrderingStrategy atom_ordering = AtomOrderingStrategy::AsDiscovered,
    StringEqualityStrategy string_equality = StringEqualityStrategy::Opaque,
    TreeLayoutStrategy tree_layout = TreeLayoutStrategy::Pointer)
      : predicates_(predicates),
        max_leaves_per_tree_(max_leaves_per_tree),
        atom_ordering_(atom_ordering),
        string_equality_(string_equality),
        tree_layout_(tree_layout),
        translator_(ctx_, string_equality),
        algebra_(ctx_) {
    build();
  }

  MintermTreeEvaluator(const MintermTreeEvaluator&) = delete;
  MintermTreeEvaluator& operator=(const MintermTreeEvaluator&) = delete;

  Bitset operator()(Types::EventWrapper& event) override {
    ZoneScopedN("MintermTree::eval");
    Bitset result(predicates_.size());

    // Predicates that admit any event type are always evaluated directly.
    {
      CORE_ZONE_FINE("MintermTree::catch_all");
      for (const auto& [index, predicate] : catch_all_) {
        if ((*predicate)(event)) result.set(index);
      }
    }

    Types::UniqueEventTypeId event_type = event.get_unique_event_type_id();

    // The layout is fixed for the evaluator's life, so this branch always goes
    // the same way and costs (next to) nothing; every layout pays it equally.
    if (tree_layout_ == TreeLayoutStrategy::Pointer
        || tree_layout_ == TreeLayoutStrategy::PointerDirectEval) {
      auto tree = trees_.find(event_type);
      if (tree != trees_.end()) {
        CORE_ZONE_FINE("MintermTree::tree_walk");
        // Walk down the tree: each internal node's atom decides left or right.
        MintermTreeNode<z3::expr>* node = tree->second.get();
        if (tree_layout_ == TreeLayoutStrategy::Pointer) {
          // operator() re-checks that the atom admits this event type.
          while (!node->isLeaf()) {
            node = (*node->split_atom)(event) ? node->left.get() : node->right.get();
          }
        } else {
          // eval() skips that check: every atom in this type's tree admits the
          // type (build_tree_for_event_type selects only those), so the check
          // could only ever pass.
          while (!node->isLeaf()) {
            node = node->split_atom->eval(event) ? node->left.get() : node->right.get();
          }
        }
        // The leaf already knows which predicates hold in its region.
        result |= node->satisfied_predicates;
        return result;
      }
    } else {
      auto flat = flat_trees_.find(event_type);
      if (flat != flat_trees_.end()) {
        CORE_ZONE_FINE("MintermTree::tree_walk");
        // The atom's truth value comes straight from eval() (see flat_walk).
        result |= flat->second.leaf_bits[flat_walk</*kCountSteps=*/false>(
          flat->second, [&event](CEA::PhysicalPredicate* atom, size_t /*index*/) {
            return atom->eval(event);
          })];
        return result;
      }
    }

    if (direct_event_types_.contains(event_type)) {
      CORE_ZONE_FINE("MintermTree::direct_fallback");
      // No tree for this event type (see SAFETY NET): evaluate as the baseline.
      for (size_t index : optimizable_indices_) {
        if ((*predicates_[index])(event)) result.set(index);
      }
    }
    // Otherwise no optimizable predicate admits this event type: all bits 0.
    return result;
  }

  // Debug-only: walks the tree exactly like operator() does for this event,
  // but returns how many decisions were taken instead of the result Bitset.
  // Mirrors operator()'s tree-walk branch line for line; never called from
  // operator() itself, so the hot, timed path stays exactly as it was. Exists
  // only for the ground-truth path-length validation
  // (minterm_tree_path_length.cpp), which checks the average of this value
  // over real events against tests/benchmark/path_length_model.py's
  // analytical estimate of the same quantity. Returns 0 if this event type
  // has no tree (falls back to direct evaluation, or no optimizable
  // predicate admits it) - neither case arises for the queries this is
  // actually run on, but a tree-less event type has no decisions to count
  // either way.
  //
  // With an array layout it counts the decisions of the array walk instead
  // (the same walk as flat_walk, counted), so tests can check that every
  // layout takes exactly the same number of decisions as the pointer tree.
  size_t debug_path_length(Types::EventWrapper& event) override {
    Types::UniqueEventTypeId event_type = event.get_unique_event_type_id();
    if (tree_layout_ == TreeLayoutStrategy::ArrayCompact
        || tree_layout_ == TreeLayoutStrategy::ArrayImplicit) {
      auto flat = flat_trees_.find(event_type);
      if (flat == flat_trees_.end()) return 0;
      size_t steps = 0;
      flat_walk</*kCountSteps=*/true>(
        flat->second,
        [&event](CEA::PhysicalPredicate* atom, size_t /*index*/) {
          return atom->eval(event);
        },
        &steps);
      return steps;
    }
    auto tree = trees_.find(event_type);
    if (tree == trees_.end()) return 0;
    size_t steps = 0;
    MintermTreeNode<z3::expr>* node = tree->second.get();
    while (!node->isLeaf()) {
      steps++;
      node = (*node->split_atom)(event) ? node->left.get() : node->right.get();
    }
    return steps;
  }

  // Number of leaves in the tree built for `event_type` (0 if none). Lets tests
  // show that correlated atoms do not blow up into every combination.
  size_t debug_leaf_count(Types::UniqueEventTypeId event_type) {
    auto tree = trees_.find(event_type);
    if (tree == trees_.end()) return 0;
    std::vector<MintermTreeNode<z3::expr>*> leaves;
    tree->second->collectLeaves(leaves);
    return leaves.size();
  }

  // How many internal nodes MintermTreeNode::reduce() collapsed into leaves
  // for event_type's tree (0 if none, or if no tree exists for that type).
  size_t debug_reduced_node_count(Types::UniqueEventTypeId event_type) const {
    auto it = reduced_nodes_per_type_.find(event_type);
    return it == reduced_nodes_per_type_.end() ? 0 : it->second;
  }

  // The atoms admitting event_type, as to_string(), in the order they were
  // actually refined in (whatever AtomOrderingStrategy produced it). Empty if
  // the type has no tree.
  std::vector<std::string> debug_atom_order(Types::UniqueEventTypeId event_type) const {
    auto it = atom_order_per_type_.find(event_type);
    return it == atom_order_per_type_.end() ? std::vector<std::string>{} : it->second;
  }

  // The layout `event_type`'s tree is actually walked with: tree_layout_,
  // except that ArrayImplicit falls back to ArrayCompact for a tree deeper
  // than kMaxImplicitTreeDepth. Meaningless for a type without a tree.
  TreeLayoutStrategy debug_layout_in_use(Types::UniqueEventTypeId event_type) const {
    auto flat = flat_trees_.find(event_type);
    if (flat != flat_trees_.end() && tree_layout_ == TreeLayoutStrategy::ArrayImplicit
        && !flat->second.implicit) {
      return TreeLayoutStrategy::ArrayCompact;
    }
    return tree_layout_;
  }

  // ---- Walk benchmark hooks (core_bench --method walk / walk_eval) ---------
  //
  // They answer "what does the WALK cost, apart from the atoms?" (backlog
  // item 26's double check). A decision is the atom's eval() plus a few
  // instructions of walking; with ~30 ns atoms a layout's effect on those few
  // instructions is lost in the noise. So core_bench first records, for each
  // event, the outcome of every atom of its type's tree as one bit of a mask
  // (debug_atom_mask, untimed), then times the walk with each atom's eval()
  // replaced by reading its bit (debug_walk_with_masks). That leaves exactly
  // the traversal: the node loads, the index arithmetic and the loop exit, on
  // the real trees and the real sequence of outcomes. debug_walk_with_eval
  // times the same loop with the real atoms, so the difference between the
  // two is the atoms' share. Never called by the engine.

  // Bit k = the outcome of atom k of this event's type tree (atom ids follow
  // the order atoms were refined in). nullopt if the type has no tree or more
  // than 63 atoms (the array walk reads bit id + 1, see tagged_flat_trees_).
  std::optional<uint64_t> debug_atom_mask(Types::EventWrapper& event) override {
    auto atoms = atoms_per_type_.find(event.get_unique_event_type_id());
    if (atoms == atoms_per_type_.end() || atoms->second.size() > 63
        || !trees_.contains(event.get_unique_event_type_id())) {
      return std::nullopt;
    }
    uint64_t mask = 0;
    for (size_t k = 0; k < atoms->second.size(); k++) {
      if (atoms->second[k]->eval(event)) mask |= uint64_t{1} << k;
    }
    return mask;
  }

  // Walks `event_type`'s tree once per mask with the selected layout, reading
  // each atom's outcome from the mask. Returns the total number of decisions
  // (equal to the sum of debug_path_length over the same events, which
  // core_bench checks), so the work cannot be optimized away.
  size_t debug_walk_with_masks(Types::UniqueEventTypeId event_type,
                               const std::vector<uint64_t>& masks) override {
    size_t decisions = 0;
    if (tree_layout_ == TreeLayoutStrategy::Pointer
        || tree_layout_ == TreeLayoutStrategy::PointerDirectEval) {
      const MintermTreeNode<z3::expr>* root = trees_.at(event_type).get();
      for (uint64_t mask : masks) {
        decisions += pointer_walk_steps(root,
                                        [mask](const MintermTreeNode<z3::expr>* node) {
                                          return ((mask >> node->atom_id) & 1U) != 0;
                                        });
      }
      return decisions;
    }
    // The tagged copy: each node's atom field holds (atom id + 1), so reading
    // the outcome takes exactly the loads the real walk takes to reach the
    // atom (see tagged_flat_trees_). Bit id + 1 of the shifted mask is atom
    // id's outcome.
    const FlatTree& tree = tagged_flat_trees_.at(event_type);
    for (uint64_t mask : masks) {
      const uint64_t shifted = mask << 1U;
      flat_walk</*kCountSteps=*/true>(
        tree,
        [shifted](CEA::PhysicalPredicate* tag, size_t /*index*/) {
          // NOLINTNEXTLINE(performance-no-int-to-ptr): the tag is an id, not a pointer
          return ((shifted >> reinterpret_cast<uintptr_t>(tag)) & 1U) != 0;
        },
        &decisions);
    }
    return decisions;
  }

  // The same loop with the real atoms, called the way each layout calls them
  // (operator() for Pointer, eval() otherwise). Returns the total decisions.
  size_t debug_walk_with_eval(Types::UniqueEventTypeId event_type,
                              std::vector<Types::EventWrapper>& events) override {
    size_t decisions = 0;
    if (tree_layout_ == TreeLayoutStrategy::Pointer) {
      const MintermTreeNode<z3::expr>* root = trees_.at(event_type).get();
      for (Types::EventWrapper& event : events) {
        decisions += pointer_walk_steps(root,
                                        [&event](const MintermTreeNode<z3::expr>* node) {
                                          return (*node->split_atom)(event);
                                        });
      }
      return decisions;
    }
    if (tree_layout_ == TreeLayoutStrategy::PointerDirectEval) {
      const MintermTreeNode<z3::expr>* root = trees_.at(event_type).get();
      for (Types::EventWrapper& event : events) {
        decisions += pointer_walk_steps(root,
                                        [&event](const MintermTreeNode<z3::expr>* node) {
                                          return node->split_atom->eval(event);
                                        });
      }
      return decisions;
    }
    const FlatTree& tree = flat_trees_.at(event_type);
    for (Types::EventWrapper& event : events) {
      flat_walk</*kCountSteps=*/true>(
        tree,
        [&event](CEA::PhysicalPredicate* atom, size_t /*index*/) {
          return atom->eval(event);
        },
        &decisions);
    }
    return decisions;
  }

  // True if `event_type` has no tree and is evaluated directly (SAFETY NET).
  bool uses_direct_evaluation(Types::UniqueEventTypeId event_type) const {
    return direct_event_types_.contains(event_type);
  }

  // One-line summary, useful to confirm the optimization is active in a run.
  std::string describe() const {
    std::string out = "MintermTreeEvaluator: " + std::to_string(predicates_.size())
                      + " predicates (" + std::to_string(catch_all_.size())
                      + " evaluated directly), " + std::to_string(atom_count_)
                      + " atoms (" + std::to_string(translator_.opaque_atom_count())
                      + " opaque), " + std::to_string(trees_.size()) + " trees with "
                      + std::to_string(total_leaves_) + " leaves in total (largest "
                      + std::to_string(max_leaves_) + ", "
                      + std::to_string(reduced_nodes_) + " reduced)";
    // Only mentioned when non-default, so the printed text for the default
    // strategy (AsDiscovered) stays byte-identical to before this option
    // existed - no existing exact-text test needs to change for it.
    if (atom_ordering_ == AtomOrderingStrategy::MostSharedAttributesFirst) {
      out += "; atom ordering: most-shared-attributes-first";
    }
    // Same convention: only mentioned when non-default.
    if (string_equality_ == StringEqualityStrategy::InternedEquality) {
      out += "; string equality: interned";
    }
    // Same convention: only mentioned when non-default.
    switch (tree_layout_) {
      case TreeLayoutStrategy::Pointer:
        break;
      case TreeLayoutStrategy::PointerDirectEval:
        out += "; tree layout: pointer, direct eval";
        break;
      case TreeLayoutStrategy::ArrayCompact:
        out += "; tree layout: compact array";
        break;
      case TreeLayoutStrategy::ArrayImplicit:
        out += "; tree layout: implicit array";
        break;
    }
    for (const auto& [event_type, depth] : implicit_fallbacks_) {
      out += "; event type " + std::to_string(event_type) + " too deep for the implicit "
             "array (depth " + std::to_string(depth) + "), compact array used";
    }
    for (const auto& [event_type, reason] : direct_event_types_) {
      out += "; event type " + std::to_string(event_type) + " evaluated directly ("
             + reason + ")";
    }
    return out;
  }

  // The trees this evaluator built, in an orderly, readable form, for debugging
  // (mostly from unit tests: it shows what the optimization built for a set of
  // predicates). For example, for the predicates `x > 100`, `x > 50`, `x < 10`
  // on one event type:
  //
  //   MintermTreeEvaluator: 3 predicates (0 evaluated directly), 3 atoms (0 opaque), ...
  //
  //   Predicates
  //     [0] Event[1] > 100   (tree)
  //     [1] Event[1] > 50   (tree)
  //     [2] Event[1] < 10   (tree)
  //
  //   Event type 0: 4 leaves, 3 atoms
  //     Event[1] > 100 ?
  //     +-- true:  leaf #0 -> p0 p1   region: ...
  //     `-- false: Event[1] > 50 ?
  //         +-- true:  leaf #1 -> p1   region: ...
  //         `-- false: Event[1] < 10 ?
  //             +-- true:  leaf #2 -> p2   region: ...
  //             `-- false: leaf #3 -> (none)   region: ...
  //
  // Each internal node is the condition tested for an event (left = true, right
  // = false), each leaf lists the predicates (p<index>) that hold in its region.
  // Opaque atoms are tagged [opaque]; predicates and event types evaluated
  // directly say why. Read-only: printing never changes how events are evaluated.
  std::string trees_to_string(const TreePrintOptions& options = {}) const {
    std::ostringstream out;
    print_trees(out, options);
    return out.str();
  }

  void print_trees(std::ostream& out, const TreePrintOptions& options = {}) const {
    out << describe() << "\n\nPredicates\n";
    for (size_t i = 0; i < predicates_.size(); i++) {
      out << "  [" << i << "] " << predicates_[i]->to_string() << "   ";
      auto reason = catch_all_reasons_.find(i);
      if (reason != catch_all_reasons_.end()) {
        out << "(direct: " << reason->second << ")\n";
      } else {
        out << "(tree)\n";
      }
    }

    std::set<Types::UniqueEventTypeId> event_types;
    for (const auto& [event_type, tree] : trees_) event_types.insert(event_type);
    for (const auto& [event_type, reason] : direct_event_types_)
      event_types.insert(event_type);

    MintermTreeNode<z3::expr>::Printer printer = make_printer(options);
    for (Types::UniqueEventTypeId event_type : event_types) {
      out << "\nEvent type " << event_type;
      auto tree = trees_.find(event_type);
      if (tree == trees_.end()) {
        out << ": evaluated directly (" << direct_event_types_.at(event_type) << ")\n";
        continue;
      }
      std::vector<const MintermTreeNode<z3::expr>*> leaves;
      tree->second->collectLeaves(leaves);
      std::set<const CEA::PhysicalPredicate*> atoms;
      collect_split_atoms(*tree->second, atoms);
      out << ": " << leaves.size() << " leaves, " << atoms.size() << " atoms\n";
      tree->second->print(out, printer, "  ");
    }
  }

 private:
  // Declaration order matters for destruction: the trees (which hold Z3
  // expressions) must be destroyed before the Z3 context they belong to, and
  // the predicates outlive everything that points into them.
  std::vector<std::shared_ptr<CEA::PhysicalPredicate>> predicates_;
  size_t max_leaves_per_tree_;
  AtomOrderingStrategy atom_ordering_;
  StringEqualityStrategy string_equality_;
  TreeLayoutStrategy tree_layout_;
  z3::context ctx_;
  PhysicalPredicateZ3Translator translator_;
  PhysicalPredicateZ3Algebra algebra_;

  // Predicates evaluated directly on every event (index, predicate).
  std::vector<std::pair<size_t, CEA::PhysicalPredicate*>> catch_all_;
  // Why each of those is evaluated directly (index -> reason, for printing).
  std::map<size_t, std::string> catch_all_reasons_;
  // Indices of the predicates the trees are responsible for.
  std::vector<size_t> optimizable_indices_;
  // One minterm tree per event type.
  std::unordered_map<Types::UniqueEventTypeId, std::unique_ptr<MintermTreeNode<z3::expr>>>
    trees_;
  // Event types without a tree, with the reason (for describe()).
  std::map<Types::UniqueEventTypeId, std::string> direct_event_types_;

  // ---- Array layouts (TreeLayoutStrategy::ArrayCompact / ArrayImplicit) ----
  //
  // The same tree as trees_ (built, then reduced), copied into contiguous
  // memory so the walk follows indices instead of unique_ptrs. trees_ keeps
  // the pointer version, which printing and the debug_* methods read. Only
  // filled when an array layout is selected.

  // ArrayCompact: one entry per INTERNAL node, in breadth-first order (the
  // root, if it is not a leaf, is nodes[0]). Leaves are not nodes: a child
  // index >= 0 is the next internal node, a negative one is ~leaf_index into
  // leaf_bits. child[1] is the "atom true" side (the pointer tree's left),
  // child[0] the "atom false" side, so the atom's bool picks the child
  // directly, with no if/else on its outcome.
  struct FlatNode {
    CEA::PhysicalPredicate* atom;
    std::array<int32_t, 2> child;
  };

  // ArrayImplicit: slot i of a full binary tree; its children are 2i+1 (atom
  // false) and 2i+2 (atom true), so no child index is stored and the next
  // slot is simply 2i + 1 + atom(event). atom == nullptr marks a leaf, whose
  // bits are leaf_bits[leaf]. Slots under a leaf are never reached and stay
  // empty.
  struct ImplicitSlot {
    CEA::PhysicalPredicate* atom = nullptr;
    int32_t leaf = -1;
  };

  struct FlatTree {
    bool implicit = false;            // which of the two arrays below is used
    std::vector<FlatNode> nodes;      // ArrayCompact
    int32_t root = 0;                 // ArrayCompact: nodes index, or ~leaf
    std::vector<ImplicitSlot> slots;  // ArrayImplicit
    std::vector<Bitset> leaf_bits;    // both: leaf index -> satisfied predicates
  };

  std::unordered_map<Types::UniqueEventTypeId, FlatTree> flat_trees_;
  // Walk benchmark only: per event type, its tree's atoms in refine order;
  // atom k is bit k of debug_atom_mask's mask and every node's atom_id.
  std::unordered_map<Types::UniqueEventTypeId, std::vector<CEA::PhysicalPredicate*>>
    atoms_per_type_;
  // Walk benchmark only (array layouts): a copy of each flat tree in which
  // every atom pointer is replaced by the tag (atom id + 1), never a real
  // address. The mask walk reads the tag exactly where the real walk reads
  // the atom pointer, so both do the same loads in the same order; only the
  // virtual eval() call becomes a shift. (+1 keeps an implicit leaf, whose
  // atom is nullptr, distinguishable from atom 0.)
  std::unordered_map<Types::UniqueEventTypeId, FlatTree> tagged_flat_trees_;

  static FlatTree
  tag_atoms(FlatTree tree, const std::vector<CEA::PhysicalPredicate*>& atoms) {
    std::unordered_map<const CEA::PhysicalPredicate*, uintptr_t> tag;
    for (size_t k = 0; k < atoms.size(); k++) tag.emplace(atoms[k], k + 1);
    auto as_tag = [&tag](CEA::PhysicalPredicate* atom) {
      // NOLINTNEXTLINE(performance-no-int-to-ptr): a tag, never dereferenced
      return reinterpret_cast<CEA::PhysicalPredicate*>(tag.at(atom));
    };
    for (FlatNode& node : tree.nodes) node.atom = as_tag(node.atom);
    for (ImplicitSlot& slot : tree.slots) {
      if (slot.atom != nullptr) slot.atom = as_tag(slot.atom);
    }
    return tree;
  }

  // Sets every internal node's atom_id to its atom's position in `ids`.
  static void
  assign_atom_ids(MintermTreeNode<z3::expr>& node,
                  const std::unordered_map<const CEA::PhysicalPredicate*, uint32_t>& ids) {
    if (node.isLeaf()) return;
    node.atom_id = ids.at(node.split_atom);
    assign_atom_ids(*node.left, ids);
    assign_atom_ids(*node.right, ids);
  }

  // Types whose tree was too deep for ArrayImplicit (type -> depth).
  std::map<Types::UniqueEventTypeId, size_t> implicit_fallbacks_;

  // The array walk: returns the index (into leaf_bits) of the leaf reached.
  // `outcome(atom, index)` gives the truth value of the node's atom, where
  // index is the node's position in tree.nodes / tree.slots. The engine passes
  // a lambda calling atom->eval(event) and ignoring index; the walk benchmark
  // passes one that reads the atom's bit from a recorded mask instead. Both
  // are inlined, so the engine's walk is exactly the code below with eval().
  // With kCountSteps (debug and benchmark only) it also counts the decisions
  // into *steps; the engine instantiates it with false, so its walk contains
  // no counting at all.
  template <bool kCountSteps, typename Outcome>
  static size_t
  flat_walk(const FlatTree& tree, Outcome&& outcome, size_t* steps = nullptr) {
    if (tree.implicit) {
      size_t i = 0;
      while (tree.slots[i].atom != nullptr) {
        if constexpr (kCountSteps) (*steps)++;
        // false -> 2i+1, true -> 2i+2: the outcome (0 or 1) is added, no if.
        i = 2 * i + 1 + static_cast<size_t>(outcome(tree.slots[i].atom, i));
      }
      return static_cast<size_t>(tree.slots[i].leaf);
    }
    int32_t i = tree.root;
    while (i >= 0) {
      if constexpr (kCountSteps) (*steps)++;
      const auto at = static_cast<size_t>(i);
      const FlatNode& node = tree.nodes[at];
      i = node.child[static_cast<size_t>(outcome(node.atom, at))];
    }
    const int32_t leaf = ~i;  // a leaf is stored as ~(its index), always >= 0 here
    return static_cast<size_t>(leaf);
  }

  // The pointer tree's walk with the outcome supplied by `outcome(node)`, as
  // flat_walk. Benchmark only (debug_walk_with_*): the engine's own pointer
  // walk in operator() stays as it was. Returns the number of decisions.
  template <typename Outcome>
  static size_t
  pointer_walk_steps(const MintermTreeNode<z3::expr>* node, Outcome&& outcome) {
    size_t steps = 0;
    while (!node->isLeaf()) {
      steps++;
      node = outcome(node) ? node->left.get() : node->right.get();
    }
    return steps;
  }

  // Copies a built (and reduced) pointer tree into the compact array, breadth
  // first: `order` lists the internal nodes in the order they get their index,
  // so nodes[k] always describes order[k].
  static FlatTree flatten_compact(const MintermTreeNode<z3::expr>& root) {
    FlatTree out;
    if (root.isLeaf()) {
      out.leaf_bits.push_back(root.satisfied_predicates);
      out.root = ~int32_t{0};
      return out;
    }
    std::vector<const MintermTreeNode<z3::expr>*> order{&root};
    // A child's index: a new leaf_bits entry for a leaf, or its future place in
    // `order`/nodes for an internal node.
    auto index_of = [&](const MintermTreeNode<z3::expr>* child) -> int32_t {
      if (child->isLeaf()) {
        out.leaf_bits.push_back(child->satisfied_predicates);
        return ~static_cast<int32_t>(out.leaf_bits.size() - 1);
      }
      order.push_back(child);
      return static_cast<int32_t>(order.size() - 1);
    };
    for (size_t k = 0; k < order.size(); k++) {
      const MintermTreeNode<z3::expr>* node = order[k];
      FlatNode flat{node->split_atom, {0, 0}};
      flat.child[1] = index_of(node->left.get());   // atom true
      flat.child[0] = index_of(node->right.get());  // atom false
      out.nodes.push_back(flat);
    }
    out.root = 0;
    return out;
  }

  // Number of decisions on the longest root-to-leaf path (0 for a lone leaf).
  static size_t tree_depth(const MintermTreeNode<z3::expr>& node) {
    if (node.isLeaf()) return 0;
    return 1 + std::max(tree_depth(*node.left), tree_depth(*node.right));
  }

  // Copies a built (and reduced) pointer tree into the implicit array. The
  // caller has checked the depth, so the array (2^(depth+1) - 1 slots) is
  // bounded.
  static FlatTree flatten_implicit(const MintermTreeNode<z3::expr>& root, size_t depth) {
    FlatTree out;
    out.implicit = true;
    out.slots.resize((size_t{1} << (depth + 1)) - 1);
    place_implicit(root, 0, out);
    return out;
  }

  static void
  place_implicit(const MintermTreeNode<z3::expr>& node, size_t slot, FlatTree& out) {
    if (node.isLeaf()) {
      out.leaf_bits.push_back(node.satisfied_predicates);
      out.slots[slot].leaf = static_cast<int32_t>(out.leaf_bits.size() - 1);
      return;
    }
    out.slots[slot].atom = node.split_atom;
    place_implicit(*node.right, 2 * slot + 1, out);  // atom false
    place_implicit(*node.left, 2 * slot + 2, out);   // atom true
  }

  size_t atom_count_ = 0;
  size_t total_leaves_ = 0;
  size_t max_leaves_ = 0;
  size_t reduced_nodes_ = 0;
  // Per event type: how many internal nodes MintermTreeNode::reduce()
  // collapsed for that type's tree. Absent for a type with no tree.
  std::unordered_map<Types::UniqueEventTypeId, size_t> reduced_nodes_per_type_;
  // Per event type: the atoms, as to_string(), in the order they were refined
  // in (see debug_atom_order). Testing only.
  std::unordered_map<Types::UniqueEventTypeId, std::vector<std::string>> atom_order_per_type_;

  void build() {
    ZoneScopedN("MintermTree::build");
    // Step 1: decide which predicates the trees handle. A predicate that admits
    // any event type, or that contains an atom doing so, cannot be tied to one
    // event type's tree, so it is evaluated directly.
    std::vector<CEA::PhysicalPredicate*> optimizable_roots;
    for (size_t i = 0; i < predicates_.size(); i++) {
      CEA::PhysicalPredicate* predicate = predicates_[i].get();
      std::optional<std::string> reason = direct_evaluation_reason(predicate);
      if (!reason.has_value()) {
        optimizable_indices_.push_back(i);
        optimizable_roots.push_back(predicate);
      } else {
        catch_all_.emplace_back(i, predicate);
        catch_all_reasons_[i] = *reason;
      }
    }

    std::vector<CEA::PhysicalPredicate*> atoms = GetAllAtomicPhysicalPredicates(
      optimizable_roots);
    atom_count_ = atoms.size();

    // The event types we must handle: any type some optimizable predicate or
    // atom admits.
    std::set<Types::UniqueEventTypeId> event_types;
    for (CEA::PhysicalPredicate* root : optimizable_roots) {
      event_types.insert(root->admissible_event_types.begin(),
                         root->admissible_event_types.end());
    }
    for (CEA::PhysicalPredicate* atom : atoms) {
      event_types.insert(atom->admissible_event_types.begin(),
                         atom->admissible_event_types.end());
    }

    for (Types::UniqueEventTypeId event_type : event_types) {
      try {
        build_tree_for_event_type(event_type, atoms);
      } catch (const SatUnknownError& error) {
        use_direct_evaluation(event_type, error.what());
      } catch (const MintermTreeTooLargeError& error) {
        use_direct_evaluation(event_type, error.what());
      } catch (const z3::exception& error) {
        use_direct_evaluation(event_type, error.msg());
      }
    }
  }

  // Why a predicate must be evaluated directly, or nullopt if the trees can
  // handle it. A predicate is optimizable if it names specific event types and so
  // do all of its atoms, and none of its atoms can fail when evaluated
  // speculatively (a division that the baseline's short-circuit might have
  // guarded).
  //
  // One predicate that admits any event type is nevertheless handled by the trees:
  // an Or. The CEQL visitor for weakly typed filters builds `alias[a OR b]` with
  // the constructor that admits every event type (it computes the union of the
  // children's event types but does not apply it). That is harmless at runtime
  // because an Or evaluates its children through operator(), which returns false
  // for event types a child does not admit, so the Or is false for every event
  // type none of its atoms names. The translation mirrors exactly that (gated
  // children, see PhysicalPredicate::translate), so as long as every atom names
  // its event types the trees can handle it. Any other predicate that admits
  // every event type (stream/event name checks, ...) depends on the event itself
  // and stays direct.
  static std::optional<std::string>
  direct_evaluation_reason(CEA::PhysicalPredicate* predicate) {
    const std::string admits_any = "it admits any event type";
    if (predicate->admits_any_event_type
        && dynamic_cast<const CEA::OrPredicate*>(predicate) == nullptr) {
      return admits_any;
    }
    std::vector<CEA::PhysicalPredicate*> atoms;
    GetAllAtomicPhysicalPredicates(predicate, atoms);
    for (CEA::PhysicalPredicate* atom : atoms) {
      if (atom->admits_any_event_type) return admits_any;
      if (!speculation_safe(atom)) {
        return "an atom could fail if evaluated early (integer division or modulo by an "
               "unknown divisor)";
      }
    }
    return std::nullopt;
  }

  // ---- Printing -------------------------------------------------------------

  // The distinct atoms tested somewhere in a tree.
  static void collect_split_atoms(const MintermTreeNode<z3::expr>& node,
                                  std::set<const CEA::PhysicalPredicate*>& atoms) {
    if (node.isLeaf()) return;
    atoms.insert(node.split_atom);
    collect_split_atoms(*node.left, atoms);
    collect_split_atoms(*node.right, atoms);
  }

  // A formula on one line, cut to `max_chars` (Z3 prints multi-line, indented).
  static std::string compact(const std::string& text, size_t max_chars) {
    std::string out;
    for (char c : text) {
      bool space = c == ' ' || c == '\n' || c == '\t';
      if (space && (out.empty() || out.back() == ' ')) continue;
      out.push_back(space ? ' ' : c);
    }
    if (!out.empty() && out.back() == ' ') out.pop_back();
    if (out.size() > max_chars) out = out.substr(0, max_chars) + "...";
    return out;
  }

  MintermTreeNode<z3::expr>::Printer make_printer(const TreePrintOptions& options) const {
    MintermTreeNode<z3::expr>::Printer printer;
    printer.atom_label = [this](const CEA::PhysicalPredicate* atom) {
      return atom->to_string() + (translator_.is_opaque(atom) ? " [opaque]" : "");
    };
    printer.leaf_label = [this](const MintermTreeNode<z3::expr>& leaf) {
      std::string bits;
      for (size_t i = 0; i < predicates_.size() && i < leaf.satisfied_predicates.size();
           i++) {
        if (!leaf.satisfied_predicates.test(i)) continue;
        if (!bits.empty()) bits += " ";
        bits += "p" + std::to_string(i);
      }
      return bits.empty() ? std::string("(none)") : bits;
    };
    if (options.show_regions) {
      printer.region_label =
        [max_chars = options.max_region_chars](const z3::expr& region) {
          return compact(region.to_string(), max_chars);
        };
    }
    return printer;
  }

  // Can the tree reproduce `node`'s evaluation for events of `event_type`?
  // The tree only knows the atoms that admit the event type. At runtime, though,
  // the children of And/Not are evaluated without any type check (eval()), so
  // such a child that does not admit the type would be evaluated but missing
  // from the tree. The CEQL visitors never build that shape (an And admits the
  // intersection of its children's types), but hand-built predicates can.
  static bool tree_can_reproduce(const CEA::PhysicalPredicate* node,
                                 Types::UniqueEventTypeId event_type,
                                 bool gated) {
    bool admitted = node->admits_any_event_type
                    || node->admissible_event_types.contains(event_type);
    // Gated and not admitted: never evaluated, its formula is a constant false.
    if (gated && !admitted) return true;
    if (!node->is_compound()) return admitted;

    // Or evaluates its children through operator() (gated); And/Not do not.
    bool children_gated = dynamic_cast<const CEA::OrPredicate*>(node) != nullptr;
    for (const CEA::PhysicalPredicate* child : node->get_children()) {
      if (!tree_can_reproduce(child, event_type, children_gated)) return false;
    }
    return true;
  }

  void
  use_direct_evaluation(Types::UniqueEventTypeId event_type, const std::string& reason) {
    trees_.erase(event_type);
    flat_trees_.erase(event_type);
    tagged_flat_trees_.erase(event_type);
    direct_event_types_[event_type] = reason;
  }

  // Steps 2-6 of the build phase for one event type.
  void build_tree_for_event_type(Types::UniqueEventTypeId event_type,
                                 const std::vector<CEA::PhysicalPredicate*>& atoms) {
    ZoneScopedN("MintermTree::build_type");
    for (size_t index : optimizable_indices_) {
      if (!tree_can_reproduce(predicates_[index].get(), event_type, /*gated=*/true)) {
        use_direct_evaluation(event_type, "predicate shape the tree cannot reproduce");
        return;
      }
    }

    std::vector<CEA::PhysicalPredicate*> selected_atoms;
    std::vector<z3::expr> selected_formulas;
    std::vector<std::pair<size_t, z3::expr>> predicate_formulas;
    {
      ZoneScopedN("MintermTree::z3_translate");
      // Step 2: the atoms that can apply to this event type, as Z3 formulas.
      for (CEA::PhysicalPredicate* atom : atoms) {
        if (!atom->admissible_event_types.contains(event_type)) continue;
        selected_atoms.push_back(atom);
        selected_formulas.push_back(translator_.translate_atom(atom, event_type));
      }

      // Which predicates can fire for this event type, as Z3 formulas. Computed
      // once here rather than once per leaf.
      for (size_t index : optimizable_indices_) {
        const CEA::PhysicalPredicate* predicate = predicates_[index].get();
        // A predicate that admits every event type (a weakly typed Or, see
        // direct_evaluation_reason) applies here too: its formula is false unless
        // one of its atoms names this event type.
        if (!predicate->admits_any_event_type
            && !predicate->admissible_event_types.contains(event_type)) {
          continue;
        }
        predicate_formulas.emplace_back(index,
                                        translator_.translate_formula(predicate,
                                                                      event_type));
      }
    }

    if (selected_atoms.empty()) {
      // Nothing to split on (should not happen for well-formed queries); stay
      // safe by evaluating this event type directly.
      if (!predicate_formulas.empty()) {
        use_direct_evaluation(event_type, "no atoms to build a tree from");
      }
      return;
    }

    // Step 2b: optionally reorder the atoms before refining. The final leaf
    // count never depends on this order (see atom_ordering.hpp), but which
    // leaves end up siblings does, and reduce() (step 5b below) only merges
    // siblings - so a better order can leave it with more to collapse.
    if (atom_ordering_ == AtomOrderingStrategy::MostSharedAttributesFirst) {
      std::vector<std::set<std::pair<Types::UniqueEventTypeId, size_t>>> keys;
      keys.reserve(selected_formulas.size());
      for (const z3::expr& formula : selected_formulas) {
        keys.push_back(translator_.attribute_keys_of(formula));
      }
      std::vector<size_t> order = order_by_shared_keys(keys);

      std::vector<CEA::PhysicalPredicate*> reordered_atoms;
      std::vector<z3::expr> reordered_formulas;
      reordered_atoms.reserve(order.size());
      reordered_formulas.reserve(order.size());
      for (size_t index : order) {
        reordered_atoms.push_back(selected_atoms[index]);
        reordered_formulas.push_back(selected_formulas[index]);
      }
      selected_atoms = std::move(reordered_atoms);
      selected_formulas = std::move(reordered_formulas);
    }

    // Record the order atoms are actually refined in, whatever the strategy,
    // for debug_atom_order() (tests only; never read at runtime).
    std::vector<std::string> atom_order;
    atom_order.reserve(selected_atoms.size());
    for (CEA::PhysicalPredicate* atom : selected_atoms)
      atom_order.push_back(atom->to_string());
    atom_order_per_type_[event_type] = std::move(atom_order);

    // Step 3: refine a tree with the atoms.
    std::unique_ptr<MintermTreeNode<z3::expr>> tree;
    {
      ZoneScopedN("MintermTree::refine");
      tree = buildMintermTree<z3::expr>(selected_atoms,
                                        selected_formulas,
                                        algebra_,
                                        max_leaves_per_tree_);
    }

    // Step 4: per-leaf bitmask. A leaf sets bit i when its region implies
    // predicate i's formula, i.e. when (leaf AND NOT formula) is unsatisfiable.
    std::vector<MintermTreeNode<z3::expr>*> leaves;
    {
      ZoneScopedN("MintermTree::leaf_bitmasks");
      tree->collectLeaves(leaves);
      for (MintermTreeNode<z3::expr>* leaf : leaves) {
        Bitset bits(predicates_.size());
        for (const auto& [index, formula] : predicate_formulas) {
          if (!algebra_.isSat(algebra_.And(leaf->phi, algebra_.Not(formula)))) {
            bits.set(index);
          }
        }
        leaf->satisfied_predicates = std::move(bits);
      }
    }

    // Step 5b: some leaves agree on every bit after all (e.g. "a AND b" with
    // a, b independent: every leaf where a is false answers "none" regardless
    // of b) - collapse those sibling pairs. Must run after the bitmasks above
    // are real (see MintermTreeNode::reduce), and strictly before the counts
    // below, which must reflect the tree as it will actually be walked.
    {
      ZoneScopedN("MintermTree::reduce");
      reduced_nodes_per_type_[event_type] = tree->reduce();
    }
    reduced_nodes_ += reduced_nodes_per_type_[event_type];

    // Some of the leaves collected above may no longer exist after the
    // collapse (reduce() frees a collapsed node's children), and the vector's
    // size would undercount the reduction even where nothing was freed.
    // Recompute rather than reuse it.
    leaves.clear();
    tree->collectLeaves(leaves);

    total_leaves_ += leaves.size();
    if (leaves.size() > max_leaves_) max_leaves_ = leaves.size();

    // Walk benchmark bookkeeping (never read by the engine): number the atoms
    // in refine order and label each internal node with its atom's number.
    {
      std::unordered_map<const CEA::PhysicalPredicate*, uint32_t> ids;
      for (size_t k = 0; k < selected_atoms.size(); k++) {
        ids.emplace(selected_atoms[k], static_cast<uint32_t>(k));
      }
      assign_atom_ids(*tree, ids);
      atoms_per_type_[event_type] = selected_atoms;
    }

    // Step 6 (array layouts only): copy the finished tree into the array the
    // walk will use. Done last, so the copy is of the tree exactly as reduced.
    if (tree_layout_ == TreeLayoutStrategy::ArrayCompact) {
      flat_trees_.emplace(event_type, flatten_compact(*tree));
    } else if (tree_layout_ == TreeLayoutStrategy::ArrayImplicit) {
      size_t depth = tree_depth(*tree);
      if (depth <= kMaxImplicitTreeDepth) {
        flat_trees_.emplace(event_type, flatten_implicit(*tree, depth));
      } else {
        implicit_fallbacks_[event_type] = depth;
        flat_trees_.emplace(event_type, flatten_compact(*tree));
      }
    }
    if (auto flat = flat_trees_.find(event_type); flat != flat_trees_.end()) {
      tagged_flat_trees_.emplace(event_type, tag_atoms(flat->second, selected_atoms));
    }
    trees_.emplace(event_type, std::move(tree));
  }
};

}  // namespace CORE::Internal::Optimizations::MintermTree
