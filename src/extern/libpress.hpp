#pragma once

#include "dtl/types.hpp"

extern "C"
{
    void libpress_DecDCTReset(int32_t mode);
    void libpress_DecDCTin(uint32_t* buffer, int32_t mode);
    void libpress_DecDCTout(uint32_t* buffer, int32_t words);
    int32_t libpress_DecDCToutCallback(void (*callback)());
    int32_t libpress_DecDCTvlc2(uint32_t* bitstream, uint32_t* output, uint16_t* table);
    void libpress_DecDCTvlcBuild(uint16_t* table);
}
