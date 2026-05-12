#include "Effects.hpp"
#include "GameObjects.hpp"
#include "Helper.hpp"
#include "Math.hpp"
#include "extern/dtl/types.hpp"
#include "extern/dw1.hpp"
#include "extern/libgte.hpp"

namespace
{
    constexpr dtl::array<RGB8, 18> PARTICLE_COLOR1 = {{
        {0x02, 0x04, 0x01},
        {0x04, 0x06, 0x02},
        {0x07, 0x09, 0x03},
        {0x0a, 0x0b, 0x03},
        {0x0c, 0x0d, 0x04},
        {0x0f, 0x10, 0x04},
        {0x12, 0x12, 0x05},
        {0x15, 0x15, 0x06},
        {0x17, 0x17, 0x06},
        {0x1a, 0x19, 0x07},
        {0x1d, 0x1c, 0x07},
        {0x1f, 0x1e, 0x08},
        {0x22, 0x21, 0x09},
        {0x25, 0x23, 0x09},
        {0x27, 0x25, 0x0a},
        {0x2a, 0x28, 0x0a},
        {0x2d, 0x2a, 0x0b},
        {0x30, 0x2d, 0x0c},
    }};

    constexpr dtl::array<RGB8, 18> PARTICLE_COLOR2{{
        {0x04, 0x09, 0x03},
        {0x0a, 0x0e, 0x05},
        {0x10, 0x13, 0x06},
        {0x16, 0x18, 0x07},
        {0x1b, 0x1d, 0x09},
        {0x21, 0x23, 0x0a},
        {0x27, 0x28, 0x0b},
        {0x2d, 0x2d, 0x0d},
        {0x33, 0x32, 0x0e},
        {0x39, 0x37, 0x0f},
        {0x3f, 0x3d, 0x10},
        {0x44, 0x42, 0x12},
        {0x4a, 0x47, 0x13},
        {0x50, 0x4c, 0x14},
        {0x56, 0x51, 0x16},
        {0x5c, 0x57, 0x17},
        {0x62, 0x5c, 0x18},
        {0x68, 0x61, 0x1a},
    }};
    constexpr dtl::array<RGB8, 5> PARTICLE_COLOR3{{
        {0x47, 0x51, 0x0},
        {0x60, 0x6b, 0x19},
        {0x7d, 0x86, 0x3d},
        {0xb3, 0xbb, 0x7a},
        {0xd7, 0xdc, 0xb7},
    }};

    struct HealingParticleData
    {
        int16_t counter;
        SVector pos;
    };

    struct HealingParticle
    {
        int16_t frameId;
        int16_t offsetX;
        int16_t offsetY;
        int16_t offsetZ;
        int16_t radius;
        bool hasParticle2;
        dtl::array<HealingParticleData, 20> particles1;
        dtl::array<HealingParticleData, 20> particles2;
    };

    HealingParticle particles;

    void tickHealingParticles(int32_t instanceId)
    {
        auto frameId = particles.frameId++;

        if (frameId >= 44)
        {
            particles.frameId = -1;
            removeObject(ObjectID::HEALING_PARTICLES, instanceId);
            return;
        }

        if (frameId < 20)
        {
            Matrix mat;
            auto& particle = particles.particles1[frameId];

            particle.counter = 17;
            SVector rot      = {.x = 0, .y = static_cast<int16_t>(frameId * 0x280 & 0xFFF), .z = 0};
            SVector pos      = {.x = particles.radius, .y = 0, .z = 0};
            libgte_RotMatrixZYX(&rot, &mat);
            libgte_ApplyMatrixSV(&mat, &pos, &particle.pos);

            particle.pos.x += particles.offsetX;
            particle.pos.y += particles.offsetY - lerp(0, particles.offsetY, 0, 20, frameId);
            particle.pos.z += particles.offsetZ;
        }

        for (auto& particle : particles.particles1)
        {
            particle.counter--;
            particle.pos.pad = (rand() % 70) + 15;
        }

        if (frameId < 39)
        {
            auto id = frameId % 5;

            for (int32_t i = 0; i < 4; i++)
            {
                auto& particle = particles.particles2[id + i * 5];

                auto val1          = lerp(0, 512, 0, 13, frameId);
                auto val2          = i * 13;
                auto progressValue = val1 + 512 / (val2 / 4);
                auto sinVal        = sin(progressValue);
                auto cosVal        = cos(progressValue);

                particle.counter = 5;
                particle.pos.x   = particles.offsetX + rand() % 60 - 30 - (particles.radius + 150) * sinVal;
                particle.pos.z   = particles.offsetZ + rand() % 60 - 30 + (particles.radius + 150) * cosVal;
                particle.pos.y   = lerp(particles.offsetY + rand() % 60 - 30, 0, 0, 0x27, frameId);
            }
        }

        for (auto& particle : particles.particles2)
            particle.counter--;
    }

    void renderHealingParticles(int32_t instanceId)
    {
        for (auto& particle : particles.particles1)
        {
            if (particle.counter < 0) continue;

            renderFXParticle(&particle.pos, particle.pos.pad, &PARTICLE_COLOR1[particle.counter]);
            renderFXParticle(&particle.pos, particle.pos.pad / 2, &PARTICLE_COLOR2[particle.counter]);
        }

        if (!particles.hasParticle2) return;

        for (auto& particle : particles.particles2)
        {
            if (particle.counter < 0) continue;

            renderFXParticle(&particle.pos, 24, &PARTICLE_COLOR3[particle.counter]);
        }
    }
} // namespace

void addHealingParticleEffect(Entity* entity, bool param)
{
    // vanilla treats these as if HEALING_PARTICLES is an array, but there is only one instance
    auto data = getDigimonData(entity->type);

    particles.frameId      = 0;
    particles.offsetX      = entity->posData->posMatrix.work.t[0];
    particles.offsetY      = -70 - data->height;
    particles.offsetZ      = entity->posData->posMatrix.work.t[2];
    particles.radius       = data->radius * 12 / 10;
    particles.hasParticle2 = param;

    for (int32_t i = 0; i < particles.particles1.size(); i++)
    {
        particles.particles1[i].counter = -1;
        particles.particles2[i].counter = -1;
    }

    addObject(ObjectID::HEALING_PARTICLES, 0, tickHealingParticles, renderHealingParticles);
}

void initializeHealingParticles()
{
    particles.frameId = -1;
}