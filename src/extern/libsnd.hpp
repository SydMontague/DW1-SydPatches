#pragma once
#include "stddef.hpp"

constexpr auto SS_SEQ_TABSIZ = 176;

extern "C"
{
    void libsnd_SsVabClose(uint16_t vabId);
    int16_t libsnd_SsVabOpenHeadSticky(uint8_t* addr, int16_t vabId, uint32_t sbAddr);
    int16_t libsnd_SsVabTransBody(uint8_t* addr, int16_t vabId);
    int16_t libsnd_SsVabTransCompleted(int16_t immediateFlag);
    void libsnd_SsSeqStop(int16_t seq_access_num);
    void libsnd_SsSeqClose(int16_t seq_access_num);
    int16_t libsnd_SsSeqOpen(uint8_t* addr, int16_t vabId);
    void libsnd_SsSeqSetVol(int16_t seq_access_num, int16_t left, int16_t right);
    void libsnd_SsSeqPlay(int16_t seq_access_num, int8_t play_mode, int16_t l_count);
    int32_t libsnd_SsUtGetVBaddrInSB(int16_t vabId);
    int16_t libsnd_SsUtAutoVol(int16_t voice, int16_t start, int16_t end, int16_t time);
    int16_t libsnd_SsUtKeyOffV(int16_t voice);
    void libsnd_SsUtAllKeyOff(int16_t mode);
    int16_t libsnd_SsUtKeyOnV(int16_t voice,
                              int16_t vabId,
                              int16_t prog,
                              int16_t tone,
                              int16_t note,
                              int16_t fine,
                              int16_t volLeft,
                              int16_t volRight);
    void libsnd_SsInit();
    void libsnd_SsSetTableSize(uint8_t* table, int16_t s_max, int16_t t_max);
    void libsnd_SsSetTicKMode(int32_t tick_mode);
    void libsnd_SsStart();
    void libsnd_SsSetMVol(int16_t left, int16_t right);
    void libsnd_SsEnd();
    void libsnd_SsQuit();
}