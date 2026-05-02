#pragma once

#include "type_traits.hpp"

namespace dtl
{
    // TODO converting default_delete missing
    template<typename T> struct default_delete
    {
        constexpr default_delete() noexcept = default;

        void operator()(T* ptr) const { delete ptr; }
    };
    template<typename T> struct default_delete<T[]>
    {
        constexpr default_delete() noexcept = default;

        void operator()(T* ptr) const { delete[] ptr; }
    };

    /*
     * A basic unique_ptr implementation, analogous to std::unique_ptr, but with fewer features.
     */
    template<typename T, class Deleter = default_delete<T>> class unique_ptr
    {
    public:
        using pointer   = T*;
        using reference = T&;

        constexpr unique_ptr() noexcept
            : value(nullptr)
        {
        }
        constexpr unique_ptr(pointer val)
            : value(val)
        {
        }
        constexpr ~unique_ptr()
        {
            if (value) Deleter()(value);
        }
        constexpr unique_ptr(const unique_ptr& other) = delete;
        constexpr unique_ptr(unique_ptr&& other)
            : value(other.release())
        {
        }

        constexpr pointer release() noexcept
        {
            auto old = get();
            value    = nullptr;
            return old;
        }
        constexpr void reset(pointer ptr = pointer()) noexcept
        {
            auto old = get();
            value    = ptr;
            if (old) Deleter()(old);
        }
        constexpr void swap(unique_ptr& other) { swap(value, other.value); }
        constexpr pointer get() { return value; }
        constexpr const pointer get() const { return value; }

        constexpr unique_ptr& operator=(const unique_ptr& other) = delete;
        constexpr unique_ptr& operator=(unique_ptr&& other) { reset(other.release()); }
        constexpr explicit operator bool() { return get() != nullptr; }
        constexpr pointer operator->() const noexcept { return get(); }
        constexpr reference operator*() const noexcept { return *get(); }

    private:
        pointer value;
    };

    template<typename T, class Deleter> class unique_ptr<T[], Deleter>
    {
    public:
        using pointer   = T*;
        using reference = T&;

        constexpr unique_ptr() noexcept
            : value(nullptr)
        {
        }
        constexpr unique_ptr(pointer val)
            : value(val)
        {
        }
        constexpr ~unique_ptr()
        {
            if (value) Deleter()(value);
        }
        constexpr unique_ptr(const unique_ptr& other) = delete;
        constexpr unique_ptr(unique_ptr&& other)
            : value(other.release())
        {
        }

        constexpr pointer release() noexcept
        {
            auto old = get();
            value    = nullptr;
            return old;
        }
        constexpr void reset(pointer ptr = pointer()) noexcept
        {
            auto old = get();
            value    = ptr;
            if (old) Deleter()(old);
        }
        constexpr void swap(unique_ptr& other) { swap(value, other.value); }
        constexpr pointer get() { return value; }
        constexpr const pointer get() const { return value; }

        constexpr unique_ptr& operator=(const unique_ptr& other) = delete;
        constexpr unique_ptr& operator=(unique_ptr&& other) { reset(other.release()); }
        constexpr explicit operator bool() { return get() != nullptr; }
        constexpr pointer operator->() const noexcept { return get(); }
        constexpr reference operator*() const noexcept { return *get(); }

    private:
        pointer value;
    };

    template<IsUnboundArray T> constexpr unique_ptr<T> make_unique(size_t size)
    {
        return unique_ptr<T>(new remove_extent<T>::type[size]);
    }

    template<IsNotArray T, typename... Args> constexpr unique_ptr<T> make_unique(Args&&... args)
    {
        return unique_ptr<T>(new T(args...));
    }
} // namespace dtl