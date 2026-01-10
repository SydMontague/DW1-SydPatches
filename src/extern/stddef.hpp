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
    template<typename T> using Comparator = bool(const T& left, const T& right);

    /*
     * A basic array implementation, analogous to std::array, but with fewer features
     */
    template<typename T, size_t elem_count> struct array
    {
        using value_type = T;
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