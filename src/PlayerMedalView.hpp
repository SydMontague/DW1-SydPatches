#pragma once
#include "extern/dtl/types.hpp"
#include "extern/dtl/unique_ptr.hpp"

struct MedalView
{
    struct Private;

    MedalView();
    ~MedalView();
    
    void tick();
    void render(int32_t depth);
    bool canBeClosed();

private:
    dtl::unique_ptr<Private> impl;
};