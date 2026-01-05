#include "Files.hpp"
#include "GameObjects.hpp"
#include "GameTime.hpp"
#include "Helper.hpp"
#include "Map.hpp"
#include "Math.hpp"
#include "Partner.hpp"
#include "Sound.hpp"
#include "Tamer.hpp"
#include "extern/STD.hpp"
#include "extern/dw1.hpp"

namespace
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
    TournamentConfig getTournamentConfig()
    {
        dtl::array<uint8_t, 0x70> array;

        auto tournamentId = readPStat(3);
        auto* scriptPtr   = getScriptJumpTableEntry(10, tournamentId);
        auto* scriptEntry = readScriptJumpTableEntry(scriptPtr, 4);
        auto* entryPtr    = reinterpret_cast<uint8_t*>(scriptEntry + 2);
        int32_t count     = 0;
        TournamentConfig config;

        config.id = tournamentId;

        // TODO refactor this shit, maybe get the data from the script
        // fill viable Digimon array
        if (*entryPtr >= 0xFE)
        {
            for (int32_t i = 0; i < 0x70; i++)
            {
                if (isTriggerSet(200 + i)) continue;
                array[count++] = i;
            }
        }
        else
        {
            while (*entryPtr < 0xFE)
            {
                auto type = *entryPtr;
                entryPtr++;
                if (tournamentId != 22 && !isTriggerSet(200 + type)) continue;
                array[count++] = type;
            }
        }

        // pick participants
        if (count <= 7)
        {
            for (int32_t i = 0; i < count; i++)
                config.participants[i] = array[i];

            for (int32_t i = count; i < 7; i++)
                config.participants[i] = array[random(count)];
        }
        else
        {
            // vanilla messes with the 'count' value here, creating stupid results
            for (int32_t i = 0; i < 7; i++)
            {
                auto choice = random(count);
                auto type   = array[choice];
                if (type == 0xFF || type == static_cast<uint8_t>(PARTNER_ENTITY.type))
                {
                    i--;
                    continue;
                }
                config.participants[i] = array[choice];
                array[choice]          = 0xFF;
            }
        }

        return config;
    }

} // namespace

extern "C"
{
    void startTournament()
    {
        auto config = getTournamentConfig();
        stopBGM();
        loadDynamicLibrary(Overlay::STD_REL, nullptr, false, nullptr, nullptr);
        auto wins = STD_tournamentMain(&config);
        reinitializeAfterTournament();
        unsetTrigger(0x25);

        auto result2 = readPStat(3) == 5 ? 2 : 3;

        if (result2 == wins)
            TOURNAMENTS_WON = enforceStatsLimits(ScriptStats::TOURNAMENTS_WON, TOURNAMENTS_WON + 1);
        else
            TOURNAMENTS_LOST = enforceStatsLimits(ScriptStats::TOURNAMENTS_LOST, TOURNAMENTS_LOST + 1);

        TOURNAMENT_WINS = enforceStatsLimits(ScriptStats::TOURNAMENT_WINS, TOURNAMENT_WINS + wins);
        writePStat(255, wins);
    }
}