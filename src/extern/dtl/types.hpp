#pragma once

namespace dtl
{
    using uint8_t  = unsigned char;
    using uint16_t = unsigned short;
    using uint32_t = unsigned int;
    using uint64_t = unsigned long long;
    using int8_t   = signed char;
    using int16_t  = signed short;
    using int32_t  = signed int;
    using int64_t  = signed long long;
    using size_t   = uint32_t;

    struct in_place_t
    {
        explicit in_place_t() = default;
    };
    inline constexpr in_place_t in_place{};

    static_assert(sizeof(int64_t) == 8);
    static_assert(sizeof(int32_t) == 4);
    static_assert(sizeof(int16_t) == 2);
    static_assert(sizeof(int8_t) == 1);
    static_assert(sizeof(uint64_t) == 8);
    static_assert(sizeof(uint32_t) == 4);
    static_assert(sizeof(uint16_t) == 2);
    static_assert(sizeof(uint8_t) == 1);
} // namespace dtl

using uint8_t  = dtl::uint8_t;
using uint16_t = dtl::uint16_t;
using uint32_t = dtl::uint32_t;
using uint64_t = dtl::uint64_t;
using int8_t   = dtl::int8_t;
using int16_t  = dtl::int16_t;
using int32_t  = dtl::int32_t;
using int64_t  = dtl::int64_t;
using size_t   = dtl::size_t;