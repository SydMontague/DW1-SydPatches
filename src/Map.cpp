#include "Partner.hpp"
#include "Tamer.hpp"
#include "extern/dw1.hpp"
#include "extern/stddef.hpp"

extern "C"
{
    void loadMapEntities(uint8_t* buffer, uint32_t mapId, uint32_t exitId)
    {
        memcpy(&MAP_WARPS, buffer, sizeof(MapWarps));

        auto posX     = MAP_WARPS.spawnX[exitId];
        auto posY     = MAP_WARPS.spawnY[exitId];
        auto posZ     = MAP_WARPS.spawnZ[exitId];
        auto rotation = MAP_WARPS.rotation[exitId];

        setupTamerOnWarp(posX, posY, posZ, rotation);

        if (rotation <= 0x200 || rotation > 0xE00) posZ += 200;
        if (rotation > 0x200 && rotation <= 0x600) posX += 200;
        if (rotation > 0x600 && rotation <= 0xA00) posZ -= 200;
        if (rotation > 0xA00 && rotation <= 0xE00) posX -= 200;

        setupPartnerOnWarp(posX, posY, posZ, rotation);
        loadMapDigimon(buffer + sizeof(MapWarps), mapId);
        STORED_TAMER_POS = TAMER_ENTITY.posData->location;
    }
}