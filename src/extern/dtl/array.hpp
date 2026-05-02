#pragma once

#include "types.hpp"

namespace dtl
{
    /*
     * A basic array implementation, analogous to std::array, but with fewer features
     */
    template<typename T, size_t elem_count> struct array
    {
        using value_type                      = T;
        static constexpr size_t element_count = elem_count;

        static_assert(elem_count != 0);

        T elements[elem_count];

        constexpr T* data() { return elements; }
        constexpr const T* data() const { return elements; }
        constexpr size_t size() const { return elem_count; }

        constexpr T& operator[](size_t idx) { return elements[idx]; }
        constexpr const T& operator[](size_t idx) const { return elements[idx]; }

        constexpr T* begin() { return elements; }
        constexpr const T* begin() const { return elements; }
        constexpr const T* cbegin() const { return elements; }

        constexpr T* end() { return elements + elem_count; }
        constexpr const T* end() const { return elements + elem_count; }
        constexpr const T* cend() const { return elements + elem_count; }

        constexpr bool contains(const T& value) const
        {
            for (const T& val : elements)
                if (val == value) return true;

            return false;
        };

        constexpr size_t indexOf(const T& value) const
        {
            for (size_t i = 0; i < elem_count; i++)
            {
                if (elements[i] == value) return i;
            }

            return 0xFFFFFFFF;
        }
    };

    static_assert(sizeof(array<uint32_t, 64>) == sizeof(uint32_t[64]));
} // namespace dtl