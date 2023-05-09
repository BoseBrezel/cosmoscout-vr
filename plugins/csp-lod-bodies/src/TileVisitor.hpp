////////////////////////////////////////////////////////////////////////////////////////////////////
//                               This file is part of CosmoScout VR                               //
////////////////////////////////////////////////////////////////////////////////////////////////////

// SPDX-FileCopyrightText: German Aerospace Center (DLR) <cosmoscout@dlr.de>
// SPDX-License-Identifier: MIT

#ifndef CSP_LOD_BODIES_TILEVISITOR_HPP
#define CSP_LOD_BODIES_TILEVISITOR_HPP

#include "HEALPix.hpp"
#include "TileId.hpp"
#include "TileQuadTree.hpp"

namespace csp::lodbodies {

class TileNode;

/// Base class template for TileQuadTree visitors. It is a base class template that should be
/// instantiated with the deriving class as template argument, i.e.:
///
/// @code{.cpp}
/// class MyVisitor : public TileVisitor<MyVisitor> { };
/// @endcode
///
/// This is the CRTP (curiously recurring template pattern) pattern and allows TileVistor to call
/// member functions of DerivedT without having to resort to runtime polymorphism (i.e. virtual
/// functions).
///
/// The functions preTraverse, postTraverse, preVisitRoot, postVisitRoot, preVisit, and postVisit
/// are called during traversal before/after the traversal as a whole, visiting a root node, and
/// visiting a non-root node respectively. The order of calls looks like:
///
/// @code{.cpp}
/// preTraverse()
///   preVisitRoot()        // root 0
///     preVisit()          // root 0 - child 0
///     postVisit()         // root 0 - child 0
///     preVisit()          // root 0 - child 1
///     postVisit()         // root 0 - child 1
///     // ...
///   postVisitRoot()       // root 0
/// postTraverse()
/// @endcode
///
/// A common requirement for tree traversal is maintaining a state stack. In order to allow DerivedT
/// to extend the state it must implement three additional functions: pushState, popState, and
/// getState and maintain a stack of objects derived from TileVisitor::StateBase.
///
/// The default implementation of the functions pushState, popState does not actually maintain a
/// stack, but uses a single StateBase object - this is sufficient to provide correct information to
/// the preVisit / preVisitRoot callbacks, but the postVisit / postVisitRoot ones will not see
/// correct values!
///
/// If the "callback" functions (pre/postTraverse, pre/postVisit) are protected or private in
/// DerivedT make TileVisitor a friend class so that it can call these functions.
template <typename DerivedT>
class TileVisitor {
 public:
  using DerivedType = DerivedT;

  explicit TileVisitor(TileQuadTree* tree);

  /// Start traversal of the trees passed to the constructor.
  void visit();

 protected:
  /// Convenience access to members of DerivedType.
  DerivedType&       self();
  DerivedType const& self() const;

  void visitRoot(TileNode* root);
  void visitLevel(TileNode* node);

  /// Called before visiting the first root node. Returns if traversal should commence or
  /// not.
  /// Reimplement in the derived class, the default just returns true.
  virtual bool preTraverse();

  /// Called after visiting the last node. Reimplement in the derived class, the default
  /// just does
  /// nothing.
  virtual void postTraverse();

  /// Called for each root node visited, before visiting any children. Returns if any
  /// children
  /// should be visited (true) or skipped (false).
  /// Reimplement in the derived class, the default just returns false.
  ///
  /// For finer grained control over which children to visit, set the corresponding entries of
  /// StateBase::children to true (visit child - the default) or false (skip child). These
  /// entries are only considered if this functions returns true.
  virtual bool preVisitRoot(TileNode* root);

  /// Called for each root node visited, after visiting any children. Reimplement in the
  /// derived
  /// class, the default just does nothing.
  virtual void postVisitRoot(TileNode* root);

  /// Called for each non-root node visited, before visiting the child nodes.
  /// Returns if children should be visited (true) or skipped (false).
  /// Reimplement in the derived class, the default just returns false.
  ///
  /// For finer grained control over which children to visit, set the corresponding entries
  /// of StateBase::children to true (visit child - the default) or false (skip child).
  /// These entries are only considered if this functions returns true.
  virtual bool preVisit(TileNode* node);

  /// Called for each node visited, after visiting the child nodes. Reimplement in the
  /// derived
  /// class, the default just does nothing.
  virtual void postVisit(TileNode* node);

  TileQuadTree* mTree;
};

template <typename DerivedT>
TileVisitor<DerivedT>::TileVisitor(TileQuadTree* tree)
    : mTree(tree) {
}

template <typename DerivedT>
void TileVisitor<DerivedT>::visit() {
  if (self().preTraverse()) {
    for (int i = 0; i < TileQuadTree::sNumRoots; ++i) {
      TileNode* root = mTree->getRoot(i);
      visitRoot(root);
    }
  }

  self().postTraverse();
}

template <typename DerivedT>
typename TileVisitor<DerivedT>::DerivedType& TileVisitor<DerivedT>::self() {
  return *static_cast<DerivedType*>(this);
}

template <typename DerivedT>
typename TileVisitor<DerivedT>::DerivedType const& TileVisitor<DerivedT>::self() const {
  return *static_cast<DerivedType const*>(this);
}

template <typename DerivedT>
void TileVisitor<DerivedT>::visitRoot(TileNode* root) {

  if (self().preVisitRoot(root)) {
    for (int i = 0; i < 4; ++i) {
      TileNode* child = root ? root->getChild(i) : nullptr;

      if (child) {
        visitLevel(child);
      }
    }
  }

  self().postVisitRoot(root);
}

template <typename DerivedT>
void TileVisitor<DerivedT>::visitLevel(TileNode* node) {
  if (self().preVisit(node)) {
    for (int i = 0; i < 4; ++i) {
      TileNode* child = node ? node->getChild(i) : nullptr;

      if (child) {
        visitLevel(child);
      }
    }
  }

  self().postVisit(node);
}

template <typename DerivedT>
bool TileVisitor<DerivedT>::preTraverse() {
  // default impl - start traversal
  return true;
}

template <typename DerivedT>
void TileVisitor<DerivedT>::postTraverse() {
  // default impl - empty
}

template <typename DerivedT>
bool TileVisitor<DerivedT>::preVisitRoot(TileNode* /*root*/) {
  // default impl - do not visit children
  return false;
}

template <typename DerivedT>
void TileVisitor<DerivedT>::postVisitRoot(TileNode* /*root*/) {
  // default impl - empty
}

template <typename DerivedT>
bool TileVisitor<DerivedT>::preVisit(TileNode* /*node*/) {
  // default impl - do not visit children
  return false;
}

template <typename DerivedT>
void TileVisitor<DerivedT>::postVisit(TileNode* /*node*/) {
  // default impl - empty
}

} // namespace csp::lodbodies

#endif // CSP_LOD_BODIES_TILEVISITOR_HPP
