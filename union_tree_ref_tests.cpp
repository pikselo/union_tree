#include <concepts>
#include <utility>
#include <cassert>
#include "union_tree_ref.hpp"

int main()
{
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

    using storage_t = parsed_types<int, char, float, double, unsigned, std::byte, short, sentinel_t>;

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

    using t_ref_short = node_t<short&, void>;
    static_assert( std::is_same_v<t_ref_short, parsed_types<short&, void>>);

    using t_ref_int_char = node_t<int&, char&>;
    using t_ref_2 = node_t<t_ref_int_char, sentinel_t>;
    static_assert(std::is_same_v<t_ref_2, parsed_types<int&, char&, sentinel_t>>);

    using t_ref_int_char_short = node_t<t_ref_int_char, t_ref_short>;
    using t_ref_3 = node_t<t_ref_int_char_short, sentinel_t>;
    static_assert(std::is_same_v<t_ref_3, parsed_types<int&, char&, short&, sentinel_t>>);

    using t_ref_float_double = node_t<float&, double&>;
    using t_ref_int_char_float_double = node_t<t_ref_int_char, t_ref_float_double>;
    using t_ref_4 = node_t<t_ref_int_char_float_double, sentinel_t>;
    static_assert(std::is_same_v<t_ref_4, parsed_types<int&, char&, float&, double&, sentinel_t>>);

    using t_ref_int_char_float_double_short = node_t<t_ref_int_char_float_double, t_ref_short>;
    using t_ref_5 = node_t<t_ref_int_char_float_double_short, sentinel_t>;
    static_assert(std::is_same_v<t_ref_5, parsed_types<int&, char&, float&, double&, short&, sentinel_t>>);

    using t_ref_unsigned_byte = node_t<unsigned&, std::byte&>;
    using t_ref_int_char_float_double_unsigned_byte = node_t<t_ref_int_char_float_double, t_ref_unsigned_byte>;
    using t_ref_6 = node_t<t_ref_int_char_float_double_unsigned_byte, sentinel_t>;
    static_assert(std::is_same_v<t_ref_6, parsed_types<int&, char&, float&, double&, unsigned&, std::byte&, sentinel_t>>);

    using t_ref_unsigned_byte_short = node_t<t_ref_unsigned_byte, t_ref_short>;
    using t_ref_int_char_float_double_unsigned_byte_short = node_t<t_ref_int_char_float_double, t_ref_unsigned_byte_short>;
    using t_ref_7 = node_t<t_ref_int_char_float_double_unsigned_byte_short, sentinel_t>;
    static_assert(std::is_same_v<t_ref_7, parsed_types<int&, char&, float&, double&, unsigned&, std::byte&, short&, sentinel_t>>);

    using t_ref_signed_long = node_t<signed&, long&>;
    using t_ref_unsigned_byte_signed_long = node_t<t_ref_unsigned_byte, t_ref_signed_long>;
    using t_ref_8 = node_t<node_t<t_ref_int_char_float_double, t_ref_unsigned_byte_signed_long>, sentinel_t>;
    static_assert(std::is_same_v<t_ref_8, parsed_types<int&, char&, float&, double&, unsigned&, std::byte&, signed&, long&, sentinel_t>>);

    using storage_ref_t = parsed_types<int&, char&, float&, double&, unsigned&, std::byte&, short&, sentinel_t>;

    int var_int{3};
    storage_ref_t ref_storage{index_t<0>{}, var_int};
    var_int                                                    =  5;
    assert(ref_storage.get<0>()                                == 5);
    ref_storage.get<0>()                                       =  0;
    assert(ref_storage.get<0>()                                == 0);
    assert(var_int                                             == 0);
    static_assert(std::is_same<decltype(ref_storage.get<0>()), int&>::value);


    char var_char{3};
    storage_ref_t ref_storage_1{index_t<1>{}, var_char};
    var_char                                                   =  5;
    assert(ref_storage_1.get<1>()                              == static_cast<char>(5));
    ref_storage_1.get<1>()                                     =  1;
    assert(ref_storage_1.get<1>()                              == static_cast<char>(1));
    assert(var_char                                            == static_cast<char>(1));
    static_assert(std::is_same<decltype(ref_storage.get<1>()), char&>::value);


    float var_float{3};
    storage_ref_t ref_storage_2{index_t<2>{}, var_float};
    var_float                                                  =  5;
    assert(ref_storage_2.get<2>()                              == 5);
    ref_storage_2.get<2>()                                     =  2;
    assert(ref_storage_2.get<2>()                              == 2);
    assert(var_float                                           == 2);
    static_assert(std::is_same<decltype(ref_storage.get<2>()), float&>::value);


    double var_double{3};
    storage_ref_t ref_storage_3{index_t<3>{}, var_double};
    var_double                                                 =  5;
    assert(ref_storage_3.get<3>()                              == 5);
    ref_storage_3.get<3>()                                     =  3;
    assert(ref_storage_3.get<3>()                              == 3);
    assert(var_double                                          == 3);
    static_assert(std::is_same<decltype(ref_storage.get<3>()), double&>::value);


    unsigned var_unsigned{3};
    storage_ref_t ref_storage_4{index_t<4>{}, var_unsigned};
    var_unsigned                                               =  5;
    assert(ref_storage_4.get<4>()                              == 5);
    ref_storage_4.get<4>()                                     =  4;
    assert(ref_storage_4.get<4>()                              == 4);
    assert(var_unsigned                                        == 4);
    static_assert(std::is_same<decltype(ref_storage.get<4>()), unsigned&>::value);


    std::byte var_byte{3};
    storage_ref_t ref_storage_5{index_t<5>{}, var_byte};
    var_byte                                                   =  static_cast<std::byte>(4);
    assert(ref_storage_5.get<5>()                              == static_cast<std::byte>(4));
    ref_storage_5.get<5>()                                     =  static_cast<std::byte>(5);
    assert(ref_storage_5.get<5>()                              == static_cast<std::byte>(5));
    assert(var_byte                                            == static_cast<std::byte>(5));
    static_assert(std::is_same<decltype(ref_storage.get<5>()), std::byte&>::value);


    using t_const_ref_short = node_t<const short&, void>;
    static_assert( std::is_same_v<t_const_ref_short, parsed_types<const short&, void>>);

    using t_const_ref_int_char = node_t<const int&, const char&>;
    using t_const_ref_2 = node_t<t_const_ref_int_char, sentinel_t>;
    static_assert(std::is_same_v<t_const_ref_2, parsed_types<const int&, const char&, sentinel_t>>);

    using t_const_ref_int_char_short = node_t<t_const_ref_int_char, t_const_ref_short>;
    using t_const_ref_3 = node_t<t_const_ref_int_char_short, sentinel_t>;
    static_assert(std::is_same_v<t_const_ref_3, parsed_types<const int&, const char&, const short&, sentinel_t>>);


    using storage_const_ref_t = parsed_types<const int&, const char&, float&, const double&, const unsigned&, const std::byte&, const short&, sentinel_t>;
    int var_const_int{3};
    storage_const_ref_t const_ref_storage_0{index_t<0>{}, var_const_int};
    var_const_int                                             =  5; 
    assert(const_ref_storage_0.get<0>()                       == 5);
    //const_ref_storage_0.get<0>()                            =  3; //cannot assign to return value because function 'get<3UL>' returns a const value
    //assert(const_ref_storage_0.get<0>()                     == 3);
    static_assert(std::is_same<decltype(const_ref_storage_0.get<0>()), const int&>::value);


    float var_const_float{3};
    storage_const_ref_t const_ref_storage_2{index_t<2>{}, var_const_float};
    assert(const_ref_storage_2.get<2>()                       == 3);
    //const_ref_storage_0.get<0>()                            =  3; //cannot assign to return value because function returns a const value
    //assert(const_ref_storage_2.get<2>()                     == 3);
    static_assert(std::is_same<decltype(const_ref_storage_2.get<2>()), float&>::value);


    const double var_const_double{3};
    storage_const_ref_t const_ref_storage_3{index_t<3>{}, var_double};
    //var_const_double                                            =  5; //note: variable 'var_const_double' declared const
    assert(const_ref_storage_3.get<3>()                         == 3);
    //const_ref_storage_3.get<3>()                              =  3; //cannot assign to return value because function 'get<3UL>' returns a const value
    assert(const_ref_storage_3.get<3>()                         == 3);
    assert(var_double                                           == 3);
    static_assert(std::is_same<decltype(const_ref_storage_3.get<3>()), const double&>::value);


    using t_ptr_short = node_t<short*, void>;
    static_assert( std::is_same_v<t_ptr_short, parsed_types<short*, void>>);

    using t_ptr_int_char = node_t<int*, char*>;
    using t_ptr_2 = node_t<t_ptr_int_char, sentinel_t>;
    static_assert(std::is_same_v<t_ptr_2, parsed_types<int*, char*, sentinel_t>>);

    using t_ptr_int_char_short = node_t<t_ptr_int_char, t_ptr_short>;
    using t_ptr_3 = node_t<t_ptr_int_char_short, sentinel_t>;
    static_assert(std::is_same_v<t_ptr_3, parsed_types<int*, char*, short*, sentinel_t>>);

    using t_ptr_float_double = node_t<float*, double*>;
    using t_ptr_int_char_float_double = node_t<t_ptr_int_char, t_ptr_float_double>;
    using t_ptr_4 = node_t<t_ptr_int_char_float_double, sentinel_t>;
    static_assert(std::is_same_v<t_ptr_4, parsed_types<int*, char*, float*, double*, sentinel_t>>);

    using storage_ptr_t = parsed_types<int*, char*, float*, double*, unsigned*, std::byte*, short, sentinel_t>;

    int tmp = 3;
    int* ptr_int = &tmp;
    storage_ptr_t ptr_storage{index_t<0>{}, ptr_int};
    *ptr_int                                                    =  5;
    assert(*ptr_storage.get<0>()                                == 5);
    *ptr_storage.get<0>()                                       =  0;
    assert(*ptr_storage.get<0>()                                == 0);
    assert(*ptr_int                                             == 0);
    static_assert(std::is_same<decltype(ptr_storage.get<0>()), int*>::value);

    return 0;
}