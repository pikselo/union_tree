The file implements a compile-time “binary-tree variant”:

You hand it a sequence of types (int, double, … , sentinel_t).

At compile time it folds that sequence into a perfectly-typed binary tree of unions (node_t).

Any element can later be accessed by position with get<Index>(), and construction is done with index_t<N>{}.

The result behaves a bit like std::variant, but

the storage layout is a balanced binary tree (so lookup is O(log n) template-instantiation steps instead of a linear chain of std::variant’s std::visit),

everything is constexpr and therefore free at runtime,

no dynamic allocation nor extra tag is stored (the index is known at compile time).


Further extension supporting refrence types + more.

Parsing / folding meta-functions recursively fold a pack of types into a single node_t tree while preserving order. The algorithm rotates the pack, each step combining the first two types until 

only one tree + the trailing sentinel_t remains. All done in consteval, so the final type is ready before runtime.

Union_tree_ref adds compile-time mapping reference → pointer. Enables the pointer trick above without altering non-reference types and keeps reference semantics intact all the way up the tree.

'constexpr' construction/ access still works for value leaves. When storing references you lose constexpr because a pointer to a dynamic object isn’t a constant expression.

Due to compile-time footprint 'consteval' functions have been replaced with TMP meta code with that Bcompiler stores fewer AST nodes (no function‐body, no temporary object), big metaprogramming 

trees compile faster and consume less RAM—important when thousands of instantiations are generated.

Below is a “from–the-inside-out” tour of the parsing / folding algorithm that turns an ordered pack of leaf types

T0, T1, …, TN-1, sentinel_t

What we want to end up with
Every node_t<X,Y> stores either another node_t (an internal node) or an actual leaf (possibly a pointer when the user supplied a reference).

The fold is left-associative but size-balanced – the “old” subtree is always packed into the left slot (A), a freshly consumed element goes to the right slot (B).

We keep pushing the result of each fold to the back of the parameter pack so the algorithm keeps chewing on the “front”.

int, char, float, double, sentinel_t

1) fold(int , char)   -> node_t<int , char>
   pack becomes       float, double, sentinel_t, node_t<int,char>

2) fold(float, double) -> node_t<float, double>
   pack becomes       node_t<int,char>, node_t<float, double>, sentinel_t

3) fold(node_t<int , char>, node_t<float, double>,  )     -> node_t< node_t<int , char> , node_t<float, double>>
   pack becomes       node_t<node_t< node_t<int , char> , node_t<float, double>>, sentinel_t>

4) sentinel reached  -> final node_t< previous_tree , sentinel_t>

When the parser finds something, sentinel_t

the something might be a naked leaf. Turning it into node_t<Leaf,void> guarantees a uniform interface (elem_size, get<…>, …) and avoids special-casing later.

