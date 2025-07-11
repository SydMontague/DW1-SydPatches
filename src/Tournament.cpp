#include "GameObjects.hpp"
#include "GameTime.hpp"
#include "Helper.hpp"
#include "Map.hpp"
#include "Partner.hpp"
#include "Tamer.hpp"
#include "extern/dw1.hpp"

extern "C"
{
    void reinitializeAfterTournament()
    {
        addObject(ObjectID::GAME_CLOCK, 0, tickGameClock, renderGameClock);
        addObject(ObjectID::STATUS_BAR, 0, nullptr, renderStatusBars);
        addObject(ObjectID::MAP, 0, nullptr, renderMap);
        addObject(ObjectID::POOP, 0, nullptr, renderPoop);
        Tamer_setState(0);
        Partner_setState(1);
    }
}