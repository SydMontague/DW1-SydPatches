#pragma once

#include "array.hpp"
#include "types.hpp"

namespace dtl
{
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
} // namespace dtl