#pragma once
#include "extern/dtl/unique_ptr.hpp"

struct ChartView
{
    struct Private;

    ChartView();
    ~ChartView();
    void tick();
    void render(int32_t depth);

    bool canBeClosed();

private:
    dtl::unique_ptr<Private> impl;
};