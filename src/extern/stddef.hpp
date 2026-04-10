#pragma once

using uint8_t  = unsigned char;
using uint16_t = unsigned short;
using uint32_t = unsigned int;
using uint64_t = unsigned long long;
using int8_t   = signed char;
using int16_t  = signed short;
using int32_t  = signed int;
using int64_t  = signed long long;
using size_t   = uint32_t;

static_assert(sizeof(int64_t) == 8);
static_assert(sizeof(int32_t) == 4);
static_assert(sizeof(int16_t) == 2);
static_assert(sizeof(int8_t) == 1);
static_assert(sizeof(uint64_t) == 8);
static_assert(sizeof(uint32_t) == 4);
static_assert(sizeof(uint16_t) == 2);
static_assert(sizeof(uint8_t) == 1);

// Digimon Template Library
namespace dtl
{
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

    template<typename T> remove_reference_t<T>&& move(T&& val)
    {
        return static_cast<remove_reference<T>&&>(val);
    }
    struct in_place_t
    {
        explicit in_place_t() = default;
    };
    inline constexpr in_place_t in_place{};

    template<typename T> using Comparator = bool(const T& left, const T& right);

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

    template<typename T, size_t size> struct RingQueue
    {
    private:
        dtl::array<T, size> array{};
        size_t readIndex{0};
        size_t writeIndex{0};
        size_t entryCount{0};

    public:
        bool push(const T& entry)
        {
            if (entryCount >= size) return false;

            array[writeIndex] = entry;
            writeIndex        = (writeIndex + 1) % size;
            entryCount++;
            return true;
        }

        T* front()
        {
            if (entryCount == 0) return nullptr;

            return &array[readIndex];
        }

        void pop()
        {
            if (entryCount == 0) return;

            array[readIndex] = {};
            readIndex        = (readIndex + 1) % size;
            entryCount--;
        }

        size_t getSize() { return entryCount; }
    };

    /*
     * A basic unique_ptr implementation, analogous to std::unique_ptr, but with fewer features.
     * In particular arrays are not implemented as of yet, but may be if the need arises.
     */
    template<typename T> class unique_ptr
    {
    public:
        using pointer   = T*;
        using reference = T&;

        constexpr unique_ptr() noexcept {}
        constexpr unique_ptr(pointer val)
            : value(val)
        {
        }
        constexpr ~unique_ptr()
        {
            if (value) delete value;
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
            if (old) delete old;
        }
        constexpr void swap(unique_ptr& other) { swap(value, other.value); }
        constexpr pointer get() { return value; }

        constexpr unique_ptr& operator=(const unique_ptr& other) = delete;
        constexpr unique_ptr& operator=(unique_ptr&& other) { reset(other.release()); }
        constexpr explicit operator bool() { return get() != nullptr; }
        constexpr pointer operator->() const noexcept { return get(); }
        constexpr reference operator*() const noexcept { return *get(); }

    private:
        pointer value;
    };

    /*
     * A basic optional implementation, analogous to std::optional, but with fewer features.
     */
    template<typename T> class optional
    {
    public:
        using value_type = T;

        constexpr optional() {}
        constexpr optional(T&& val)
            : value_(val)
            , has_value_(true)
        {
        }
        constexpr optional(const optional& other)
        {
            if (other.has_value())
            {
                value_     = *other;
                has_value_ = true;
            }
        }
        template<typename... Args>
        constexpr optional(in_place_t, Args&&... args)
            : value_(args...)
            , has_value_(true)
        {
        }

        constexpr optional(optional&& other)
        {
            if (other.has_value()) value_ = dtl::move(other.value_);
            has_value_ = other.has_value_;
        }

        constexpr optional& operator=(const optional& other)
        {
            has_value_ = other.has_value_;
            if (has_value_) value_ = other.value_;
        }
        constexpr optional& operator=(optional&& other)
        {
            if (other.has_value()) value_ = dtl::move(other.value_);
            has_value_ = other.has_value_;
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

    template<typename T, typename... Args> constexpr unique_ptr<T> make_unique(Args&&... args)
    {
        return unique_ptr<T>(new T(args...));
    }

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

    static_assert(sizeof(array<uint32_t, 64>) == sizeof(uint32_t[64]));
} // namespace dtl