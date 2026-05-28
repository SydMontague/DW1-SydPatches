#pragma once

#include "extern/stddef.hpp"

void addDigimonMenu();
void removeDigimonMenu();
bool isDigimonMenuActive();
bool isDigimonMenuOpened();
void tickDigimonMenu();
void renderDigimonMenu(int32_t layer);

// Accessors for sub-views (TechView's expand/collapse) that need to mutate
// the menu's outer box height at runtime.
int16_t digimonMenuBoxHeight();
void    setDigimonMenuBoxHeight(int16_t height);