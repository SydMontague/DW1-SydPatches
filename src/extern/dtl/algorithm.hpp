#pragma once

#include "type_traits.hpp"

namespace dtl
{
    template<typename T> using Comparator = bool(const T& left, const T& right);

    template<typename T> constexpr void swap(T& left, T& right)
    {
        T tmp = left;
        left  = right;
        right = tmp;
    }

    template<typename T> constexpr bool less(const T& left, const T& right)
    {
        return left < right;
    }

    template<typename T> T* __sort_partition(T* first, T* last, Comparator<T> compare)
    {
        auto* pivot = last - 1;
        auto* i     = first;
        auto* j     = first;

        while (j != pivot)
        {
            if (compare(*j, *pivot))
            {
                swap(*i, *j);
                i++;
            }
            j++;
        }

        swap(*i, *pivot);
        return i;
    }

    template<typename T> void sort(T* first, T* last, Comparator<T> compare = less)
    {
        if (first >= last - 1) return;

        auto* partition = __sort_partition(first, last, compare);
        sort(first, partition, compare);
        sort(partition + 1, last, compare);
    }

    template<class InputItr, class OutputItr> constexpr OutputItr copy(InputItr begin, InputItr end, OutputItr output)
    {
        for (; begin != end; begin++, output++)
            *output = *begin;

        return output;
    }

    template<typename T> remove_reference_t<T>&& move(T&& val)
    {
        return static_cast<remove_reference<T>&&>(val);
    }
} // namespace dtl