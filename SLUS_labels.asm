.definelabel lookupFileTable, 0x800a31f8
.definelabel lookupFileTable_end, 0x800a32c8
.definelabel global_pointer, 0x8013bb2c
.definelabel padWithSpaces, 0x800ff51c
.definelabel unsetObject, 0x800a3008
.definelabel updateNamingPreview, 0x8010aa60
.definelabel playSound, 0x800c6374
.definelabel clearTextSubArea, 0x8010cbc4
.definelabel addObject, 0x800a2f64
.definelabel initializeStatusObjects, 0x800a435c
.definelabel renderNumber, 0x800e5444
.definelabel drawString, 0x8010cf24
.definelabel convertAsciiToGameChar, 0x800f18c8
.definelabel swapShortBytes, 0x800f1ab0
.definelabel convertAsciiToJis, 0x800f18c8
.definelabel renderString, 0x800e5b50
.definelabel setUVDataPolyFT4, 0x800e56b4
.definelabel setPosDataPolyFT4, 0x800e56ec

.definelabel CHAR_TO_GLYPH_TABLE, 0x801307e0
.definelabel GLYPH_DATA, 0x80130880
.definelabel COLORCODE_HIGHBITS, 0x8013465d
.definelabel COLORCODE_LOWBITS, 0x8013465c
.definelabel DIGIMON_PARA, 0x8012ceb4
.definelabel PARTNER_PARA, 0x80138460
.definelabel RNG_STATE, 0x80009010
.definelabel TEXT_COLORS, 0x8012b918
.definelabel ACTIVE_ORDERING_TABLE, 0x80134ed0

// libc
.definelabel strcat,  0x800911cc
.definelabel strcmp,  0x800911dc
.definelabel strncmp, 0x800911ec
.definelabel strcpy,  0x800911fc
.definelabel strncpy, 0x8009120c
.definelabel strlen,  0x8009121c
.definelabel strrchr,  0x8009122c
.definelabel memset, 0x8009125c
.definelabel sprintf, 0x8009129c

// libcd
.definelabel libcd_CdSearchFile, 0x800b23c0

// libetc
.definelabel libetc_InterruptCallback, 0x8009237c

// libgs
.definelabel libgs_GsGetWorkBase, 0x80098c38
.definelabel libgs_GsSetWorkBase, 0x80098c28

// libgs
.definelabel libgpu_ClearImage, 0x80094818
.definelabel libgpu_LoadImage, 0x800948a8
.definelabel libgpu_AddPrim, 0x80092ad4
.definelabel libgpu_GetClut, 0x80092abc
.definelabel libgpu_SetPolyFT4, 0x80092b88

// PSX internal
.definelabel TIMER0, 0x1F801100
.definelabel TIMER1, 0x1F801110
.definelabel TIMER2, 0x1F801120