#pragma once

#include "AtlasFont.hpp"
#include "extern/dtl/array.hpp"
#include "extern/dtl/unique_ptr.hpp"

struct Private;

struct TechView
{
    TechView();
    ~TechView();
    void tick();
    void render(int32_t depth);

    bool canBeClosed();

private:
    dtl::unique_ptr<Private> pimpl;
};