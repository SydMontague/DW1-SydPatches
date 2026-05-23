#pragma once

#include "types.hpp"

namespace dtl
{
    struct true_type
    {
        static constexpr bool value = true;
    };

    struct false_type
    {
        static constexpr bool value = false;
    };

    template<class T> struct is_array : false_type
    {
    };

    template<class T> struct is_array<T[]> : true_type
    {
    };

    template<class T, size_t N> struct is_array<T[N]> : true_type
    {
    };

    template<class T> struct is_unbound_array : false_type
    {
    };

    template<class T> struct is_unbound_array<T[]> : true_type
    {
    };

    template<class T> struct is_bound_array : false_type
    {
    };

    template<class T, size_t N> struct is_bound_array<T[N]> : true_type
    {
    };

    template<typename T>
    concept IsArray = is_array<T>::value;
    template<typename T>
    concept IsBoundArray = is_bound_array<T>::value;
    template<typename T>
    concept IsUnboundArray = is_unbound_array<T>::value;
    template<typename T>
    concept IsNotArray = !is_array<T>::value;

    template<typename T> struct remove_extent
    {
        using type = T;
    };
    template<typename T> struct remove_extent<T[]>
    {
        using type = T;
    };
    template<typename T, size_t N> struct remove_extent<T[N]>
    {
        using type = T;
    };

    template<typename T> struct remove_reference
    {
        using type = T;
    };
    template<typename T> struct remove_reference<T&>
    {
        using type = T;
    };
    template<typename T> struct remove_reference<T&&>
    {
        using type = T;
    };
    template<typename T> using remove_reference_t = remove_reference<T>::type;

    template<class T> struct remove_cv
    {
        using type = T;
    };
    template<class T> struct remove_cv<const T>
    {
        using type = T;
    };
    template<class T> struct remove_cv<volatile T>
    {
        using type = T;
    };
    template<class T> struct remove_cv<const volatile T>
    {
        using type = T;
    };
    template<class T> using remove_cv_t = typename remove_cv<T>::type;

    template<class T, class U> struct is_same : dtl::false_type
    {
    };
    template<class T> struct is_same<T, T> : dtl::true_type
    {
    };
    template<class T, class U> constexpr bool is_same_v = is_same<T, U>::value;

} // namespace dtl
