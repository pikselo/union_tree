//Helpers
template <std::size_t val>
using index_t = std::in_place_index_t<val>;

//Type marking end of sequence of types
//Does not have any elements stored within
struct sentinel_t{static constexpr std::size_t elem_size = 0;}; 

//Sentinel helper functions
template <typename T>
inline constexpr bool is_sentinel_v = std::is_same_v<T, sentinel_t>;

template <typename T>
concept Sentinel = is_sentinel_v<T>;

//Node is expected to hold two types specified by leafs
template <class A, class B>
struct node_t;

template <typename T>
struct is_node : std::false_type {};

template < typename T, typename S>
struct is_node<node_t<T, S>> : std::true_type {};

template <typename T>
inline constexpr bool is_node_v = is_node<T>::value;


//Type element in tree can be either Node, Leaf or Sentinel
template <typename T>
concept Node = is_node<T>::value;

template <typename T>
concept NodeOrSentinel = Node<T> or Sentinel<T>;

//Leaf is any type that is not Node neither Sentinel
template <typename T>
concept Leaf = not NodeOrSentinel<T>;


//Left node position is computed based on number of subnodes in tree
template <typename T, std::size_t Index>
concept LeftNode = requires 
  {{T::elem_size } -> std::convertible_to<std::size_t>;}  && (Index < T::elem_size);

//Right node position is computed based on number of subnodes in tree
template <typename T, std::size_t Index>
concept RightNode = requires
  {{T::elem_size}  -> std::convertible_to<std::size_t>;} && (Index >= T::elem_size);


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
  constexpr auto& get() const {
    if constexpr ( Index < T::elem_size )
	  return a.template get<Index>();
	else 
      return b.template get<Index - T::elem_size>();	
	}
};

template <Leaf T, Leaf S>
struct node_t<T, S> {
  union {T a; S b; };

  static constexpr std::size_t elem_size = 2;
	
  constexpr node_t() = default;
	
  template <class... Args>                  
  constexpr node_t(index_t<0>, Args&&... args) : a{std::forward<Args>(args)...} {}
	
  template <class... Args>                 
  constexpr node_t(index_t<1>, Args&&... args) : b{std::forward<Args>(args)...} {}

  template <std::size_t Index>
  constexpr auto& get() const {
    if constexpr ( Index == 0 )
      return a;
	else 
	  return b;
	}
};

template <Leaf T>
struct node_t<T, void> {
    T a;
	static constexpr std::size_t elem_size = 1;
	constexpr node_t() = default;
	
	template <class... Args>
	constexpr node_t(index_t<0>, Args&&... args): a{std::forward<Args>(args)...} {}

    template <std::size_t Index>
	constexpr auto& get() const {
        return a;
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

//Finalstep
template <typename Left, typename Right>
consteval auto parse() {
  return make_node<Left,  Right>{};
}

template <typename Previous>
consteval auto consume_one() {
  if constexpr (!is_node_v<Previous>) return make_node<Previous, void>{};
  else                                return Previous{};
}

template <typename Previous, typename Current>
consteval auto consume_two() {
  return make_node<Previous, Current>{};
}

template <typename Previous, typename Current>
using consume_two_t = decltype(consume_two<Previous, Current>());

template <typename Previous>
using consume_one_t = decltype(consume_one<Previous>());


//for making a node
// Node -> Node + Node     | 
//         Type + Type     | 
//         Type + Sentinel |
//         Type

// Forward declare advance:
template <typename Previous, typename Current, typename... Rest>
consteval auto advance();

// Recursive case: reduce first two, append to the end and continue
template <typename Previous, typename Current, typename... Rest> requires (sizeof...(Rest) >= 1)
consteval auto  parse() {
  if constexpr (next_is_end<Rest...>) return parse<consume_two_t<Previous, Current>, Rest...>();
  else                                return advance<Previous, Current, Rest...>();
}

//advance is implemented in terms of a rotate left with middle parameter fixed to index 1
template <typename Previous, typename Current, typename... Rest>
consteval auto advance() {
  if constexpr (is_sentinel_v<Current>) return parse<Rest..., consume_one_t<Previous>, Current>();
  else                                  return parse<Rest..., consume_two_t<Previous,  Current>>();
}
