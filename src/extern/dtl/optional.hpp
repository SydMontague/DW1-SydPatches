#pragma once

#include "algorithm.hpp"
#include "type_traits.hpp"
#include "types.hpp"

namespace dtl
{
    struct nullopt_t
    {
        constexpr explicit nullopt_t() {}
    };

    inline constexpr nullopt_t nullopt{};

    /*
     * A basic optional implementation, analogous to std::optional, but with fewer features.
     */
    template<typename T> class optional
    {
    public:
        using value_type = T;

        constexpr optional() {}
        constexpr optional(nullopt_t) {}

        constexpr optional(const optional& other)
        {
            if (other.has_value())
            {
                value_     = *other;
                has_value_ = true;
            }
        }

        constexpr optional(optional&& other)
        {
            if (other.has_value()) value_ = dtl::move(other.value_);
            has_value_ = other.has_value_;
        }

        template<typename... Args>
        constexpr optional(in_place_t, Args&&... args)
            : value_(args...)
            , has_value_(true)
        {
        }

        template<typename U = remove_cv_t<T>>
        requires(!is_same_v<optional, remove_reference_t<U>>)
        constexpr optional(U&& val)
            : value_(val)
            , has_value_(true)
        {
        }

        constexpr optional& operator=(const optional& other)
        {
            has_value_ = other.has_value_;
            if (has_value_) value_ = other.value_;
            return *this;
        }
        constexpr optional& operator=(optional&& other)
        {
            if (other.has_value()) value_ = dtl::move(other.value_);
            has_value_ = other.has_value_;
            return *this;
        }

        constexpr ~optional()
        {
            if (has_value_) value_.~T();
        }

        constexpr bool has_value() const noexcept { return has_value_; };
        constexpr const value_type& value() const { return value_; };
        constexpr value_type value_or(value_type&& default_value) const noexcept
        {
            return has_value_ ? value_ : default_value;
        }
        constexpr const value_type* operator->() const noexcept { return &value_; }
        constexpr const value_type& operator*() const noexcept { return value_; }
        constexpr value_type* operator->() noexcept { return &value_; }
        constexpr value_type& operator*() noexcept { return value_; }
        constexpr void reset() noexcept
        {
            if (has_value_) value_.~T();
            has_value_ = false;
        }

    private:
        union
        {
            uint8_t dummy_;
            T value_;
        };
        bool has_value_{false};
    };
} // namespace dtl