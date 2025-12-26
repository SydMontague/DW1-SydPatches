#include "Matrix.hpp"

#include "Math.hpp"
#include "extern/dw1.hpp"
#include "extern/libc.hpp"
#include "extern/libgs.hpp"
#include "extern/libgte.hpp"

namespace
{
    static uint32_t customRng1 = 0;
    static uint32_t customRng2 = 0x0013CC25;

    void toLocalVector(SVector* input, Vector* output)
    {
        Matrix matrix;
        SVector tmp2 = {
            .x = static_cast<int16_t>(input->x - libgs_REFERENCE_MATRIX.t[0]),
            .y = static_cast<int16_t>(input->y - libgs_REFERENCE_MATRIX.t[1]),
            .z = static_cast<int16_t>(input->z - libgs_REFERENCE_MATRIX.t[2]),
        };

        libgte_TransposeMatrix(&libgs_REFERENCE_MATRIX, &matrix);
        libgte_ApplyMatrix(&matrix, &tmp2, output);
    }
} // namespace

extern "C"
{
    int32_t customRandom(int32_t min, int32_t max)
    {
        constexpr uint32_t CUSTOM_RNG_FACTOR = 1103515821;
        constexpr uint32_t CUSTOM_RNG_VALUE  = 12345;

        if (min == max) return max;
        if (min > max) dtl::swap(min, max);

        customRng1 = customRng1 * CUSTOM_RNG_FACTOR + CUSTOM_RNG_VALUE;
        customRng2 = customRng2 * CUSTOM_RNG_FACTOR + CUSTOM_RNG_VALUE;

        return min + ((customRng1 >> 0x10 | customRng2 << 0x10) % (max - min)) + 1U;
    }

    void mapToWorldPosition(Vector* result, int16_t posX, int16_t posY, int32_t* hasValuePtr)
    {
        if (hasValuePtr != nullptr) *hasValuePtr = 0;

        SVector in1 = {0, 0, 0};
        SVector in2 = {posX, posY, static_cast<int16_t>(VIEWPORT_DISTANCE)};
        Vector out1;
        Vector out2;

        toLocalVector(&in1, &out1);
        toLocalVector(&in2, &out2);

        auto diff = out2.y - out1.y;

        if (out1.y == 0) return;
        if (diff == 0) return;
        if (out1.y > 0 == diff > 0) return;

        result->x = out1.x + ((out2.x - out1.x) * out1.y) / -diff;
        result->y = 0;
        result->z = out1.z + ((out2.z - out1.z) * out1.y) / -diff;
        if (hasValuePtr != nullptr) *hasValuePtr = 1;
    }

    void rotateVector(SVector* rotation, Vector* input, Vector* output)
    {
        Matrix matrix;
        libgte_RotMatrixYXZ(rotation, &matrix);
        libgte_ApplyMatrixLV(&matrix, input, output);
    }

    int32_t getDistance(int32_t diffX, int32_t diffY, int32_t diffZ)
    {
        // vanilla implements its own sqrt function, but the libgte version should be good enough
        return libgte_SquareRoot0(diffX * diffX + diffY * diffY + diffZ * diffZ);
    }

    void toEulerAngles(SVector* output, int32_t deltaX, int32_t deltaY, int32_t deltaZ)
    {
        output->x = -libgte_ratan2(deltaY, getDistance(deltaX, 0, deltaZ)) & 0xFFF;
        output->y = libgte_ratan2(deltaX, deltaZ) & 0xFFF;
        output->z = 0;

        if (output->x >= 0x800) output->x -= 0x1000;
        if (output->y >= 0x800) output->y -= 0x1000;
    }

    void matrixToEuler1(Matrix* input, SVector* output)
    {
        if (input->m00 == 0 && input->m22 == 0)
        {
            output->x = libgte_ratan2(input->m21, input->m11);
            output->y = input->m02 > 0 ? 1024 : -1024;
            output->z = 0;
            return;
        }

        output->x = libgte_ratan2(-input->m12, input->m22);
        output->z = libgte_ratan2(-input->m01, input->m00);

        auto rsinX = libgte_rsin(output->x);
        auto rcosX = libgte_rcos(output->x);
        auto rsinZ = libgte_rsin(output->z);
        auto rcosZ = libgte_rcos(output->z);

        auto absRsinX = abs(rsinX);
        auto absRcosX = abs(rcosX);
        auto absRsinZ = abs(rsinZ);
        auto absRcosZ = abs(rcosZ);

        auto maxZ       = max(absRsinZ, absRcosZ);
        auto maxX       = max(absRsinX, absRcosX);
        auto maxOverall = max(maxX, maxZ);

        auto val = 0;
        if (maxOverall == absRsinX)
            val = (-4096 * input->m12) / rsinX;
        else if (maxOverall == absRcosX)
            val = (4096 * input->m22) / rcosX;
        else if (maxOverall == absRsinZ)
            val = (-4096 * input->m01) / rsinZ;
        else if (maxOverall == absRcosZ)
            val = (4096 * input->m00) / rcosZ;

        output->y = libgte_ratan2(input->m02, val);
    }

    void matrixToEuler2(Matrix* input, SVector* output)
    {
        if (input->m02 == 0 && input->m22 == 0)
        {
            output->y = libgte_ratan2(-input->m20, input->m00);
            output->x = input->m12 > 0 ? -1024 : 1024;
            output->z = 0;
            return;
        }

        output->y = libgte_ratan2(input->m02, input->m22);
        output->z = libgte_ratan2(input->m10, input->m11);

        auto rsinY = libgte_rsin(output->y);
        auto rcosY = libgte_rcos(output->y);
        auto rsinZ = libgte_rsin(output->z);
        auto rcosZ = libgte_rcos(output->z);

        auto absRsinY = abs(rsinY);
        auto absRcosY = abs(rcosY);
        auto absRsinZ = abs(rsinZ);
        auto absRcosZ = abs(rcosZ);

        auto maxZ       = max(absRsinZ, absRcosZ);
        auto maxY       = max(absRsinY, absRcosY);
        auto maxOverall = max(maxY, maxZ);

        auto val = 0;
        if (maxOverall == absRsinY)
            val = (4096 * input->m02) / rsinY;
        else if (maxOverall == absRcosY)
            val = (4096 * input->m22) / rcosY;
        else if (maxOverall == absRsinZ)
            val = (4096 * input->m10) / rsinZ;
        else if (maxOverall == absRcosZ)
            val = (4096 * input->m11) / rcosZ;

        output->x = libgte_ratan2(-input->m12, val);
    }

    void calculatePosition(GsCOORDINATE2* coord, Matrix* matrix)
    {
        dtl::array<GsCOORDINATE2*, 100> arr;
        int32_t i = 0;

        while (coord != nullptr)
        {
            arr[i++] = coord;
            coord    = coord->super;
        }

        *matrix = arr[i - 1]->coord;
        for (int j = i - 2; j >= 0; j--)
            libgs_GsMulCoord3(matrix, &arr[j]->coord);
    }

    void multiplyRotations(SVector* rotation1, SVector* rotation2)
    {
        if (rotation2->x == 0 && rotation2->y == 0 && rotation2->z == 0) return;

        Matrix matrix1;
        Matrix matrix2;
        Matrix matrix3;

        libgte_RotMatrixYXZ(rotation1, &matrix1);
        libgte_RotMatrixYXZ(rotation2, &matrix2);
        libgte_MulMatrix0(&matrix1, &matrix2, &matrix3);
        matrixToEuler2(&matrix3, rotation1);
    }
}