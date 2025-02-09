#include "Math.hpp"
#include "extern/dw1.hpp"
#include "extern/libgte.hpp"
#include "extern/stddef.hpp"

extern "C"
{
    void initializeParticleFX()
    {
        for (ParticleFXData& data : PARTICLE_FX_DATA)
            data.tickCount = -1;

        for (SPosition3D& data : FX_PARTICLE_DATA)
        {
            Matrix matrix;
            SVector rotation;
            rotation.x = ((random2(128) - 64) << 6);
            rotation.y = ((random2(128) - 64) << 6);
            rotation.z = ((random2(128) - 64) << 6);
            SVector vec2;
            vec2.x = 0;
            vec2.y = 0;
            vec2.z = (random2(150) + 250) * 16;

            libgte_RotMatrixZYX(&rotation, &matrix);
            libgte_ApplyMatrixSV(&matrix, &vec2, &vec2);

            data.x = vec2.x;
            data.y = vec2.y;
            data.z = vec2.z;
        }
    }
}