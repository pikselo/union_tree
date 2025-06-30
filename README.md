The file implements a compile-time “binary-tree variant”:

You hand it a sequence of types (int, double, … , sentinel_t).

At compile time it folds that sequence into a perfectly-typed binary tree of unions (node_t).

Any element can later be accessed by position with get<Index>(), and construction is done with index_t<N>{}.

The result behaves a bit like std::variant, but

the storage layout is a balanced binary tree (so lookup is O(log n) template-instantiation steps instead of a linear chain of std::variant’s std::visit),

everything is constexpr and therefore free at runtime,

no dynamic allocation nor extra tag is stored (the index is known at compile time).

Parsing / folding meta-functions recursively fold a pack of types into a single node_t tree while preserving order. The algorithm rotates the pack, each step combining the first two types until 

only one tree + the trailing sentinel_t remains. All done in consteval, so the final type is ready before runtime.

Union_tree_ref adds compile-time mapping reference → pointer. Enables the pointer trick above without altering non-reference types and keeps reference semantics intact all the way up the tree.

'constexpr' construction/ access still works for value leaves. When storing references you lose constexpr because a pointer to a dynamic object isn’t a constant expression.