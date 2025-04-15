#pragma once

using uint8_t  = unsigned char;
using uint16_t = unsigned short;
using uint32_t = unsigned int;
using int8_t   = signed char;
using int16_t  = signed short;
using int32_t  = signed int;
using size_t   = uint32_t;

static_assert(sizeof(int32_t) == 4);
static_assert(sizeof(int16_t) == 2);
static_assert(sizeof(int8_t) == 1);
static_assert(sizeof(uint32_t) == 4);
static_assert(sizeof(uint16_t) == 2);
static_assert(sizeof(uint8_t) == 1);

// Digimon Template Library
namespace dtl
{
    /*
     * A basic array implementation, analogous to std::array, but with fewer features
     */
    template<typename T, size_t elem_count> struct array
    {
        static_assert(elem_count != 0);

        T elements[elem_count];

        constexpr T* data() { return elements; }
        constexpr const T* data() const { return elements; }
        constexpr size_t size() { return elem_count; }

        constexpr T& operator[](size_t idx) { return elements[idx]; }
        constexpr const T& operator[](size_t idx) const { return elements[idx]; }

        constexpr T* begin() { return elements; }
        constexpr const T* begin() const { return elements; }
        constexpr const T* cbegin() const { return elements; }

        constexpr T* end() { return elements + elem_count; }
        constexpr const T* end() const { return elements + elem_count; }
        constexpr const T* cend() const { return elements + elem_count; }
    };

    static_assert(sizeof(array<uint32_t, 64>) == sizeof(uint32_t[64]));
} // namespace dtl