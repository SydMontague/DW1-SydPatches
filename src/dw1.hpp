#pragma once
extern "C" {

using uint8_t = unsigned char;
using uint16_t = unsigned short;
using uint32_t = unsigned int;
using int8_t = char;
using int16_t = short;
using int32_t = int;

static_assert(sizeof(int32_t) == 4);
static_assert(sizeof(int16_t) == 2);
static_assert(sizeof(int8_t) == 1);
static_assert(sizeof(uint32_t) == 4);
static_assert(sizeof(uint16_t) == 2);
static_assert(sizeof(uint8_t) == 1);

struct GlyphData {
  uint16_t pixelData[11];
  uint8_t width;
  uint8_t padding;
};

struct RECT {
  int16_t x;
  int16_t y;
  int16_t width;
  int16_t height;
};

struct Position {
  int32_t x;
  int32_t y;
  int32_t z;
  int32_t pad;
};

enum class Type : uint8_t {
  UNDEFINED = 0,
  DATA = 1,
  VACCINE = 2,
  VIRUS = 3,
};

enum class Level : uint8_t {
  UNDEFINED = 0,
  FRESH = 1,
  IN_TRAINING = 2,
  ROOKIE = 3,
  CHAMPION = 4,
  ULTIMATE = 5,
};

enum class Special : uint8_t {
  FIRE = 0,
  COMBAT = 1,
  AIR = 2,
  NATURE = 3,
  ICE = 4,
  MACHINE = 5,
  FILTH = 6,
  NONE = 0xFF,
};

struct DigimonPara {
  char name[20];
  int32_t boneCount;
  int16_t radius;
  int16_t height;
  Type type;
  Level level;
  Special special[3];
  uint8_t dropItem;
  uint8_t dropChance;
  int8_t moves[16];
  uint8_t padding;
};

static_assert(sizeof(DigimonPara) == 0x34);

struct PartnerPara {
    int32_t condition;
    int16_t sleepyHour;
    int16_t sleepyMinute;
    int16_t wakeupHour;
    int16_t wakeupMinute;
    int16_t wakeupDefault;
    int16_t sleepyDefault;
    int16_t awakeToday;
    int16_t sicknessCounter;
    int16_t missedSleepHours;
    int16_t tirednessSleepTimer;
    int16_t poopLevel;
    int16_t unused1;
    int16_t unused2;
    int16_t virusBar;
    int16_t poopingTimer;
    int16_t tiredness;
    int16_t subTiredness;
    int16_t tirednessHungerTimer;
    int16_t discipline;
    int16_t happiness;
    int16_t unused3;
    int16_t unused4;
    int16_t timesBeingSick;
    int16_t sicknessTries;
    int16_t areaEffectTimer;
    int16_t sicknessTimer;
    int16_t injuryTimer;
    int16_t nextHungerHour;
    int16_t energyLevel;
    int16_t foodLevel;
    int16_t starvationTimer;
    int16_t weight;
    int16_t refusedFavFood;
    int16_t emptyStomachTimer;
    int16_t remainingLifetime;
    int16_t age;
    uint16_t trainBoostFlag;
    uint16_t trainBoostValue;
    uint16_t trainBoostTimer;
    int16_t careMistakes;
    int16_t battles;
    int16_t evoTimer;
    int16_t unused5;
    int16_t unused6;
    int16_t unused7;
    int16_t fishCaught;
    int16_t unused8;
    int16_t unused9;
    int16_t unused10;
    int16_t upgradeHPcounter;
    int16_t upgradeMPcounter;
    int16_t upgradeOffCounter;
    int16_t buggedUpgradeBrainsCounter;
    int16_t upgradeDefenseCounter;
    int16_t upgradeSpeedCounter;
    int16_t upgradeBrainCounter;
    int16_t unused11;
    int16_t sukaBackupHP;
    int16_t sukaBackupMP;
    int16_t sukaBackupOff;
    int16_t sukaBackupDef;
    int16_t sukaBackupSpeed;
    int16_t sukaBackupBrain;
    int16_t unused12;
};

static_assert(sizeof(PartnerPara) == 0x84);

extern PartnerPara PARTNER_PARA;
// dummy size, used for unbound memory access
extern DigimonPara DIGIMON_PARA[1];

extern uint16_t CHAR_TO_GLYPH_TABLE[80];
extern GlyphData GLYPH_DATA[79];
extern uint8_t COLORCODE_HIGHBITS;
extern uint8_t COLORCODE_LOWBITS;

extern uint8_t *jis_at_index(uint8_t *string, uint32_t index);
extern uint16_t convertAsciiToJis(uint8_t input);
extern void *memset(void *dest, int fillbyte, uint32_t length);
extern uint32_t strlen(uint8_t* string);
extern uint8_t* strncpy(uint8_t* dest, uint8_t* src, uint32_t byte_cnt);
extern void clearTextSubArea(RECT* rect);
extern int libgpu_LoadImage(RECT *rect, uint32_t *data);
}