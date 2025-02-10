#include "extern/libgte.hpp"
#include "extern/stddef.hpp"

extern "C" uint8_t* EFE_DATA_STACK;

template<class T> constexpr T pop()
{
    T retVal = *reinterpret_cast<T*>(EFE_DATA_STACK);
    EFE_DATA_STACK -= sizeof(T); // NOLINT(bugprone-sizeof-expression)
    return retVal;
}

extern "C"
{
    void EFE_rotateVector()
    {
        Vector* rotVector = pop<Vector*>();
        Vector* vector    = pop<Vector*>();

        Matrix matrix;
        SVector work;
        work.x = rotVector->x;
        work.y = rotVector->y;
        work.z = rotVector->z;
        libgte_RotMatrixZYX(&work, &matrix);
        work.x = vector->x;
        work.y = vector->y;
        work.z = vector->z;
        libgte_ApplyMatrixSV(&matrix, &work, &work);
        vector->x = work.x;
        vector->y = work.y;
        vector->z = work.z;
    }
}