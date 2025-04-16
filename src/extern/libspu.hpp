#include "stddef.hpp"

extern "C"
{
    struct SpuVolume
    {
        int16_t left;
        int16_t right;
    };

    struct SpuReverbAttr
    {
        uint32_t mask;
        int32_t mode;
        SpuVolume depth;
        int32_t delay;
        int32_t feedback;
    };

    int32_t libspu_SpuSetReverbModeParam(SpuReverbAttr* attr);
    int32_t libspu_SpuClearReverbWorkArea(int32_t reverbMode);
    int32_t libspu_SpuSetReverbDepth(SpuReverbAttr* attr);
    uint32_t libspu_SpuSetReverbVoice(int32_t on_off, uint32_t voice_bit);
    int32_t libspu_SpuSetReverb(int32_t on_off);
    int32_t libspu_SpuGetKeyStatus(uint32_t voice_bit);
}