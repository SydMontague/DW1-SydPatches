#include "Sound.hpp"

#include "Files.hpp"
#include "extern/dw1.hpp"
#include "extern/libetc.hpp"
#include "extern/libsnd.hpp"
#include "extern/libspu.hpp"
#include "extern/stddef.hpp"

struct SoundBufferData
{
    uint8_t* buffer;
    uint16_t vabId;
    uint8_t finishedLoading;
    uint8_t unk;
};

static dtl::array<uint8_t, SS_SEQ_TABSIZ> SEQ_TABLE;
static int16_t CURRENT_SEQ_TRACK;
static int16_t CURRENT_SEQ_FONT;
static int16_t SEQ_ACCESS_NUM;
static uint32_t freeChannelIndex;
static dtl::array<SoundBufferData, 10> SOUND_BUFFERS;
static uint8_t LOAD_SOUND_COMPLETE_STATE;

static uint32_t uploadSoundStart(uint8_t* buffer, int32_t param)
{
    auto size1 = reinterpret_cast<uint32_t*>(buffer)[0];
    auto size2 = reinterpret_cast<uint32_t*>(buffer)[1];
    memcpy(VHB_HEADER_ADDR[param], buffer + (size1 & 0xFFFFFFFC), size2 - size1);
    libsnd_SsVabClose(param);
    auto result = libsnd_SsVabOpenHeadSticky(VHB_HEADER_ADDR[param], param, VHB_SOUNDBUFFER_START[param]);
    if (result < 0) return false;

    auto result2 = libsnd_SsVabTransBody(buffer + size2, param);
    if (result2 < 0) return false;

    return true;
}

static int32_t readVHBFile(int32_t vabId, const char* filename, uint8_t* buffer)
{
    readFile(filename, buffer);
    auto result = uploadSoundStart(buffer, vabId);
    if (!result) return -1;

    libsnd_SsVabTransCompleted(1);
    libsnd_SsUtGetVBaddrInSB(result);
    return result;
}

static void seqStop()
{
    if (SEQ_ACCESS_NUM == -1) return;

    libsnd_SsSeqStop(SEQ_ACCESS_NUM);
}

static void seqClose()
{
    if (SEQ_ACCESS_NUM == -1) return;

    libsnd_SsSeqClose(SEQ_ACCESS_NUM);
    SEQ_ACCESS_NUM = -1;
}

static int32_t
readVHBFileSectors(int32_t vabId, const char* filename, uint8_t* buffer, uint32_t offset, int32_t sectors)
{
    readFileSectors(filename, buffer, offset, sectors);

    auto result = uploadSoundStart(buffer, vabId);
    if (!result) return -1;

    libsnd_SsVabTransCompleted(1);
    libsnd_SsUtGetVBaddrInSB(result);
    return result;
}

static void seqOpen()
{
    auto trackCount = *reinterpret_cast<uint32_t*>(SEQ_BUFFER.data()) / 4;

    if (trackCount <= CURRENT_SEQ_TRACK) CURRENT_SEQ_TRACK = 0;

    auto trackOffset = *reinterpret_cast<uint32_t*>(SEQ_BUFFER.data() + CURRENT_SEQ_TRACK * 4);
    SEQ_ACCESS_NUM   = libsnd_SsSeqOpen(SEQ_BUFFER.data() + trackOffset, 2);
}

static void seqPlay()
{
    if (SEQ_ACCESS_NUM == -1) return;

    libsnd_SsSeqSetVol(SEQ_ACCESS_NUM, 80, 80);

    if ((CURRENT_SEQ_FONT == 0x1d && CURRENT_SEQ_TRACK != 1) || (CURRENT_SEQ_FONT == 0x1e && CURRENT_SEQ_TRACK != 2) ||
        (CURRENT_SEQ_FONT == 0x1f && CURRENT_SEQ_TRACK != 2) || (CURRENT_SEQ_FONT == 0x20 && CURRENT_SEQ_TRACK != 2))
        libsnd_SsSeqPlay(SEQ_ACCESS_NUM, 1, 1);
    else
        libsnd_SsSeqPlay(SEQ_ACCESS_NUM, 1, 0);
}

static bool loadMusicFont(int32_t font)
{
    auto result = readVHBFileSectors(2, "SOUND\\VHB\\FAALL.VHB", GENERAL_BUFFER.data(), (font - 1) * 0x27, 0x27);
    if (result == -1) return false;

    auto start = *reinterpret_cast<uint32_t*>(GENERAL_BUFFER.data() + 8);
    auto end   = *reinterpret_cast<uint32_t*>(GENERAL_BUFFER.data() + 12);

    memcpy(SEQ_BUFFER.data(), GENERAL_BUFFER.data() + (start & 0xFFFFFFFC), end - start);
    return true;
}

static inline uint32_t getFreeChannelIndex()
{
    for (int32_t i = 0; i < 14; i++)
    {
        auto val = ((freeChannelIndex + i) % 14) + 10;
        if (libspu_SpuGetKeyStatus(1 << (val & 0x1F)) != 1) return val;
    }

    return freeChannelIndex + 10;
}

static uint32_t getNextFreeChannel()
{
    auto val         = getFreeChannelIndex();
    freeChannelIndex = (val - 9) % 14;
    return val;
}

static uint32_t startSound(int32_t vabId, int32_t prog, uint32_t note)
{
    auto header = VHB_HEADER_ADDR[vabId];
    auto mask   = 0;

    for (int32_t i = 0; i < 16; i++)
    {
        if (note != header[i * 0x20 + prog * 0x200 + 0x827]) continue;

        auto voice = getNextFreeChannel();
        if (voice == 0xFFFFFFFF) return mask;

        libsnd_SsUtKeyOnV(voice, vabId, prog, i, note, 0, 0x7F, 0x7F);
        mask |= (1 << (voice & 0x1F));
    }

    return mask;
}

static int32_t getSoundBank(DigimonType type)
{
    if (type < DigimonType::BOTAMON || type > DigimonType::METALETEMON) return 15;

    return DIGIMON_VLALL_SOUND_ID[static_cast<size_t>(type)];
}

static bool loadSoundCompleteCallback(void* para)
{
    auto param = reinterpret_cast<uint32_t>(para);

    if (LOAD_SOUND_COMPLETE_STATE == 0)
    {
        auto result = uploadSoundStart(SOUND_BUFFERS[param].buffer, param);
        if (!result)
        {
            SOUND_BUFFERS[param].vabId = 0xFFFF;
            return false;
        }

        SOUND_BUFFERS[param].vabId = param;
        LOAD_SOUND_COMPLETE_STATE  = 4;
        return true;
    }

    if (LOAD_SOUND_COMPLETE_STATE == 4)
    {
        if (libsnd_SsVabTransCompleted(0) != 0) LOAD_SOUND_COMPLETE_STATE = 5;
        return true;
    }

    if (LOAD_SOUND_COMPLETE_STATE == 5)
    {
        libsnd_SsUtGetVBaddrInSB(SOUND_BUFFERS[param].vabId);
        return false;
    }

    return true;
}

static bool loadSoundFinishCallback(void* param)
{
    LOAD_SOUND_COMPLETE_STATE = 0;
    setFileReadCallback2(loadSoundCompleteCallback, param);
    return true;
}

static void uploadSoundBuffer(uint32_t param)
{
    auto result = uploadSoundStart(SOUND_BUFFERS[param].buffer, param);
    if (!result)
    {
        SOUND_BUFFERS[param].vabId = 0xFFFF;
        return;
    }

    SOUND_BUFFERS[param].vabId = param;
    libsnd_SsVabTransCompleted(1);
    libsnd_SsUtGetVBaddrInSB(SOUND_BUFFERS[param].vabId);
}

extern "C"
{
    bool initializeMusic()
    {
        libetc_ResetCallback();
        libsnd_SsInit();
        libsnd_SsSetTableSize(SEQ_TABLE.data(), 1, 1);
        libsnd_SsSetTicKMode(2); // SS_TICK240

        if (readVHBFile(0, "SOUND\\VHB\\SS.VHB", GENERAL_BUFFER.data()) == -1) return false;
        if (readVHBFile(1, "SOUND\\VHB\\SL.VHB", GENERAL_BUFFER.data()) == -1) return false;

        libsnd_SsStart();
        libsnd_SsSetMVol(127, 127);
        SEQ_ACCESS_NUM     = -1;
        CURRENT_SEQ_FONT   = -1;
        SpuReverbAttr attr = {.mask = 7, .mode = 259, .depth = {28672, 28672}, .feedback = 0};

        libspu_SpuSetReverbModeParam(&attr);
        libspu_SpuClearReverbWorkArea(3);
        libspu_SpuSetReverbDepth(&attr);
        libspu_SpuSetReverbVoice(1, 0x7fffff);
        libspu_SpuSetReverb(1);
        return true;
    }

    void finalizeMusic()
    {
        seqStop();
        seqClose();

        for (int32_t i = 0; i < 10; i++)
            libsnd_SsVabClose(i);

        libsnd_SsEnd();
        libsnd_SsQuit();
    }

    void stopSoundMask(uint32_t mask)
    {
        for (int32_t i = 0; i < 0x18; i++)
        {
            if ((mask & (1 << i)) == 0) continue;

            libsnd_SsUtAutoVol(i, 0x7f, 0, 6);
            libsnd_SsUtKeyOffV(i);
        }
    }

    uint32_t playSound(int32_t vabId, uint32_t val)
    {
        if (vabId == 0 || vabId == 1 || vabId == 8)
            return startSound(vabId, val / 16, static_cast<uint8_t>((val % 16) + 60));
        else if (vabId > 2 && vabId < 8)
            return startSound(vabId, 0, val);

        return 0;
    }

    void stopSound()
    {
        libsnd_SsUtAllKeyOff(0);

        // TODO why 10?
        for (int32_t i = 10; i < 0x18; i++)
        {
            if (libspu_SpuGetKeyStatus(1 << i) == 1)
            {
                libsnd_SsUtAutoVol(i, 0x7f, 0, 6);
                libsnd_SsUtKeyOffV(i);
            }
        }
    }

    uint32_t playSound2(int32_t vabId, uint32_t value)
    {
        return startSound(vabId, value / 16, static_cast<uint8_t>((value % 16) + 60));
    }

    void loadDigimonSounds(int32_t vabId, DigimonType type)
    {
        if (vabId < 4 || vabId > 7) return;

        readVHBFileSectors(vabId,
                           "SOUND\\VHB\\VBALL.VHB",
                           GENERAL_BUFFER.data(),
                           DIGIMON_VBALL_SOUND_ID[static_cast<size_t>(type)] * 7,
                           7);
    }

    void loadPartnerSounds(DigimonType type)
    {
        auto result =
            readVHBFileSectors(3, "SOUND\\VHB\\VLALL.VHB", GENERAL_BUFFER.data(), getSoundBank(type) * 15, 15);
        if (result == -1) return;

        loadDigimonSounds(4, type);
    }

    bool VS_loadSounds()
    {
        ACTIVE_MAP_SOUND_ID = -1;
        return readVHBFile(8, "SOUND\\VHB\\SB.VHB", GENERAL_BUFFER.data()) != -1;
    }

    bool loadMapSounds(int32_t mapSoundId)
    {
        if (ACTIVE_MAP_SOUND_ID == mapSoundId) return true;

        ACTIVE_MAP_SOUND_ID = mapSoundId;
        auto& data          = MAP_SOUND_PARA[mapSoundId];

        auto result = readVHBFileSectors(8,
                                         "SOUND\\VHB\\ESALL.VHB",
                                         GENERAL_BUFFER.data(),
                                         data.sectorId / 2,
                                         data.sectorCount / 2);
        return result != -1;
    }

    bool playMusic(int32_t font, int32_t track)
    {
        if (font < 1 || font > 0x21) return false;

        if (font == CURRENT_SEQ_FONT && track == CURRENT_SEQ_TRACK) return true;

        seqStop();
        seqClose();

        if (CURRENT_SEQ_FONT != font)
        {
            loadMusicFont(font);
            CURRENT_SEQ_FONT = font;
        }
        CURRENT_SEQ_TRACK = track;
        seqOpen();
        seqPlay();
        return true;
    }

    void stopBGM()
    {
        seqStop();
        seqClose();
        CURRENT_SEQ_TRACK = -1;
    }

    int32_t isSoundBufferLoadingDone(int32_t id)
    {
        return SOUND_BUFFERS[id].finishedLoading;
    }

    int32_t readVBALLSection(int32_t id, int32_t soundId)
    {
        if (id < 4 || id > 7) SOUND_BUFFERS[id].vabId = 0xFFFF;

        SOUND_BUFFERS[id].buffer = GENERAL_BUFFER.data();
        addFileReadRequestSection("SOUND\\VHB\\VBALL.VHB",
                                  GENERAL_BUFFER.data(),
                                  DIGIMON_VBALL_SOUND_ID[soundId] * 7,
                                  7,
                                  &SOUND_BUFFERS[id].finishedLoading,
                                  loadSoundFinishCallback,
                                  reinterpret_cast<void*>(id));
        return id;
    }

    int32_t loadMapSounds2(int32_t mapSoundId)
    {
        if (ACTIVE_MAP_SOUND_ID == mapSoundId) return 1;

        ACTIVE_MAP_SOUND_ID     = mapSoundId;
        auto& para              = MAP_SOUND_PARA;
        SOUND_BUFFERS[8].buffer = GENERAL_BUFFER.data();
        addFileReadRequestSection("SOUND\\VHB\\VBALL.VHB",
                                  GENERAL_BUFFER.data(),
                                  para[mapSoundId].sectorId / 2,
                                  para[mapSoundId].sectorCount / 2,
                                  &SOUND_BUFFERS[8].finishedLoading,
                                  loadSoundFinishCallback,
                                  reinterpret_cast<void*>(8));
        return 8;
    }

    int32_t isSoundLoaded(bool isAsync, int32_t soundId)
    {
        if (isAsync)
        {
            while (SOUND_BUFFERS[soundId].finishedLoading != 0)
                tickFileReadQueue();
        }
        else if (SOUND_BUFFERS[soundId].finishedLoading != 0)
            return -1;

        return SOUND_BUFFERS[soundId].vabId != 0xFFFF;
    }

    int32_t loadVLALL(int16_t type, uint8_t* buffer)
    {
        readVBALLSection(4, type);
        auto soundId = (type < 1 || type > 65) ? 15 : DIGIMON_VLALL_SOUND_ID[type];

        SOUND_BUFFERS[3].buffer = buffer;
        addFileReadRequestSection("SOUND\\VHB\\VLALL.VHB",
                                  buffer,
                                  soundId * 15,
                                  15,
                                  &SOUND_BUFFERS[3].finishedLoading,
                                  nullptr,
                                  nullptr);
        return 3;
    }

    void waitForSoundBufferLoading(int32_t param)
    {
        while (SOUND_BUFFERS[param].finishedLoading != 0)
            tickFileReadQueue();
        uploadSoundBuffer(param);
    }

    int32_t loadSB()
    {
        ACTIVE_MAP_SOUND_ID     = -1;
        SOUND_BUFFERS[8].buffer = GENERAL_BUFFER.data();
        addFileReadRequestLookup("SOUND\\VHB\\SB.VHB",
                                 GENERAL_BUFFER.data(),
                                 &SOUND_BUFFERS[8].finishedLoading,
                                 loadSoundFinishCallback,
                                 reinterpret_cast<void*>(8));
        return 8;
    }
}