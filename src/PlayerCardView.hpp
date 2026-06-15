#pragma once

#include "AtlasFont.hpp"
#include "UIBox.hpp"
#include "UIElements.hpp"

struct CardView
{
    struct Private;

    CardView();
    ~CardView();
    void tick();
    void render(int32_t depth);

    bool canBeClosed();

private:
    dtl::unique_ptr<Private> impl;
};
