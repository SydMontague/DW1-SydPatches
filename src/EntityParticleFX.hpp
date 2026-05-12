
#include "extern/dtl/types.hpp"
#include "extern/dw1.hpp"

void initializeEntityParticleFX();
extern "C"
{
    void addEntityParticleFX(Entity* entity, int32_t counter);
}