#pragma once

#include <type_traits>

namespace mystl{

namespace detail{

// for cnditional_t
template<bool>
struct conditional_;

template<>
struct conditional_<false>{
    template<typename , typename T>
    struct type{
        using apply = T;
    };
};


template<>
struct conditional_<true>{
    template<typename T, typename>
    using apply = T;
};

}// namespace detail

// If true, return T, else, return F
template <bool B, typename T, typename F>
using conditional_t = typename detail::conditional_<B>::template type<T, F>::apply;

template<bool b, class T>
struct enable_if{};

template<class T>
struct enable_if<true, T>{
    using type = T;
};

template<bool b, class T = void>
using enable_if_t = typename enable_if<b, T>::type;

template <class T, T val>
struct integral_constant{
    static constexpr T value = val;
    using value_type = T;
    using type = integral_constant;
    constexpr operator value_type() const noexcept { return value; }
    constexpr value_type operator()() const noexcept { return value; }
};

template<class T, T val>
constexpr T integral_constant<T, val>::value;

template<bool val>
using bool_constant = integral_constant<bool, val>;

struct true_type : bool_constant<true> {};
struct false_type : bool_constant<false> {};

template<class T, class U>
struct is_same : false_type {};

template<class T>
struct is_same<T, T> : true_type {};

template<class T, class U>
constexpr bool is_same_v = is_same<T, U>::value;

template<typename T, typename... Args>
struct is_one_of;// only a declaration

template<typename T>
struct is_one_of<T> : false_type {};// 遞迴終止

template<typename T, typename U, typename... Args>
struct is_one_of<T, U, Args...> : bool_constant<is_same<T, U>::value || is_one_of<T, Args...>::value> {};

template<typename T>
struct is_void : is_one_of<T, void, const void, volatile void, const volatile void> {};


// 預設類行為false
template<class T>
struct is_integral : false_type {};

template<class T>
constexpr bool is_integral_v = is_integral<T>::value;

template<>
struct is_integral<bool> : true_type {};

template<>
struct is_integral<char> : true_type {};

template<>
struct is_integral<signed char> : true_type {};

template<>
struct is_integral<unsigned char> : true_type {};

template<>
struct is_integral<wchar_t> : true_type {};

template<>
struct is_integral<short> : true_type {};

template<>
struct is_integral<unsigned short> : true_type {};

template<>
struct is_integral<int> : true_type {};

template<>
struct is_integral<unsigned int> : true_type {};

template<>
struct is_integral<long> : true_type {};

template<>
struct is_integral<unsigned long> : true_type {};

template<>
struct is_integral<long long> : true_type {};

template<>
struct is_integral<unsigned long long> : true_type {};


// 預設類行為non-POD類型
template<class T>
struct type_traits {
  using has_trivial_default_constructor = false_type;
  using has_trivial_copy_constructor = false_type;
  using has_trivial_assignment_operator = false_type;
  using has_trivial_destructor = false_type;
  using is_POD_type = false_type;
};

template<>
struct type_traits<bool> {
  using has_trivial_default_constructor = true_type;
  using has_trivial_copy_constructor = true_type;
  using has_trivial_assignment_operator = true_type;
  using has_trivial_destructor = true_type;
  using is_POD_type = true_type;
};

template<>
struct type_traits<char> {
  using has_trivial_default_constructor = true_type;
  using has_trivial_copy_constructor = true_type;
  using has_trivial_assignment_operator = true_type;
  using has_trivial_destructor = true_type;
  using is_POD_type = true_type;
};

template<>
struct type_traits<unsigned char> {
  using has_trivial_default_constructor = true_type;
  using has_trivial_copy_constructor = true_type;
  using has_trivial_assignment_operator = true_type;
  using has_trivial_destructor = true_type;
  using is_POD_type = true_type;
};

template<>
struct type_traits<signed char> {
  using has_trivial_default_constructor = true_type;
  using has_trivial_copy_constructor = true_type;
  using has_trivial_assignment_operator = true_type;
  using has_trivial_destructor = true_type;
  using is_POD_type = true_type;
};

template<>
struct type_traits<wchar_t> {
  using has_trivial_default_constructor = true_type;
  using has_trivial_copy_constructor = true_type;
  using has_trivial_assignment_operator = true_type;
  using has_trivial_destructor = true_type;
  using is_POD_type = true_type;
};

template<>
struct type_traits<short> {
  using has_trivial_default_constructor = true_type;
  using has_trivial_copy_constructor = true_type;
  using has_trivial_assignment_operator = true_type;
  using has_trivial_destructor = true_type;
  using is_POD_type = true_type;
};

template<>
struct type_traits<unsigned short> {
  using has_trivial_default_constructor = true_type;
  using has_trivial_copy_constructor = true_type;
  using has_trivial_assignment_operator = true_type;
  using has_trivial_destructor = true_type;
  using is_POD_type = true_type;
};

template<>
struct type_traits<int> {
  using has_trivial_default_constructor = true_type;
  using has_trivial_copy_constructor = true_type;
  using has_trivial_assignment_operator = true_type;
  using has_trivial_destructor = true_type;
  using is_POD_type = true_type;
};

template<>
struct type_traits<unsigned int> {
  using has_trivial_default_constructor = true_type;
  using has_trivial_copy_constructor = true_type;
  using has_trivial_assignment_operator = true_type;
  using has_trivial_destructor = true_type;
  using is_POD_type = true_type;
};

template<>
struct type_traits<long> {
  using has_trivial_default_constructor = true_type;
  using has_trivial_copy_constructor = true_type;
  using has_trivial_assignment_operator = true_type;
  using has_trivial_destructor = true_type;
  using is_POD_type = true_type;
};

template<>
struct type_traits<unsigned long> {
  using has_trivial_default_constructor = true_type;
  using has_trivial_copy_constructor = true_type;
  using has_trivial_assignment_operator = true_type;
  using has_trivial_destructor = true_type;
  using is_POD_type = true_type;
};

template<>
struct type_traits<long long> {
  using has_trivial_default_constructor = true_type;
  using has_trivial_copy_constructor = true_type;
  using has_trivial_assignment_operator = true_type;
  using has_trivial_destructor = true_type;
  using is_POD_type = true_type;
};

template<>
struct type_traits<unsigned long long> {
  using has_trivial_default_constructor = true_type;
  using has_trivial_copy_constructor = true_type;
  using has_trivial_assignment_operator = true_type;
  using has_trivial_destructor = true_type;
  using is_POD_type = true_type;
};

template<>
struct type_traits<float> {
  using has_trivial_default_constructor = true_type;
  using has_trivial_copy_constructor = true_type;
  using has_trivial_assignment_operator = true_type;
  using has_trivial_destructor = true_type;
  using is_POD_type = true_type;
};

template<>
struct type_traits<double> {
  using has_trivial_default_constructor = true_type;
  using has_trivial_copy_constructor = true_type;
  using has_trivial_assignment_operator = true_type;
  using has_trivial_destructor = true_type;
  using is_POD_type = true_type;
};

template<>
struct type_traits<long double> {
  using has_trivial_default_constructor = true_type;
  using has_trivial_copy_constructor = true_type;
  using has_trivial_assignment_operator = true_type;
  using has_trivial_destructor = true_type;
  using is_POD_type = true_type;
};

template<class T>
struct type_traits<T *> {
  using has_trivial_default_constructor = true_type;
  using has_trivial_copy_constructor = true_type;
  using has_trivial_assignment_operator = true_type;
  using has_trivial_destructor = true_type;
  using is_POD_type = true_type;
};

template<class T>
struct type_traits<const T *> {
  using has_trivial_default_constructor = true_type;
  using has_trivial_copy_constructor = true_type;
  using has_trivial_assignment_operator = true_type;
  using has_trivial_destructor = true_type;
  using is_POD_type = true_type;
};

template<>
struct type_traits<char *> {
  using has_trivial_default_constructor = true_type;
  using has_trivial_copy_constructor = true_type;
  using has_trivial_assignment_operator = true_type;
  using has_trivial_destructor = true_type;
  using is_POD_type = true_type;
};

template<>
struct type_traits<unsigned char *> {
  using has_trivial_default_constructor = true_type;
  using has_trivial_copy_constructor = true_type;
  using has_trivial_assignment_operator = true_type;
  using has_trivial_destructor = true_type;
  using is_POD_type = true_type;
};

template<>
struct type_traits<signed char *> {
  using has_trivial_default_constructor = true_type;
  using has_trivial_copy_constructor = true_type;
  using has_trivial_assignment_operator = true_type;
  using has_trivial_destructor = true_type;
  using is_POD_type = true_type;
};

template<>
struct type_traits<const char *> {
  using has_trivial_default_constructor = true_type;
  using has_trivial_copy_constructor = true_type;
  using has_trivial_assignment_operator = true_type;
  using has_trivial_destructor = true_type;
  using is_POD_type = true_type;
};

template<>
struct type_traits<const unsigned char *> {
  using has_trivial_default_constructor = true_type;
  using has_trivial_copy_constructor = true_type;
  using has_trivial_assignment_operator = true_type;
  using has_trivial_destructor = true_type;
  using is_POD_type = true_type;
};

template<>
struct type_traits<const signed char *> {
  using has_trivial_default_constructor = true_type;
  using has_trivial_copy_constructor = true_type;
  using has_trivial_assignment_operator = true_type;
  using has_trivial_destructor = true_type;
  using is_POD_type = true_type;
};

template<class T>
using has_trivial_default_constructor_t = typename type_traits<T>::has_trivial_default_constructor;

template<class T>
using has_trivial_copy_constructor_t = typename type_traits<T>::has_trivial_copy_constructor;

template<class T>
using has_trivial_assignment_operator_t = typename type_traits<T>::has_trivial_assignment_operator;

template<class T>
using has_trivial_destructor_t = typename type_traits<T>::has_trivial_destructor;

template<class T>
using is_POD_type_t = typename type_traits<T>::is_POD_type;

template<class T>
struct is_reference : false_type {};

template<class T>
struct is_reference<T &> : true_type {};

template<class T>
struct is_reference<T &&> : true_type {};

template<class T>
struct type_identity {
  using type = T;
};

// remove reference
template<class T>
struct remove_reference : type_identity<T> {};

template<class T>
struct remove_reference<T &> : type_identity<T> {};

template<class T>
struct remove_reference<T &&> : type_identity<T> {};

template<class T>
using remove_reference_t = typename remove_reference<T>::type;

namespace detail {
// add reference
template<class T>
auto try_add_lvalue_reference(int) -> type_identity<T &>;
template<class T>
auto try_add_lvalue_reference(...) -> type_identity<T>;

template<class T>
auto try_add_rvalue_reference(int) -> type_identity<T &&>;
template<class T>
auto try_add_rvalue_reference(...) -> type_identity<T>;
}// namespace detail

template<class T>
struct add_lvalue_reference : decltype(detail::try_add_lvalue_reference<T>(0)) {};

template<class T>
struct add_rvalue_reference : decltype(detail::try_add_rvalue_reference<T>(0)) {};

template<class T>
using add_lvalue_reference_t = typename add_lvalue_reference<T>::type;

template<class T>
using add_rvalue_reference_t = typename add_rvalue_reference<T>::type;

// usually use declval with decltype
// return T&&
template<class T>
add_rvalue_reference_t<T> declval() noexcept;

// remove pointer
template<class T>
struct remove_pointer {
  using type = T;
};

template<class T>
struct remove_pointer<T *> {
  using type = T;
};

template<class T>
struct remove_pointer<T *const> {
  using type = T;
};

template<class T>
struct remove_pointer<T *volatile> {
  using type = T;
};

template<class T>
struct remove_pointer<T *const volatile> {
  using type = T;
};

template<class T>
using remove_pointer_t = typename remove_pointer<T>::type;

namespace detail {
// aux for void_t
template<class T, class...>
struct type_t_ {
  using type = T;
};

//或許模板參數的第一個 (以此實現void_t)
template<class T, class... Ts>
using type_t = typename type_t_<T, Ts...>::type;
}// namespace detail

// 獲取void類型，用於SFINAE
template<class... Ts>
using void_t = detail::type_t<void, Ts...>;

// has_value_type<T>::value is true if T has a nested type `value_type`
template<class T, class = void>
struct has_value_type : false_type {};

template<class T>
struct has_value_type<T, void_t<typename T::value_type>> : true_type {};

template<typename, typename>
constexpr bool is_similar_instantiation_v = false;
template<template<typename...> class C, typename... A, typename... B>
constexpr bool
    is_similar_instantiation_v<C<A...>, C<B...>> = true;
template<typename A, typename B>
struct is_similar_instantiation
    : bool_constant<is_similar_instantiation_v<A, B>> {};

// Whether a value may be passed in a register.
inline constexpr std::size_t register_pass_max_size = 16u;
template<typename T>
constexpr bool is_register_pass_v =
    (sizeof(T) <= register_pass_max_size) && std::is_trivially_copyable<T>::value;
template<typename T>
constexpr bool is_register_pass_v<T &> = true;
template<typename T>
constexpr bool is_register_pass_v<T &&> = true;




}// namespace mystl



