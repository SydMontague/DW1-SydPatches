#include "../Math.hpp"
#include "../Matrix.hpp"
#include "Transform.hpp"

using namespace EFETransform;

namespace
{
    void toEulerAnglesXYZ(const Matrix& matrix, SVector& angles)
    {
        if (matrix.m00 == 0 && matrix.m22 == 0) {
            angles.x = libgte_ratan2(matrix.m21, matrix.m11);
            angles.y = matrix.m02 > 0 ? 1024 : -1024;
            angles.z = 0;
            return;
        }

        angles.x        = libgte_ratan2(-matrix.m12, matrix.m22);
        angles.z        = libgte_ratan2(-matrix.m01, matrix.m00);
        const auto sinX = libgte_rsin(angles.x);
        const auto cosX = libgte_rcos(angles.x);
        const auto sinZ = libgte_rsin(angles.z);
        const auto cosZ = libgte_rcos(angles.z);

        // Recover cos(Y) using the largest denominator for fixed-point precision.
        const auto largest = max(max(abs(sinX), abs(cosX)), max(abs(sinZ), abs(cosZ)));
        int32_t cosY;
        if (largest == abs(sinX))
            cosY = (-4096 * matrix.m12) / sinX;
        else if (largest == abs(cosX))
            cosY = (4096 * matrix.m22) / cosX;
        else if (largest == abs(sinZ))
            cosY = (-4096 * matrix.m01) / sinZ;
        else
            cosY = (4096 * matrix.m00) / cosZ;

        angles.y = libgte_ratan2(matrix.m02, cosY);
    }
} // namespace

extern "C"
{
    // Subopcodes 45 and 58 normalize the same three angles.
    void EFE_ENTRY(normalizeRotationAngles)()
    {
        auto rotation = toShortVector(popPointer<int32_t>());
        auto* out     = popPointer<int32_t>();
        Matrix matrix;
        libgte_RotMatrixYXZ(&rotation, &matrix);
        toEulerAnglesXYZ(matrix, rotation);
        writeVector(out, rotation);
    }

    void EFE_ENTRY(rotateShortVector)()
    {
        auto* words   = popPointer<int32_t>();
        auto rotation = toShortVector(popPointer<int32_t>());
        auto vector   = toShortVector(words);
        Matrix matrix;
        libgte_RotMatrixZYX(&rotation, &matrix);
        libgte_ApplyMatrixSV(&matrix, &vector, &vector);
        writeVector(words, vector);
    }

    void EFE_ENTRY(rotateVectorByAngles)()
    {
        auto* words   = popPointer<int32_t>();
        auto rotation = toShortVector(popPointer<int32_t>());
        Vector vector = {words[0], words[1], words[2]};
        rotateVector(&rotation, &vector, &vector);
        words[0] = vector.x;
        words[1] = vector.y;
        words[2] = vector.z;
    }

    void EFE_ENTRY(combineRotations)()
    {
        auto other    = toShortVector(popPointer<int32_t>());
        auto* out     = popPointer<int32_t>();
        auto rotation = toShortVector(out);
        if (other.x != 0 || other.y != 0 || other.z != 0) {
            Matrix first, second, combined;
            libgte_RotMatrixYXZ(&rotation, &first);
            libgte_RotMatrixYXZ(&other, &second);
            libgte_MulMatrix0(&first, &second, &combined);
            matrixToEuler2(&combined, &rotation);
        }
        writeVector(out, rotation);
    }
}
