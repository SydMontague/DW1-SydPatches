#include "../GameObjects.hpp"
#include "../Helper.hpp"
#include "../Math.hpp"
#include "Operands.hpp"

extern "C" int16_t EFE_ENTRY(calculateAttackHitPosition)(SVector* out, Entity* victim, Entity* source, int32_t radius);

namespace
{
    const auto& instancePosition()
    {
        return reinterpret_cast<const EFEInstance*>(EFE_INSTANCE_BASE)->position;
    }

    constexpr uint32_t LEFT  = 1;
    constexpr uint32_t RIGHT = 2;
    constexpr uint32_t BELOW = 4;
    constexpr uint32_t ABOVE = 8;

    constexpr uint32_t getOutsideSides(Position point, Position min, Position max)
    {
        uint32_t sides = 0;
        if (point.x > max.x)
            sides |= RIGHT;
        else if (point.x < min.x)
            sides |= LEFT;
        if (point.y > max.y)
            sides |= ABOVE;
        else if (point.y < min.y)
            sides |= BELOW;
        return sides;
    }

    constexpr bool lineIntersectsBounds(Position start, Position end, Position min, Position max)
    {
        const auto first = getOutsideSides(start, min, max);
        const auto last  = getOutsideSides(end, min, max);
        if (first == 0 || last == 0) return true;
        if (first & last) return false;

        // Vanilla accepts endpoints in opposite regions, including opposite corners.
        const auto sides = first | last;
        if (sides == (LEFT | RIGHT) || sides == (BELOW | ABOVE) || sides == (LEFT | RIGHT | BELOW | ABOVE)) return true;

        const uint32_t dx      = end.x - start.x;
        const uint32_t dy      = end.y - start.y;
        const auto side        = [&](int32_t x, int32_t y) { return dx * (y - start.y) - dy * (x - start.x); };
        const auto firstCorner = side(min.x, min.y);
        // Vanilla tests the signed low word of each product, including when it wraps.
        return static_cast<int32_t>(firstCorner * side(max.x, min.y)) < 0 ||
               static_cast<int32_t>(firstCorner * side(min.x, max.y)) < 0 ||
               static_cast<int32_t>(firstCorner * side(max.x, max.y)) < 0;
    }

    DigimonEntity* getAttackCandidate(int32_t index)
    {
        auto* entity = static_cast<DigimonEntity*>(ENTITY_TABLE.table[index]);
        if (entity == nullptr) return nullptr;
        if (entity == EFE_CURRENT_SUBEFFECT->sourceEntity) return nullptr;
        if (entity->stats.isHit) return nullptr;
        if (!entity->isOnMap) return nullptr;
        return entity;
    }

    void queueHit(int32_t victimIndex, SVector& position, int32_t subEffectIndex)
    {
        const auto casterIndex =
            ENTITY_TABLE.getEntityIndex(EFE_CURRENT_SUBEFFECT->sourceEntity).value_or(ENTITY_TABLE.table.size());
        addAttackObject(victimIndex, 1, &position, EFE_SLOT_INDEX, subEffectIndex, casterIndex);
    }

    void applyBoxHit(int32_t subEffectIndex)
    {
        auto* hit     = EFEOperands::popPointer<int32_t>();
        auto* extents = EFEOperands::popPointer<const int32_t>();
        *hit          = 0;
        if (subEffectIndex < 0) subEffectIndex = EFE_SUBEFFECT_INDEX;
        auto center = EFEHelpers::toShortVector(instancePosition().data());
        AABB box{.center = &center, .extent = EFEHelpers::toShortVector(extents)};
        for (EFE_HIT_ENTITY_INDEX = 1;; ++EFE_HIT_ENTITY_INDEX) {
            EFE_HIT_ENTITY_INDEX = findAABBHitEntity(&box, EFE_CURRENT_SUBEFFECT->sourceEntity, EFE_HIT_ENTITY_INDEX);
            if (EFE_HIT_ENTITY_INDEX == -1) return;
            auto* victim = static_cast<DigimonEntity*>(ENTITY_TABLE.table[EFE_HIT_ENTITY_INDEX]);
            if (victim->stats.isHit) continue;
            victim->stats.isHit = true;
            queueHit(EFE_HIT_ENTITY_INDEX, center, subEffectIndex);
            *hit = 1;
            return;
        }
    }

    void applyPositionedHit(DigimonEntity* victim, int32_t radius, int32_t height)
    {
        victim->stats.isHit = true;
        SVector position{};
        EFE_ENTRY(calculateAttackHitPosition)(&position, victim, EFE_CURRENT_SUBEFFECT->sourceEntity, radius);
        position.y = height;
        queueHit(EFE_HIT_ENTITY_INDEX, position, EFE_SUBEFFECT_INDEX);
    }

} // namespace

extern "C"
{
    void EFE_ENTRY(applyBoxAttackHit)()
    {
        applyBoxHit(EFEOperands::pop());
    }

    void EFE_ENTRY(applyDefaultBoxAttackHit)()
    {
        applyBoxHit(-1);
    }

    void EFE_ENTRY(addAttackObjectToTarget)()
    {
        auto* target = static_cast<DigimonEntity*>(EFE_CURRENT_SUBEFFECT->targetEntity);
        if (target->stats.isHit) return;
        auto position          = EFEHelpers::toShortVector(instancePosition().data());
        target->stats.isHit    = true;
        const auto targetIndex = ENTITY_TABLE.getEntityIndex(target).value_or(ENTITY_TABLE.table.size());
        queueHit(targetIndex, position, EFE_SUBEFFECT_INDEX);
    }

    void EFE_ENTRY(applyRadiusAttackHit)()
    {
        auto* hit            = EFEOperands::popPointer<int32_t>();
        const auto radius    = EFEOperands::pop();
        *hit                 = 0;
        const auto& position = instancePosition();
        for (EFE_HIT_ENTITY_INDEX = 1; EFE_HIT_ENTITY_INDEX < static_cast<int32_t>(ENTITY_TABLE.table.size());
             ++EFE_HIT_ENTITY_INDEX) {
            auto* victim = getAttackCandidate(EFE_HIT_ENTITY_INDEX);
            if (victim == nullptr) continue;
            const auto& location = victim->posData->location;
            const auto* species  = getDigimonData(victim->type);
            const auto dx        = static_cast<uint32_t>(position[0]) - location.x;
            const auto dz        = static_cast<uint32_t>(position[2]) - location.z;
            if (EFEHelpers::sumOfSquares(dx, dz) > EFEHelpers::sumOfSquares(species->radius, radius)) continue;
            applyPositionedHit(victim, species->radius, -species->height / 2);
            *hit = 1;
        }
    }

    [[gnu::optimize("Os")]]
    void EFE_ENTRY(applyLineAttackHit)()
    {
        EFEOperands::pop();
        auto* hit            = EFEOperands::popPointer<int32_t>();
        auto* extents        = EFEOperands::popPointer<const int32_t>();
        *hit                 = 0;
        const auto& position = instancePosition();
        const auto& source   = EFE_CURRENT_SUBEFFECT->sourceEntity->posData->location;
        const Position start{static_cast<int16_t>(position[0]), static_cast<int16_t>(position[2])};
        const Position end{static_cast<int16_t>(source.x), static_cast<int16_t>(source.z)};
        for (EFE_HIT_ENTITY_INDEX = 1; EFE_HIT_ENTITY_INDEX < static_cast<int32_t>(ENTITY_TABLE.table.size());
             ++EFE_HIT_ENTITY_INDEX) {
            auto* victim = getAttackCandidate(EFE_HIT_ENTITY_INDEX);
            if (victim == nullptr) continue;
            const auto& location = victim->posData->location;
            const auto* species  = getDigimonData(victim->type);
            const auto radius    = species->radius;
            const Position min{static_cast<int16_t>(location.x - radius), static_cast<int16_t>(location.z - radius)};
            const Position max{static_cast<int16_t>(location.x + radius), static_cast<int16_t>(location.z + radius)};
            if (!lineIntersectsBounds(start, end, min, max) || position[1] < -species->height - extents[1]) continue;
            applyPositionedHit(victim, radius, position[1]);
            *hit = 1;
            return;
        }
    }
}
