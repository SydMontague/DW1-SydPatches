#include "../Effects.hpp"
#include "../Math.hpp"
#include "../UIElements.hpp"
#include "../extern/dtl/optional.hpp"
#include "../extern/dtl/utility.hpp"
#include "Operands.hpp"

namespace
{
    struct Particle
    {
        int16_t distance;
        int16_t velocity;
        SVector direction;
        SVector position;
    };

    struct Emitter
    {
        EFEInstance* instance;
        int16_t frames;
        int16_t startOffset;
        int16_t startVelocity;
        int16_t acceleration;
        RGB8 color;
        int8_t type;
        dtl::array<Particle, 21> particles;
    };

    // The active EFE engine lends the arena. Every engine entry replaces this
    // pointer before registering its callbacks; removal precedes overlay replacement.
    dtl::array<Emitter, 4>* emitters = nullptr;

    inline dtl::optional<Emitter*> findFreeEmitter()
    {
        for (auto& entry : *emitters)
            if (entry.instance == nullptr) return &entry;
        return dtl::nullopt;
    }

    inline dtl::optional<Particle*> findFreeParticle(Emitter& entry)
    {
        // The twenty-first particle is an inactive sentinel, never a spawn slot.
        for (auto* particle = entry.particles.begin(); particle != entry.particles.end() - 1; ++particle)
            if (particle->distance <= 0) return particle;
        return dtl::nullopt;
    }

    struct EndpointFactors
    {
        int32_t first;
        int32_t second;
    };

    inline EndpointFactors endpointFactors(const Emitter& entry, const Particle& particle)
    {
        int32_t first;
        int32_t second;
        switch (entry.type) {
            case 0:
                first  = particle.distance * 16;
                second = (particle.distance + 7) * 16;
                break;
            case 1:
                first  = particle.distance * 16;
                second = particle.distance * 4;
                break;
            case 2:
                first  = (entry.startOffset - particle.distance) * 16;
                second = (entry.startOffset - particle.distance - 5) * 16;
                break;
            case 3:
                first  = (entry.startOffset - particle.distance) * 16;
                second = (entry.startOffset - particle.distance) * 4;
                break;
            default:
                // The EFE producer contract supports only these four types.
                dtl::unreachable();
        }
        return {.first = first / entry.startOffset, .second = second / entry.startOffset};
    }

    inline SVector coordinate(const Particle& particle, int32_t factor)
    {
        auto axis = [factor](int16_t position, int16_t direction) -> int16_t {
            // Keep vanilla low-word multiplication and rounding toward negative infinity.
            auto product = static_cast<uint32_t>(direction) * factor;
            return position + (static_cast<int32_t>(product) >> 8);
        };
        return {
            .x   = axis(particle.position.x, particle.direction.x),
            .y   = axis(particle.position.y, particle.direction.y),
            .z   = axis(particle.position.z, particle.direction.z),
            .pad = 0,
        };
    }
} // namespace

extern "C"
{
    uint8_t* EFE_initializeParticleEmitters(uint8_t* arena)
    {
        emitters = reinterpret_cast<dtl::array<Emitter, 4>*>(arena);
        for (auto& entry : *emitters)
            entry.instance = nullptr;
        // Preserve the engine's downstream arena addresses, including unused spacing.
        return arena + 9156;
    }

    void EFE_addParticleEmitter()
    {
        auto frames = EFEOperands::pop();
        auto* color = reinterpret_cast<const int32_t*>(EFEOperands::pop());
        auto size   = EFEOperands::pop();
        auto type   = EFEOperands::pop();

        auto available = findFreeEmitter();
        // Full pools still consume all four operands.
        if (!available.has_value()) return;

        auto& entry = **available;
        entry.type  = type;
        for (auto& particle : entry.particles)
            particle.distance = 0;
        entry.instance      = reinterpret_cast<EFEInstance*>(EFE_INSTANCE_BASE);
        entry.frames        = frames;
        entry.color.red     = EFEOperands::readMemory<int32_t>(color);
        entry.color.green   = EFEOperands::readMemory<int32_t>(color + 1);
        entry.color.blue    = EFEOperands::readMemory<int32_t>(color + 2);
        entry.startOffset   = static_cast<uint32_t>(size) * 16;
        entry.startVelocity = static_cast<uint32_t>(size / 8) * 16;
        entry.acceleration  = size / 160 * 16;
    }

    void EFE_tickParticleEmitters()
    {
        for (auto& entry : *emitters) {
            auto* owner = entry.instance;
            if (owner == nullptr) continue;
            if (owner->frame == -1) {
                entry.instance = nullptr;
                continue;
            }

            auto available = findFreeParticle(entry);
            if (available.has_value()) {
                auto& particle    = **available;
                particle.distance = entry.startOffset;
                particle.velocity = entry.startVelocity;
                SVector rotation{
                    .x   = static_cast<int16_t>(((rand() & 127) - 64) * 64),
                    .y   = static_cast<int16_t>(((rand() & 127) - 64) * 64),
                    .z   = static_cast<int16_t>(((rand() & 127) - 64) * 64),
                    .pad = 0,
                };
                particle.direction.y = 0;
                particle.direction.x = 0;
                particle.direction.z = entry.startOffset;
                Matrix matrix;
                libgte_RotMatrixZYX(&rotation, &matrix);
                libgte_ApplyMatrixSV(&matrix, &particle.direction, &particle.direction);
                particle.position.x = owner->position[0];
                particle.position.y = owner->position[1];
                particle.position.z = owner->position[2];
            }

            for (auto& particle : entry.particles) {
                if (particle.distance <= 0) continue;
                particle.distance -= particle.velocity;
                particle.velocity -= entry.acceleration;
            }
            --entry.frames;
            if (entry.frames <= 0) entry.instance = nullptr;
        }
    }

    [[gnu::optimize("Os")]]
    void EFE_renderParticleEmitters()
    {
        for (const auto& entry : *emitters) {
            if (entry.instance == nullptr) continue;
            auto type = entry.type;
            for (const auto& particle : entry.particles) {
                if (particle.distance <= 0) continue;

                auto [first, second] = endpointFactors(entry, particle);
                auto a               = coordinate(particle, first);
                auto b               = coordinate(particle, second);
                auto screen          = getMapPosition(a);
                auto depth           = screen.depth >> 2;
                if (depth < 1000 && (type & 1) == 0) {
                    renderFXParticle(&a, 25, &entry.color);
                    continue;
                }

                ScreenCoord xy{};
                int32_t interpolation{};
                int32_t flag{};
                auto secondDepth = libgte_RotTransPers(&b, &xy.raw, &interpolation, &flag);
                drawLine2P(entry.color.asUint32(),
                           screen.screenX,
                           screen.screenY,
                           xy.x,
                           xy.y,
                           (depth + secondDepth) >> 3,
                           0);
            }
        }
    }
}
