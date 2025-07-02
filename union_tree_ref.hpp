#include <concepts>
#include <utility>
#include "union_tree.hpp"

// used to mark end of user provided type sequence
struct sentinel_t{static constexpr std::size_t elem_size = 0;}; 

template <typename T>
inline constexpr bool is_sentinel_v = std::is_same_v<T, sentinel_t>;

template <typename T>
concept Sentinel = is_sentinel_v<T>;

template <class A, class B>
struct node_t;

template <typename T>
struct is_node : std::false_type {};

template < typename T, typename S>
struct is_node<node_t<T, S>> : std::true_type {};

template <typename T>
inline constexpr bool is_node_v = is_node<T>::value;

template <typename T>
concept Node = is_node<T>::value;

template <typename T>
concept NodeOrSentinel = Node<T> or Sentinel<T>;

template <typename T>
concept Leaf = not NodeOrSentinel<T>;

template <typename T, std::size_t Index>
concept LeftNode = requires 
  {{T::elem_size } -> std::convertible_to<std::size_t>;}  && (Index < T::elem_size);

template <typename T, std::size_t Index>
concept RightNode = requires
  {{T::elem_size}  -> std::convertible_to<std::size_t>;} && (Index >= T::elem_size);


template <std::size_t val>
using index_t = std::in_place_index_t<val>;


template <Node T, NodeOrSentinel S>
struct node_t<T, S> {

	union {T a; S b; };

	static constexpr std::size_t elem_size = T::elem_size + S::elem_size;
	
	constexpr node_t() = default;
	
	template <std::size_t Index, class... Args> requires LeftNode<T, Index>
	constexpr node_t(index_t<Index>, Args&&... args)  : a{index_t<Index>{}, std::forward<Args>(args)... }{}
	
	template <std::size_t Index, class... Args>	requires RightNode<T, Index>
	constexpr node_t(index_t<Index>, Args&&... args)  : b{index_t<Index - T::elem_size>{}, std::forward<Args>(args)... }{}
	
    template <std::size_t Index>
	constexpr decltype(auto) get() const {
      if constexpr ( Index < T::elem_size )
	    return a.template get<Index>();
	  else 
        return b.template get<Index - T::elem_size>();	
	}
};


template <typename T>
struct leaf_impl {
    using type = T;
};

template <typename T>
struct leaf_impl<T&> {
    using type = T*;
};

template <typename T>
using leaf_t = typename leaf_impl<T>::type;

template<typename T>
constexpr decltype(auto) ref_or_val(T&& value) {
    if constexpr (std::is_pointer_v<std::remove_reference_t<T>>) 
        return *value;
    else
        return value;
}

template<typename LeafPtr, typename Arg>
concept bindable_reference = std::is_pointer_v<LeafPtr> && std::is_lvalue_reference_v<Arg> &&
    std::is_same_v<std::remove_cv_t<std::remove_pointer_t<LeafPtr>>, std::remove_cvref_t<Arg>> &&
    (std::is_const_v<std::remove_pointer_t<LeafPtr>> || !std::is_const_v<std::remove_reference_t<Arg>>);


template<typename LeafPtr, typename... Args>
concept constructible_from_pack = (!std::is_pointer_v<LeafPtr>) && std::constructible_from<LeafPtr,Args...>;

template <Leaf T, Leaf S>
struct node_t<T, S> {

    using leaf_T = leaf_t<T>;
    using leaf_S = leaf_t<S>;

	union {leaf_T a; leaf_S b; };

	static constexpr std::size_t elem_size = 2;
	
	constexpr node_t() = default;
	
    template <class Arg>      requires bindable_reference<leaf_T, Arg&>
    constexpr node_t(index_t<0>, Arg& arg)       : a{std::addressof(arg)} {}

	template <class... Args>  requires constructible_from_pack<leaf_T, Args...>     
	constexpr node_t(index_t<0>, Args&&... args) : a{std::forward<Args>(args)...} {}
	
    template <class Arg>      requires bindable_reference<leaf_S, Arg&>
    constexpr node_t(index_t<1>, Arg& arg)       : b{std::addressof(arg)} {}

	template <class... Args>  requires constructible_from_pack<leaf_S, Args...>
	constexpr node_t(index_t<1>, Args&&... args) : b{std::forward<Args>(args)...} {}

   template <std::size_t Index>
   constexpr decltype(auto) get() const {
     if constexpr (Index == 0)
      return ref_or_val(a);
    else
      return ref_or_val(b);
   }
};

template <Leaf T>
struct node_t<T, void> {
    
    T a;
	static constexpr std::size_t elem_size = 1;
	constexpr node_t()  = default;
	
	template <class... Args>
	constexpr node_t(index_t<0>, Args&&... args): a{std::forward<Args>(args)...} {}

    template <std::size_t Index>
    constexpr decltype(auto) get() const {
       return ref_or_val(a);
    }
};


// =================== algorithm to build the tree of unions 
// take a sequence of types and perform an order preserving fold until only last pair is left

//Make tree algorithm
template <typename Left, typename Right>
using make_node = node_t<Left, Right>;


//Helper to find first element in pack
template<typename First, typename...Rest>
struct first_element {using type = First;};

template<typename... Ts>
using first_element_t = typename first_element<Ts...>::type;

// Check if the next element left in the pack is `sentinel_t`
template<typename... Ts>
constexpr bool next_is_end = std::is_same_v<first_element_t<Ts...>, sentinel_t>;

template <typename Left, typename Right>
struct parse {
  using type = node_t<Left, Right>;
};

// Single type case: make a leaf node
template <typename T>
struct parse<T, void> {
  using type = node_t<T, void>;
};

template <typename Previous>
struct consume_one {
  using type = std::conditional_t<!is_node_v<Previous>, node_t<Previous, void>, Previous>;
};

template <typename Previous, typename Current>
struct consume_two {
  using type = node_t<Previous, Current>;
};

template <typename Previous>
using consume_one_t = typename consume_one<Previous>::type;

template <typename Previous, typename Current>
using consume_two_t = typename consume_two<Previous, Current>::type;

//basic grammar for making a node
// Node -> Node + Node     | 
//         Type + Type     | 
//         Type + Sentinel |
//         Type

// Parser primary template
template <typename... Ts>
struct parser_impl;

// Base cases:
template <typename Last>
struct parser_impl<Last> {
    using type = node_t<Last, void>;
};

template <typename Node>
struct parser_impl<Node, sentinel_t> {
    using type = node_t<Node, sentinel_t>;
};

template <typename Left, typename Right>
struct parser_impl<Left, Right> {
    using type = node_t<Left, Right>;
};

// Recursive case explicitly handles sentinel at end:
template <typename First, typename Second, typename... Rest>
struct parser_impl<First, Second, sentinel_t, Rest...> {
    using next = node_t<First, Second>;
    using type = typename parser_impl<Rest..., next, sentinel_t>::type;
};

// Recursive case explicitly handles sentinel at end:
template <typename First, typename Second, typename... Rest> requires (!is_sentinel_v<Second>)
struct parser_impl<First, Second, Rest...> {
    using next = node_t<First, Second>;
    using type = typename parser_impl<Rest..., next>::type;
};

// Handle the sentinel explicitly without rotation:
template <typename Previous, typename... Rest>
struct parser_impl<Previous, sentinel_t, Rest...> {
    using next = consume_one_t<Previous>;
    using type = typename parser_impl<Rest..., next, sentinel_t>::type;
};

template <typename... Ts>
using parsed_types = typename parser_impl<Ts...>::type;
  