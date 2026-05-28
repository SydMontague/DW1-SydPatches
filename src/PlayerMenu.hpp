#pragma once
#include "extern/stddef.hpp"

void addPlayerMenu();
void removePlayerMenu();
bool isPlayerMenuActive();
bool isPlayerMenuOpened();
void tickPlayerMenu();
void renderPlayerMenu(int32_t layer);