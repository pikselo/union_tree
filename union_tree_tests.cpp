#include <concepts>
#include <utility>
#include "union_tree.hpp"

//Parse sequence of types into a binary tree
//Parse sequence of types into a binary tree
template <typename... Ts>
using parsed_types = decltype(parse<Ts...>());


using storage_t = parsed_types<int, char, float, double, unsigned, std::byte, short, sentinel_t>;

int main()
{
    //Check binary nature of types within the recursive union
    using t_short = node_t<short, void>;
    static_assert( std::is_same_v<t_short, parsed_types<short, void>>);

    using t_int_char = node_t<int, char>;
    using t_2 = node_t<t_int_char, sentinel_t>;
    static_assert(std::is_same_v<t_2, parsed_types<int, char, sentinel_t>>);

    using t_int_char_short = node_t<t_int_char, t_short>;
    using t_3 = node_t<t_int_char_short, sentinel_t>;
    static_assert(std::is_same_v<t_3, parsed_types<int, char, short, sentinel_t>>);

    using t_float_double = node_t<float, double>;
    using t_int_char_float_double = node_t<t_int_char, t_float_double>;
    using t_4 = node_t<t_int_char_float_double, sentinel_t>;
    static_assert(std::is_same_v<t_4, parsed_types<int, char, float, double, sentinel_t>>);

    using t_int_char_float_double_short = node_t<t_int_char_float_double, t_short>;
    using t_5 = node_t<t_int_char_float_double_short, sentinel_t>;
    static_assert(std::is_same_v<t_5, parsed_types<int, char, float, double, short, sentinel_t>>);

    using t_unsigned_byte = node_t<unsigned, std::byte>;
    using t_int_char_float_double_unsigned_byte = node_t<t_int_char_float_double, t_unsigned_byte>;
    using t_6 = node_t<t_int_char_float_double_unsigned_byte, sentinel_t>;
    static_assert(std::is_same_v<t_6, parsed_types<int, char, float, double, unsigned, std::byte, sentinel_t>>);

    using t_unsigned_byte_short = node_t<t_unsigned_byte, t_short>;
    using t_int_char_float_double_unsigned_byte_short = node_t<t_int_char_float_double, t_unsigned_byte_short>;
    using t_7 = node_t<t_int_char_float_double_unsigned_byte_short, sentinel_t>;
    static_assert(std::is_same_v<t_7, parsed_types<int, char, float, double, unsigned, std::byte, short, sentinel_t>>);

    using t_signed_long = node_t<signed, long>;
    using t_unsigned_byte_signed_long = node_t<t_unsigned_byte, t_signed_long>;
    using t_8 = node_t<node_t<t_int_char_float_double, t_unsigned_byte_signed_long>, sentinel_t>;
    static_assert(std::is_same_v<t_8, parsed_types<int, char, float, double, unsigned, std::byte, signed, long, sentinel_t>>);

    //Test values and types being currently stored
    constexpr storage_t storage{index_t<0>{}, int(3)};
    static_assert(storage.get<0>() == 3);
    
    constexpr storage_t storage_1{index_t<1>{}, char(3)};
    static_assert(storage_1.get<1>() == static_cast<char>(3));
    static_assert(std::is_same<std::remove_cvref_t<decltype(storage_1.get<1>())>, char>::value);

    constexpr storage_t storage_2{index_t<2>{}, float(3)};
    static_assert(storage_2.get<2>() == 3);
    static_assert(std::is_same<std::remove_cvref_t<decltype(storage_1.get<2>())>, float>::value);

    constexpr storage_t storage_3{index_t<3>{}, double(3)};
    static_assert(storage_3.get<3>() == 3);
    static_assert(std::is_same<std::remove_cvref_t<decltype(storage_1.get<3>())>, double>::value);

    constexpr storage_t storage_4{index_t<4>{}, unsigned(3)};
    static_assert(storage_4.get<4>() == 3);
    static_assert(std::is_same<std::remove_cvref_t<decltype(storage_1.get<4>())>, unsigned>::value);

    constexpr storage_t storage_5{index_t<5>{}, std::byte(3)};
    static_assert(storage_5.get<5>() == static_cast<std::byte>(3));
    static_assert(std::is_same<std::remove_cvref_t<decltype(storage_1.get<5>())>, std::byte>::value);

    constexpr storage_t storage_6{index_t<6>{}, short(3)};
    static_assert(storage_6.get<6>() == 3);
    static_assert(std::is_same<std::remove_cvref_t<decltype(storage_1.get<6>())>, short>::value);

    return 0;
}