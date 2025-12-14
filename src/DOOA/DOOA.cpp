#include "../Camera.hpp"
#include "../Partner.hpp"
#include "../Tamer.hpp"
#include "../extern/dw1.hpp"

namespace DOOA
{
    extern "C"
    {
        static int16_t storedDigimonY;

        void DOOA_storeDigimonY()
        {
            storedDigimonY = PARTNER_ENTITY.posData->location.y;
        }

        int32_t DOOA_getStoredDigimonY()
        {
            return storedDigimonY;
        }

        void DOOA_setSomeDyingState()
        {
            Tamer_setState(6);
            Partner_setFullState(8, 2);
            unsetCameraFollowPlayer();
        }
    }
} // namespace DOOA