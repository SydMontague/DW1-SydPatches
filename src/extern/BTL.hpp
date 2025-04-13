#include "dw1.hpp"

extern "C"
{
    BattleResult BTL_battleMain();
    void BTL_healStatusEffect(bool isVarious);
    void BTL_removeDeathCountdown();
}