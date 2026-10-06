#include "Curling.hpp"
#include "CurlingRenderTypes.hpp"

#include "../Helper.hpp"
#include "../UIElements.hpp"
#include "../extern/dw1.hpp"
#include "../extern/libgpu.hpp"
#include "../extern/libgs.hpp"

void KAR_renderSprite(const CurlingSprite* sprite)
{
    auto* primitive   = static_cast<POLY_FT4*>(libgs_GsGetWorkBase());
    primitive->tag[3] = 9;
    primitive->code   = 0x2C;
    primitive->tpage  = 10; // getTPage(0, 0, 640, 0)
    primitive->r0     = sprite->red;
    primitive->g0     = sprite->green;
    primitive->b0     = sprite->blue;
    primitive->clut   = getClut(sprite->clutX, sprite->clutY);
    setUVDataPolyFT4(primitive, sprite->u, sprite->v, sprite->width, sprite->height);
    setPosDataPolyFT4(primitive, sprite->x, sprite->y, sprite->width, sprite->height);
    libgpu_AddPrim(ACTIVE_ORDERING_TABLE->origin + 10, primitive);
    libgs_GsSetWorkBase(primitive + 1);
}
