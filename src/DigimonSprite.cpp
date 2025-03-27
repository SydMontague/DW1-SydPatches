

#include "DigimonSprite.hpp"

constexpr DigimonSprite digimonSprites[] = {
    {.uvX = 0x00, .uvV = 0x00, .clut = 0x0, .tpage = 24}, // TAMER
    {.uvX = 0x40, .uvV = 0x90, .clut = 0x0, .tpage = 24}, // BOTAMON
    {.uvX = 0x60, .uvV = 0x90, .clut = 0x0, .tpage = 24}, // KOROMON
    {.uvX = 0x40, .uvV = 0x80, .clut = 0x0, .tpage = 24}, // AGUMON
    {.uvX = 0x60, .uvV = 0x80, .clut = 0x1, .tpage = 24}, // BETAMON
    {.uvX = 0x40, .uvV = 0x30, .clut = 0x1, .tpage = 24}, // GREYMON
    {.uvX = 0x60, .uvV = 0x30, .clut = 0x3, .tpage = 24}, // DEVIMON
    {.uvX = 0x80, .uvV = 0x30, .clut = 0x1, .tpage = 24}, // AIRDRAMON
    {.uvX = 0xA0, .uvV = 0x30, .clut = 0x0, .tpage = 24}, // TYRANNOMON
    {.uvX = 0xC0, .uvV = 0x30, .clut = 0x0, .tpage = 24}, // MERAMON
    {.uvX = 0xE0, .uvV = 0x30, .clut = 0x1, .tpage = 24}, // SEADRAMON
    {.uvX = 0xA0, .uvV = 0x70, .clut = 0x1, .tpage = 24}, // NUMEMON
    {.uvX = 0xC0, .uvV = 0x00, .clut = 0x0, .tpage = 24}, // METALGREYMON
    {.uvX = 0xE0, .uvV = 0x00, .clut = 0x0, .tpage = 24}, // MAMEMON
    {.uvX = 0x00, .uvV = 0x10, .clut = 0x0, .tpage = 24}, // MONZAEMON
    {.uvX = 0x80, .uvV = 0x90, .clut = 0x0, .tpage = 24}, // PUNIMON
    {.uvX = 0xA0, .uvV = 0x90, .clut = 0x0, .tpage = 24}, // TSUNOMON
    {.uvX = 0x80, .uvV = 0x80, .clut = 0x2, .tpage = 24}, // GABUMON
    {.uvX = 0xA0, .uvV = 0x80, .clut = 0x0, .tpage = 24}, // ELECMON
    {.uvX = 0x00, .uvV = 0x40, .clut = 0x0, .tpage = 24}, // KABUTERIMON
    {.uvX = 0x20, .uvV = 0x40, .clut = 0x0, .tpage = 24}, // ANGEMON
    {.uvX = 0x40, .uvV = 0x40, .clut = 0x0, .tpage = 24}, // BIRDRAMON
    {.uvX = 0x60, .uvV = 0x40, .clut = 0x2, .tpage = 24}, // GARURUMON
    {.uvX = 0x80, .uvV = 0x40, .clut = 0x0, .tpage = 24}, // FRIGIMON
    {.uvX = 0xA0, .uvV = 0x40, .clut = 0x0, .tpage = 24}, // WHAMON
    {.uvX = 0xC0, .uvV = 0x70, .clut = 0x1, .tpage = 24}, // VEGIEMON
    {.uvX = 0x20, .uvV = 0x10, .clut = 0x0, .tpage = 24}, // SKULLGREYMON
    {.uvX = 0x40, .uvV = 0x10, .clut = 0x0, .tpage = 24}, // METALMAMEMON
    {.uvX = 0x60, .uvV = 0x10, .clut = 0x0, .tpage = 24}, // VADEMON
    {.uvX = 0xC0, .uvV = 0x90, .clut = 0x2, .tpage = 24}, // POYOMON
    {.uvX = 0xE0, .uvV = 0x90, .clut = 0x0, .tpage = 24}, // TOKOMON
    {.uvX = 0xC0, .uvV = 0x80, .clut = 0x0, .tpage = 24}, // PATAMON
    {.uvX = 0xE0, .uvV = 0x80, .clut = 0x0, .tpage = 24}, // KUNEMON
    {.uvX = 0xC0, .uvV = 0x40, .clut = 0x0, .tpage = 24}, // UNIMON
    {.uvX = 0xE0, .uvV = 0x40, .clut = 0x1, .tpage = 24}, // OGREMON
    {.uvX = 0x00, .uvV = 0x50, .clut = 0x2, .tpage = 24}, // SHELLMON
    {.uvX = 0x20, .uvV = 0x50, .clut = 0x0, .tpage = 24}, // CENTARUMON
    {.uvX = 0x40, .uvV = 0x50, .clut = 0x0, .tpage = 24}, // BAKEMON
    {.uvX = 0x60, .uvV = 0x50, .clut = 0x0, .tpage = 24}, // DRIMOGEMON
    {.uvX = 0xE0, .uvV = 0x70, .clut = 0x0, .tpage = 24}, // SUKAMON
    {.uvX = 0x80, .uvV = 0x10, .clut = 0x3, .tpage = 24}, // ANDROMON
    {.uvX = 0xA0, .uvV = 0x10, .clut = 0x0, .tpage = 24}, // GIROMON
    {.uvX = 0xC0, .uvV = 0x10, .clut = 0x0, .tpage = 24}, // ETEMON
    {.uvX = 0x00, .uvV = 0xA0, .clut = 0x0, .tpage = 24}, // YURAMON
    {.uvX = 0x20, .uvV = 0xA0, .clut = 0x1, .tpage = 24}, // TANEMON
    {.uvX = 0x00, .uvV = 0x90, .clut = 0x2, .tpage = 24}, // BIYOMON
    {.uvX = 0x20, .uvV = 0x90, .clut = 0x2, .tpage = 24}, // PALMON
    {.uvX = 0x80, .uvV = 0x50, .clut = 0x3, .tpage = 24}, // MONOCHROMON
    {.uvX = 0xA0, .uvV = 0x50, .clut = 0x3, .tpage = 24}, // LEOMON
    {.uvX = 0xC0, .uvV = 0x50, .clut = 0x3, .tpage = 24}, // COELAMON
    {.uvX = 0xE0, .uvV = 0x50, .clut = 0x0, .tpage = 24}, // KOKATORIMON
    {.uvX = 0x00, .uvV = 0x60, .clut = 0x0, .tpage = 24}, // KUWAGAMON
    {.uvX = 0x20, .uvV = 0x60, .clut = 0x0, .tpage = 24}, // MOJYAMON
    {.uvX = 0x00, .uvV = 0x80, .clut = 0x0, .tpage = 24}, // NANIMON
    {.uvX = 0xE0, .uvV = 0x10, .clut = 0x0, .tpage = 24}, // MEGADRAMON
    {.uvX = 0x00, .uvV = 0x20, .clut = 0x0, .tpage = 24}, // PIXIMON
    {.uvX = 0x20, .uvV = 0x20, .clut = 0x1, .tpage = 24}, // DIGITAMAMON
    {.uvX = 0x60, .uvV = 0x60, .clut = 0x0, .tpage = 24}, // PENGUINMON
    {.uvX = 0x40, .uvV = 0x60, .clut = 0x0, .tpage = 24}, // NINJAMON
    {.uvX = 0x20, .uvV = 0x00, .clut = 0x0, .tpage = 24}, // PHOENIXMON
    {.uvX = 0x40, .uvV = 0x00, .clut = 0x4, .tpage = 24}, // HERCULESKABUTERIMON
    {.uvX = 0x60, .uvV = 0x00, .clut = 0x3, .tpage = 24}, // MEGASEADRAMON
    {.uvX = 0x00, .uvV = 0x00, .clut = 0x0, .tpage = 0},  // WEREGARURUMON
    {.uvX = 0x00, .uvV = 0x00, .clut = 0x0, .tpage = 0},  // PANJYAMON
    {.uvX = 0x00, .uvV = 0x00, .clut = 0x0, .tpage = 0},  // GIGADRAMON
    {.uvX = 0x00, .uvV = 0x00, .clut = 0x0, .tpage = 0},  // METALETEMON
    {.uvX = 0xA0, .uvV = 0x00, .clut = 0x3, .tpage = 24}, // MYOTISMON
    {.uvX = 0xC0, .uvV = 0x60, .clut = 0x0, .tpage = 24}, // YANMAMON
    {.uvX = 0xE0, .uvV = 0x60, .clut = 0x3, .tpage = 24}, // GOTSUMON
    {.uvX = 0x00, .uvV = 0x70, .clut = 0x0, .tpage = 24}, // FLAREIZAMON
    {.uvX = 0x60, .uvV = 0x20, .clut = 0x4, .tpage = 24}, // WARUMONZAEMON
    {.uvX = 0x00, .uvV = 0x00, .clut = 0x0, .tpage = 0},  // SNOWAGUMON
    {.uvX = 0x00, .uvV = 0x00, .clut = 0x0, .tpage = 0},  // HYOGAMON
    {.uvX = 0x00, .uvV = 0x00, .clut = 0x0, .tpage = 0},  // PLATINUMNUMEMON
    {.uvX = 0x00, .uvV = 0x00, .clut = 0x0, .tpage = 0},  // DOKUNEMON
    {.uvX = 0x00, .uvV = 0x00, .clut = 0x0, .tpage = 0},  // SHIMAUNIMON
    {.uvX = 0x60, .uvV = 0x70, .clut = 0x3, .tpage = 24}, // TANKMON
    {.uvX = 0x00, .uvV = 0x00, .clut = 0x0, .tpage = 0},  // REDVEGIMON
    {.uvX = 0x00, .uvV = 0x00, .clut = 0x0, .tpage = 0},  // JMOJYAMON
    {.uvX = 0x00, .uvV = 0x00, .clut = 0x0, .tpage = 0},  // NISEDRIMOGEMON
    {.uvX = 0x80, .uvV = 0x70, .clut = 0x1, .tpage = 24}, // GOBURIMON
    {.uvX = 0x00, .uvV = 0x00, .clut = 0x0, .tpage = 0},  // MUDFRIGIMON
    {.uvX = 0x00, .uvV = 0x00, .clut = 0x0, .tpage = 0},  // PSYCHEMON
    {.uvX = 0x00, .uvV = 0x00, .clut = 0x0, .tpage = 0},  // MODOKIBETAMON
    {.uvX = 0x20, .uvV = 0x70, .clut = 0x0, .tpage = 24}, // TOYAGUMON
    {.uvX = 0x00, .uvV = 0x00, .clut = 0x0, .tpage = 0},  // PIDDOMON
    {.uvX = 0x00, .uvV = 0x00, .clut = 0x0, .tpage = 0},  // ARURAUMON
    {.uvX = 0x00, .uvV = 0x00, .clut = 0x0, .tpage = 0},  // GEREMON
    {.uvX = 0x00, .uvV = 0x00, .clut = 0x0, .tpage = 0},  // VERMILIMON
    {.uvX = 0x00, .uvV = 0x00, .clut = 0x0, .tpage = 0},  // FUGAMON
    {.uvX = 0x00, .uvV = 0x00, .clut = 0x0, .tpage = 0},  // TEKKAMON
    {.uvX = 0x00, .uvV = 0x00, .clut = 0x0, .tpage = 0},  // MORISHELLMON
    {.uvX = 0xE0, .uvV = 0x20, .clut = 0x4, .tpage = 24}, // GUARDROMON
    {.uvX = 0x00, .uvV = 0x00, .clut = 0x0, .tpage = 0},  // MUCHOMON
    {.uvX = 0x00, .uvV = 0x00, .clut = 0x0, .tpage = 0},  // ICEMON
    {.uvX = 0x00, .uvV = 0x00, .clut = 0x0, .tpage = 0},  // AKATORIMON
    {.uvX = 0x00, .uvV = 0x00, .clut = 0x0, .tpage = 0},  // TSUKAIMON
    {.uvX = 0x00, .uvV = 0x00, .clut = 0x0, .tpage = 0},  // SHARMAMON
    {.uvX = 0x00, .uvV = 0x00, .clut = 0x0, .tpage = 0},  // CLEARAGUMON
    {.uvX = 0x00, .uvV = 0x00, .clut = 0x0, .tpage = 0},  // WEEDMON
    {.uvX = 0x00, .uvV = 0x00, .clut = 0x0, .tpage = 0},  // ICEDEVIMON
    {.uvX = 0x00, .uvV = 0x00, .clut = 0x0, .tpage = 0},  // DARKRIZAMON
    {.uvX = 0x00, .uvV = 0x00, .clut = 0x0, .tpage = 0},  // SANDYANMAMON
    {.uvX = 0x00, .uvV = 0x00, .clut = 0x0, .tpage = 0},  // SNOWGOBURIMON
    {.uvX = 0x00, .uvV = 0x00, .clut = 0x0, .tpage = 0},  // BLUEMERAMON
    {.uvX = 0x00, .uvV = 0x00, .clut = 0x0, .tpage = 0},  // GURURUMON
    {.uvX = 0x00, .uvV = 0x00, .clut = 0x0, .tpage = 0},  // SABERDRAMON
    {.uvX = 0x00, .uvV = 0x00, .clut = 0x0, .tpage = 0},  // SOULMON
    {.uvX = 0x00, .uvV = 0x00, .clut = 0x0, .tpage = 0},  // ROCKMON
    {.uvX = 0x80, .uvV = 0x60, .clut = 0x0, .tpage = 24}, // OTAMAMON
    {.uvX = 0x40, .uvV = 0x20, .clut = 0x1, .tpage = 24}, // GEKOMON
    {.uvX = 0xA0, .uvV = 0x60, .clut = 0x0, .tpage = 24}, // TENTOMON
    {.uvX = 0x00, .uvV = 0x00, .clut = 0x0, .tpage = 0},  // WARUSEADRAMON
    {.uvX = 0x00, .uvV = 0x00, .clut = 0x0, .tpage = 0},  // METEORMON
    {.uvX = 0x00, .uvV = 0x00, .clut = 0x0, .tpage = 0},  // UNKNOWN
    {.uvX = 0x20, .uvV = 0x80, .clut = 0x0, .tpage = 24}, // MACHINEDRAMON
    {.uvX = 0x00, .uvV = 0x00, .clut = 0x0, .tpage = 0},  // ANALOGMAN
    {.uvX = 0x80, .uvV = 0x20, .clut = 0x4, .tpage = 24}, // JIJIMON
    {.uvX = 0x00, .uvV = 0x00, .clut = 0x0, .tpage = 0},  // MARKET_MANAGER
    {.uvX = 0x80, .uvV = 0x00, .clut = 0x0, .tpage = 24}, // SHOGUNGEKOMON
    {.uvX = 0xA0, .uvV = 0x20, .clut = 0x3, .tpage = 24}, // KING_SUKAMON
    {.uvX = 0xC0, .uvV = 0x20, .clut = 0x3, .tpage = 24}, // CHERRYMON
    {.uvX = 0x00, .uvV = 0x30, .clut = 0x3, .tpage = 24}, // HAGURUMON
    {.uvX = 0x00, .uvV = 0x00, .clut = 0x0, .tpage = 0},  // TINMON
    {.uvX = 0x00, .uvV = 0x00, .clut = 0x0, .tpage = 0},  // MASTER_TYRANNOMON
    {.uvX = 0x00, .uvV = 0x00, .clut = 0x0, .tpage = 0},  // NPC_GOBIROMON
    {.uvX = 0x20, .uvV = 0x30, .clut = 0x6, .tpage = 24}, // BRACHIOMON
    {.uvX = 0x40, .uvV = 0x70, .clut = 0x0, .tpage = 24}, // DEMIMERAMON
    {.uvX = 0x00, .uvV = 0x00, .clut = 0x0, .tpage = 0},  // NPC_BETAMON
    {.uvX = 0x00, .uvV = 0x00, .clut = 0x0, .tpage = 0},  // NPC_GREYMON
    {.uvX = 0x00, .uvV = 0x00, .clut = 0x0, .tpage = 0},  // NPC_DEVIMON
    {.uvX = 0x00, .uvV = 0x00, .clut = 0x0, .tpage = 0},  // NPC_AIRDRAMON
    {.uvX = 0x00, .uvV = 0x00, .clut = 0x0, .tpage = 0},  // NPC_TYRANNOMON
    {.uvX = 0x00, .uvV = 0x00, .clut = 0x0, .tpage = 0},  // NPC_MERAMON
    {.uvX = 0x00, .uvV = 0x00, .clut = 0x0, .tpage = 0},  // NPC_SEADRAMON
    {.uvX = 0x00, .uvV = 0x00, .clut = 0x0, .tpage = 0},  // NPC_NUMEMON
    {.uvX = 0x00, .uvV = 0x00, .clut = 0x0, .tpage = 0},  // NPC_METALGREYMON
    {.uvX = 0x00, .uvV = 0x00, .clut = 0x0, .tpage = 0},  // NPC_MAMEMON
    {.uvX = 0x00, .uvV = 0x00, .clut = 0x0, .tpage = 0},  // NPC_MONZAEMON
    {.uvX = 0x00, .uvV = 0x00, .clut = 0x0, .tpage = 0},  // NPC_GABUMON
    {.uvX = 0x00, .uvV = 0x00, .clut = 0x0, .tpage = 0},  // NPC_ELECMON
    {.uvX = 0x00, .uvV = 0x00, .clut = 0x0, .tpage = 0},  // NPC_kABUTERIMON
    {.uvX = 0x00, .uvV = 0x00, .clut = 0x0, .tpage = 0},  // NPC_ANGEMON
    {.uvX = 0x00, .uvV = 0x00, .clut = 0x0, .tpage = 0},  // NPC_BIDRAMON
    {.uvX = 0x00, .uvV = 0x00, .clut = 0x0, .tpage = 0},  // NPC_GARURUMON
    {.uvX = 0x00, .uvV = 0x00, .clut = 0x0, .tpage = 0},  // NPC_FRIGIMON
    {.uvX = 0x00, .uvV = 0x00, .clut = 0x0, .tpage = 0},  // NPC_WHAMON
    {.uvX = 0x00, .uvV = 0x00, .clut = 0x0, .tpage = 0},  // NPC_VEGIMON
    {.uvX = 0x00, .uvV = 0x00, .clut = 0x0, .tpage = 0},  // NPC_SKULLGREYMON
    {.uvX = 0x00, .uvV = 0x00, .clut = 0x0, .tpage = 0},  // NPC_METALMAMEMON
    {.uvX = 0x00, .uvV = 0x00, .clut = 0x0, .tpage = 0},  // NPC_VADEMON
    {.uvX = 0x00, .uvV = 0x00, .clut = 0x0, .tpage = 0},  // NPC_PATAMON
    {.uvX = 0x00, .uvV = 0x00, .clut = 0x0, .tpage = 0},  // NPC_KUNEMON
    {.uvX = 0x00, .uvV = 0x00, .clut = 0x0, .tpage = 0},  // NPC_UNIMON
    {.uvX = 0x00, .uvV = 0x00, .clut = 0x0, .tpage = 0},  // NPC_OGREMON
    {.uvX = 0x00, .uvV = 0x00, .clut = 0x0, .tpage = 0},  // NPC_SHELLMON
    {.uvX = 0x00, .uvV = 0x00, .clut = 0x0, .tpage = 0},  // NPC_CENTARUMON
    {.uvX = 0x00, .uvV = 0x00, .clut = 0x0, .tpage = 0},  // NPC_BAKEMON
    {.uvX = 0x00, .uvV = 0x00, .clut = 0x0, .tpage = 0},  // NPC_DRIMOGEMON
    {.uvX = 0x00, .uvV = 0x00, .clut = 0x0, .tpage = 0},  // NPC_SUKAMON
    {.uvX = 0x00, .uvV = 0x00, .clut = 0x0, .tpage = 0},  // NPC_ANDROMON
    {.uvX = 0x00, .uvV = 0x00, .clut = 0x0, .tpage = 0},  // NPC_GIROMON
    {.uvX = 0x00, .uvV = 0x00, .clut = 0x0, .tpage = 0},  // NPC_ETEMON
    {.uvX = 0x00, .uvV = 0x00, .clut = 0x0, .tpage = 0},  // NPC_BIYOMON
    {.uvX = 0x00, .uvV = 0x00, .clut = 0x0, .tpage = 0},  // NPC_PALMON
    {.uvX = 0x00, .uvV = 0x00, .clut = 0x0, .tpage = 0},  // NPC_MONOCHROMON
    {.uvX = 0x00, .uvV = 0x00, .clut = 0x0, .tpage = 0},  // NPC_LEOMON
    {.uvX = 0x00, .uvV = 0x00, .clut = 0x0, .tpage = 0},  // NPC_COELAMON
    {.uvX = 0x00, .uvV = 0x00, .clut = 0x0, .tpage = 0},  // NPC_KOKATORIMON
    {.uvX = 0x00, .uvV = 0x00, .clut = 0x0, .tpage = 0},  // NPC_KUWAGAMON
    {.uvX = 0x00, .uvV = 0x00, .clut = 0x0, .tpage = 0},  // NPC_MOJYAMON
    {.uvX = 0x00, .uvV = 0x00, .clut = 0x0, .tpage = 0},  // NPC_NANIMON
    {.uvX = 0x00, .uvV = 0x00, .clut = 0x0, .tpage = 0},  // NPC_MEGADRAMON
    {.uvX = 0x00, .uvV = 0x00, .clut = 0x0, .tpage = 0},  // NPC_PIXIMON
    {.uvX = 0x00, .uvV = 0x00, .clut = 0x0, .tpage = 0},  // NPC_DIGITAMAMON
    {.uvX = 0x00, .uvV = 0x00, .clut = 0x0, .tpage = 0},  // NPC_NINJAMON
    {.uvX = 0x00, .uvV = 0x00, .clut = 0x0, .tpage = 0},  // NPC_PENGUINMON
    {.uvX = 0x00, .uvV = 0x00, .clut = 0x0, .tpage = 0},  // NPC_MYOTISMON
    {.uvX = 0x00, .uvV = 0x00, .clut = 0x0, .tpage = 0},  // NPC2_GREYMON
    {.uvX = 0x00, .uvV = 0x00, .clut = 0x0, .tpage = 0},  // NPC2_METALGREYMON
};

bool hasValidSprite(DigimonType type)
{
    if (type == DigimonType::INVALID) return false;
    if (type >= DigimonType::NPC2_METALGREYMON) return false;

    return digimonSprites[static_cast<uint32_t>(type)].tpage != 0;
}

DigimonSprite const& getDigimonSprite(DigimonType type)
{
    return digimonSprites[static_cast<int32_t>(type)];
}

void DigimonSprite::render(int16_t posX, int16_t posY, uint8_t layer, uint8_t flag, int32_t frame) const
{
    auto finalU = uvX;
    auto finalV = uvV;
    if ((frame & 1) != 0) finalU += 16;
    if ((frame & 2) != 0) finalU += 16;

    renderRectPolyFT4(posX, posY, 16, 16, finalU, finalV, tpage, DIGIMON_SPRITE_CLUTS[clut], layer, flag);
}