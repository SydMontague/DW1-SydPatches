#include "../extern/dw1.hpp"

namespace DOOA
{
    extern "C"
    {
        static int16_t storedDigimonY;

        void DOOA_storeDigimonY() {
          storedDigimonY = PARTNER_ENTITY.posData->location.y;
        }

        int32_t DOOA_getStoredDigimonY() {
          return storedDigimonY;
        }
    }
} // namespace DOOA