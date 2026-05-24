
#include "AtlasFont.hpp"

struct PlayerInfoView
{
    // NOLINTNEXTLINE
    __attribute__((optimize("Os"))) PlayerInfoView();

    void tick();
    void render(int32_t);

private:
    dtl::array<AtlasString, 18> strings;
};
