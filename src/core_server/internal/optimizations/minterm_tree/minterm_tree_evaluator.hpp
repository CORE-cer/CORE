#pragma once

#include <z3++.h>

#include <cstddef>
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
#include "core_server/internal/optimizations/minterm_tree/atom_extractor.hpp"
#include "core_server/internal/optimizations/minterm_tree/minterm_generator.hpp"
#include "core_server/internal/optimizations/minterm_tree/physical_predicate_z3_algebra.hpp"
#include "core_server/internal/optimizations/minterm_tree/physical_predicate_z3_translator.hpp"
#include "core_server/internal/optimizations/minterm_tree/speculation_safety.hpp"
#include "core_server/internal/optimizations/minterm_tree/tree.hpp"
#include "core_server/internal/optimizations/minterm_tree/tree_printing.hpp"
#include "core_server/internal/optimizations/optimized_predicate_evaluator.hpp"
#include "shared/datatypes/aliases/event_type_id.hpp"
#include "shared/datatypes/bitset.hpp"
#include "shared/datatypes/eventWrapper.hpp"

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
//   Run (per event, Z3 is never touched):
//     1. Evaluate the directly-evaluated predicates.
//     2. Walk the event's type tree from the root, evaluating each node's atom
//        natively to choose left (true) or right (false), down to a leaf.
//     3. OR the leaf's precomputed bitmask into the result.
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

  // `predicates` is the query's full predicate list; bit i of the result
  // corresponds to predicates[i]. The evaluator co-owns the predicates, so it
  // stays valid however the caller's copies come and go. `max_leaves_per_tree`
  // is only lowered by tests, to exercise the fallback cheaply.
  explicit MintermTreeEvaluator(
    const std::vector<std::shared_ptr<CEA::PhysicalPredicate>>& predicates,
    size_t max_leaves_per_tree = kMaxLeavesPerTree)
      : predicates_(predicates),
        max_leaves_per_tree_(max_leaves_per_tree),
        translator_(ctx_),
        algebra_(ctx_) {
    build();
  }

  MintermTreeEvaluator(const MintermTreeEvaluator&) = delete;
  MintermTreeEvaluator& operator=(const MintermTreeEvaluator&) = delete;

  Bitset operator()(Types::EventWrapper& event) override {
    Bitset result(predicates_.size());

    // Predicates that admit any event type are always evaluated directly.
    for (const auto& [index, predicate] : catch_all_) {
      if ((*predicate)(event)) result.set(index);
    }

    Types::UniqueEventTypeId event_type = event.get_unique_event_type_id();

    auto tree = trees_.find(event_type);
    if (tree != trees_.end()) {
      // Walk down the tree: each internal node's atom decides left or right.
      MintermTreeNode<z3::expr>* node = tree->second.get();
      while (!node->isLeaf()) {
        node = (*node->split_atom)(event) ? node->left.get() : node->right.get();
      }
      // The leaf already knows which predicates hold in its region.
      result |= node->satisfied_predicates;
      return result;
    }

    if (direct_event_types_.contains(event_type)) {
      // No tree for this event type (see SAFETY NET): evaluate as the baseline.
      for (size_t index : optimizable_indices_) {
        if ((*predicates_[index])(event)) result.set(index);
      }
    }
    // Otherwise no optimizable predicate admits this event type: all bits 0.
    return result;
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
                      + std::to_string(max_leaves_) + ")";
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

  size_t atom_count_ = 0;
  size_t total_leaves_ = 0;
  size_t max_leaves_ = 0;

  void build() {
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
    direct_event_types_[event_type] = reason;
  }

  // Steps 2-4 of the build phase for one event type.
  void build_tree_for_event_type(Types::UniqueEventTypeId event_type,
                                 const std::vector<CEA::PhysicalPredicate*>& atoms) {
    for (size_t index : optimizable_indices_) {
      if (!tree_can_reproduce(predicates_[index].get(), event_type, /*gated=*/true)) {
        use_direct_evaluation(event_type, "predicate shape the tree cannot reproduce");
        return;
      }
    }

    // Step 2: the atoms that can apply to this event type, as Z3 formulas.
    std::vector<CEA::PhysicalPredicate*> selected_atoms;
    std::vector<z3::expr> selected_formulas;
    for (CEA::PhysicalPredicate* atom : atoms) {
      if (!atom->admissible_event_types.contains(event_type)) continue;
      selected_atoms.push_back(atom);
      selected_formulas.push_back(translator_.translate_atom(atom, event_type));
    }

    // Which predicates can fire for this event type, as Z3 formulas. Computed
    // once here rather than once per leaf.
    std::vector<std::pair<size_t, z3::expr>> predicate_formulas;
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

    if (selected_atoms.empty()) {
      // Nothing to split on (should not happen for well-formed queries); stay
      // safe by evaluating this event type directly.
      if (!predicate_formulas.empty()) {
        use_direct_evaluation(event_type, "no atoms to build a tree from");
      }
      return;
    }

    // Step 3: refine a tree with the atoms.
    std::unique_ptr<MintermTreeNode<z3::expr>> tree = buildMintermTree<z3::expr>(
      selected_atoms, selected_formulas, algebra_, max_leaves_per_tree_);

    // Step 4: per-leaf bitmask. A leaf sets bit i when its region implies
    // predicate i's formula, i.e. when (leaf AND NOT formula) is unsatisfiable.
    std::vector<MintermTreeNode<z3::expr>*> leaves;
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

    total_leaves_ += leaves.size();
    if (leaves.size() > max_leaves_) max_leaves_ = leaves.size();
    trees_.emplace(event_type, std::move(tree));
  }
};

}  // namespace CORE::Internal::Optimizations::MintermTree
