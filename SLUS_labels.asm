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
.definelabel random,                    0x800a36d4
.definelabel Tamer_setState,            0x800aa188
.definelabel clearTextArea,             0x8010cb90
.definelabel setTextColor,              0x8010cc0c
.definelabel callScriptSection,         0x80105b14
.definelabel addWithLimit,              0x800c563c
.definelabel addTamerLevel,             0x800acbf4
.definelabel addHealingParticleEffect,  0x800c1560
.definelabel removeTamerItem,           0x800c55fc
.definelabel hasDigimonRaised,          0x800ff824
.definelabel Tamer_getState,            0x800ac050
.definelabel Partner_setState,          0x800df4d0
.definelabel renderSeperatorLines,      0x800b8360
.definelabel renderRectPolyFT4,         0x800b8928
.definelabel renderInsetBox,            0x800be734
.definelabel renderdigimonstatsbar,     0x800bec28
.definelabel renderDigiviceEntity,      0x800beebc
.definelabel renderBoxBar,              0x800be85c
.definelabel updateConditionAnimation,  0x800df2d0
.definelabel unsetButterfly,            0x800e9fb4
.definelabel setSleepDisabled,          0x800ba40c
.definelabel advanceToTime,             0x800dd678
.definelabel setFoodTimer,              0x800a4a08
.definelabel getModelTile,              0x800c0f28
.definelabel createPoopFX,              0x800ae3c4
.definelabel getItemCount,              0x800c51e0
.definelabel setTrigger,                0x801065c0
.definelabel unsetBubble,               0x800ece20
.definelabel addConditionBubble,        0x800ec538
.definelabel setButterfly,              0x800e9d24
.definelabel startAnimation,            0x800c1a04

.definelabel ITEM_FUNCTIONS, 0x80127c88
.definelabel IS_SCRIPT_PAUSED, 0x80134ff4
.definelabel PARTNER_ENTITY, 0x801557a8
.definelabel CHAR_TO_GLYPH_TABLE, 0x801307e0
.definelabel GLYPH_DATA, 0x80130880
.definelabel COLORCODE_HIGHBITS, 0x8013465d
.definelabel COLORCODE_LOWBITS, 0x8013465c
.definelabel DIGIMON_DATA, 0x8012ceb4
.definelabel PARTNER_PARA, 0x80138460
.definelabel RNG_STATE, 0x80009010
.definelabel TEXT_COLORS, 0x8012b918
.definelabel ACTIVE_ORDERING_TABLE, 0x80134ed0
.definelabel NANIMON_TRIGGER, 0x80134c64
.definelabel EVOLUTION_TARGET, 0x80134e50
.definelabel CURRENT_SCREEN, 0x80134da8
.definelabel MENU_STATE, 0x80134d34
.definelabel SLEEP_PATTERN, 0x80122cf4
.definelabel PLAYTIME_FRAMES, 0x80134f00
.definelabel DIGIVICE_ENTITY_VIEW, 0x80123860
.definelabel CURRENT_FRAME, 0x80134f08
.definelabel LAST_HANDLED_FRAME, 0x80134f06
.definelabel HOUR, 0x80134ebc
.definelabel MINUTE, 0x80134ebe
.definelabel YEAR, 0x80134f02
.definelabel DAY, 0x80134f04
.definelabel BUTTERFLY_ID, 0x80134c60
.definelabel HAS_BUTTERFLY, 0x80134c54
.definelabel ITEM_SCOLD_FLAG, 0x80134c59
.definelabel HAS_IMMORTAL_HOUR, 0x80134ca0
.definelabel IMMORTAL_HOUR, 0x80134ca4
.definelabel WORLD_POOP, 0x80138504
.definelabel CURRENT_POOP_ID, 0x80134c58
.definelabel PARTNER_AREA_RESPONSE, 0x80150c3e
.definelabel TAMER_ITEM, 0x8013d4d0
.definelabel EVOLUTION_ITEM_TARGET, 0x80127c5c
.definelabel PARTNER_STATE, 0x80134c5b
.definelabel PARTNER_ANIMATION, 0x80134e2c
.definelabel POOP_TO_EAT, 0x80134c68

// libc
.definelabel strcat,  0x800911cc
.definelabel strcmp,  0x800911dc
.definelabel strncmp, 0x800911ec
.definelabel strcpy,  0x800911fc
.definelabel strncpy, 0x8009120c
.definelabel strlen,  0x8009121c
.definelabel strrchr, 0x8009122c
.definelabel memset,  0x8009125c
.definelabel sprintf, 0x8009129c
.definelabel printf,  0x8009128c

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