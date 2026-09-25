

#include "type_traits.hpp"

namespace dtl
{
    template<class T> constexpr T&& forward(typename dtl::remove_reference_t<T>& t) noexcept
    {
        return static_cast<T&&>(t);
    }

    template<class T> constexpr T&& forward(typename dtl::remove_reference_t<T>&& t) noexcept
    {
        return static_cast<T&&>(t);
    }
} // namespace dtl
