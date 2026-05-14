#pragma once
#include "algorithm.hpp"
#include "types.hpp"
#include "unique_ptr.hpp"

namespace dtl
{
    template<typename T> struct runtime_array
    {
    public:
        using value_type = T;

        friend void swap(runtime_array& first, runtime_array& second)
        {
            using dtl::swap;

            swap(first.elements, second.elements);
            swap(first.elem_count, second.elem_count);
        }

        constexpr runtime_array()  = default;
        constexpr ~runtime_array() = default;
        constexpr runtime_array(size_t size)
            : elements(make_unique<T[]>(size))
            , elem_count(size)
        {
        }
        constexpr runtime_array(const runtime_array& other)
        {
            elem_count = other.elem_count;
            elements   = make_unique<T[]>(elem_count);
            dtl::copy(other.begin(), other.end(), begin());
        }
        constexpr runtime_array(runtime_array&& other) { swap(*this, other); }
        constexpr runtime_array& operator=(const runtime_array& other)
        {
            elem_count = other.elem_count;
            elements   = make_unique<T[]>(elem_count);
            dtl::copy(other.begin(), other.end(), begin());
            return *this;
        }
        constexpr runtime_array& operator=(runtime_array&& other)
        {
            swap(*this, other);
            return *this;
        }

        constexpr T* data() { return elements.get(); }
        constexpr const T* data() const { return elements.get(); }
        constexpr size_t size() const { return elem_count; }

        constexpr T& operator[](size_t idx) { return elements.get()[idx]; }
        constexpr const T& operator[](size_t idx) const { return elements.get()[idx]; }

        constexpr T* begin() { return elements.get(); }
        constexpr const T* begin() const { return elements.get(); }
        constexpr const T* cbegin() const { return elements.get(); }

        constexpr T* end() { return elements.get() + elem_count; }
        constexpr const T* end() const { return elements.get() + elem_count; }
        constexpr const T* cend() const { return elements.get() + elem_count; }

        constexpr bool contains(const T& value) const
        {
            for (const T& val : this)
                if (val == value) return true;

            return false;
        };

        constexpr size_t indexOf(const T& value) const
        {
            for (size_t i = 0; i < elem_count; i++)
            {
                if (elements.get()[i] == value) return i;
            }

            return 0xFFFFFFFF;
        }

    private:
        unique_ptr<T[]> elements = nullptr;
        size_t elem_count        = 0;
    };
} // namespace dtl