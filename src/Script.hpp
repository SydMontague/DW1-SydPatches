#pragma once

#include "extern/dtl/types.hpp"

extern "C"
{
    bool isPartnerBaby();
    void dailyPStatTrigger();
    void showMapheadTextbox(int32_t line, int32_t speaker, int32_t textboxId, int32_t sectionId);
}
