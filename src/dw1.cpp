#include "extern/dw1.hpp"

int16_t StatsGains::get(Stat stat)
{
    switch (stat)
    {
        case Stat::HP: return hp;
        case Stat::MP: return mp;
        case Stat::OFFENSE: return offense;
        case Stat::DEFENSE: return defense;
        case Stat::SPEED: return speed;
        case Stat::BRAINS: return brains;
        default: return 0;
    }
}

void StatsGains::set(Stat stat, int16_t value)
{
    switch (stat)
    {
        case Stat::HP: hp = value; break;
        case Stat::MP: mp = value; break;
        case Stat::OFFENSE: offense = value; break;
        case Stat::DEFENSE: defense = value; break;
        case Stat::SPEED: speed = value; break;
        case Stat::BRAINS: brains = value; break;
        default: return;
    }
}

bool StatsGains::isAllZero()
{
    return hp == 0 && mp == 0 && offense == 0 && defense == 0 && speed == 0 && brains == 0;
}