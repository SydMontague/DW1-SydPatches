#include "algorithm.hpp"
#include "types.hpp"
#include "utility.hpp"

// forward declarations for placement new
void* operator new[](size_t size, void* p);
void* operator new(size_t size, void* p);

namespace dtl
{
    /**
     * A terrible basic vector, inspired but not 100% complatible by the STL version
     */
    template<typename T> class vector
    {
    public:
        using value_type      = T;
        using size_type       = size_t;
        using difference_type = size_t;
        using reference       = T&;
        using const_reference = const T&;
        using pointer         = T*;
        using const_pointer   = const T*;
        using iterator        = T*;
        using const_iterator  = const T*;

        vector() = default;
        vector(size_type count) { resize(count); }
        vector(size_type count, const T& value) { resize(count, value); }

        vector(vector&& other)
        {
            dtl::swap(other.end_ptr, end_ptr);
            dtl::swap(other.begin_ptr, begin_ptr);
            dtl::swap(other.capacity_ptr, capacity_ptr);
        }
        vector(const vector& other)
        {
            reserve(other.size());
            for (auto& i : other)
                push_back(i);
        }
        vector& operator=(const vector& other)
        {
            clear();
            reserve(other.size());

            for (auto& i : other)
                push_back(i);

            return *this;
        }

        vector& operator=(vector&& other)
        {
            if (&other == this) return *this;

            dtl::swap(other.end_ptr, end_ptr);
            dtl::swap(other.begin_ptr, begin_ptr);
            dtl::swap(other.capacity_ptr, capacity_ptr);
            return *this;
        }

        ~vector()
        {
            for (auto& i : *this)
                i.~T();

            delete[] begin_ptr;
        }

        reference at(size_type idx) { return *(begin() + idx); }
        const_reference at(size_type idx) const { return *(begin() + idx); }
        reference operator[](size_type idx) { return *(begin() + idx); }
        const_reference operator[](size_type idx) const { return *(begin() + idx); }
        reference front() { return *begin(); }
        const_reference front() const { return *begin(); }
        reference back() { return *(end() - 1); }
        const_reference back() const { return *(end() - 1); }

        T* data() { return begin(); }
        const T* data() const { return begin(); }

        iterator begin() { return begin_ptr; }
        const_iterator begin() const { return begin_ptr; }
        const_iterator cbegin() const { return begin_ptr; }

        iterator end() { return end_ptr; }
        const_iterator end() const { return end_ptr; }
        const_iterator cend() const { return end_ptr; }

        bool empty() const { return begin_ptr == end_ptr; }
        size_type size() const { return end_ptr - begin_ptr; }
        void reserve(size_type new_cap)
        {
            if (capacity() >= new_cap) return;

            const auto old_size = size();

            auto* new_ptr = new T[new_cap];
            dtl::move(begin_ptr, end_ptr, new_ptr);
            delete[] begin_ptr;

            capacity_ptr = new_ptr + new_cap;
            end_ptr      = new_ptr + old_size;
            begin_ptr    = new_ptr;
        }
        void resize(size_type new_size) { resize(new_size, T()); }
        void resize(size_type new_size, const value_type& value)
        {
            if (size() > new_size) {
                auto diff = size() - new_size;
                for (auto i = 0; i < diff; i++)
                    pop_back();
            }
            else {
                auto diff = new_size - size();
                for (auto i = 0; i < diff; i++)
                    push_back(value);
            }
        }

        size_type capacity() { return capacity_ptr - begin_ptr; }

        void clear()
        {
            for (int32_t i = size(); i > 0; i--)
                pop_back();
        }

        template<class... Args> void emplace_back(Args&&... args)
        {
            if (size() == capacity()) reserve(size() + 1);

            new (end_ptr) value_type(dtl::forward<Args>(args)...);
            end_ptr++;
        }
        void push_back(const T& value)
        {
            if (size() == capacity()) reserve(size() + 1);

            *end_ptr = value;
            end_ptr++;
        }
        void push_back(T&& value) { emplace_back(dtl::move(value)); }
        void pop_back()
        {
            if (end_ptr == begin_ptr) return;
            end_ptr--;
            end_ptr->~T();
        }

    private:
        value_type* begin_ptr    = nullptr;
        value_type* end_ptr      = nullptr;
        value_type* capacity_ptr = nullptr;
    };
} // namespace dtl
