#include "extern/dw1.hpp"
#include "extern/stddef.hpp"

extern "C"
{
    void setupTamerOnWarp(int32_t posX, int32_t posY, int32_t posZ, int32_t rotation);
    void Tamer_setState(uint8_t state);
    void setIsStandingOnDrop(bool value);
    void Tamer_setSubState(uint8_t state);
    void Tamer_setFullState(uint8_t state, uint8_t subState);
    uint8_t Tamer_getState();
    uint8_t Tamer_getSubState();
    void addTamerLevel(int32_t chance, int32_t amount);
    void removeTamerItem();
    void initializeTamerWaypoints();
    void checkMapInteraction();
    void startFeedingItem(ItemType item);
    bool isTamerOnScreen();
    void initializeTamer(DigimonType type,
                         int32_t posX,
                         int32_t posY,
                         int32_t posZ,
                         int32_t rotX,
                         int32_t rotY,
                         int32_t rotZ);
}