.definelabel lookupFileTable, 0x800a31f8
.definelabel lookupFileTable_end, 0x800a32c8
.definelabel global_pointer, 0x8013bb2c
.definelabel padWithSpaces, 0x800ff51c
.definelabel unsetObject, 0x800a3008
.definelabel updateNamingPreview, 0x8010aa60
.definelabel playSound, 0x800c6374
.definelabel clearTextSubArea, 0x8010cbc4
.definelabel addObject, 0x800a2f64
.definelabel renderNumber, 0x800e5444
.definelabel drawString, 0x8010cf24
.definelabel convertAsciiToGameChar, 0x800f18c8
.definelabel swapShortBytes, 0x800f1ab0
.definelabel convertAsciiToJis, 0x800f18c8
.definelabel renderString, 0x800e5b50
.definelabel setUVDataPolyFT4, 0x800e56b4
.definelabel setPosDataPolyFT4, 0x800e56ec
.definelabel random,                    0x800a36d4
;.definelabel Tamer_setState,            0x800aa188 ; use constexpr version instead
.definelabel clearTextArea,             0x8010cb90
.definelabel setTextColor,              0x8010cc0c
.definelabel callScriptSection,         0x80105b14
.definelabel addWithLimit,              0x800c563c
.definelabel addTamerLevel,             0x800acbf4
.definelabel addHealingParticleEffect,  0x800c1560
.definelabel removeTamerItem,           0x800c55fc
.definelabel hasDigimonRaised,          0x800ff824
;.definelabel Tamer_getState,            0x800ac050 ; use constexpr version instead
;.definelabel Partner_setState,          0x800df4d0 ; use constexpr version instead
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
.definelabel getModelTile,              0x800c0f28
.definelabel createPoopFX,              0x800ae3c4
.definelabel getItemCount,              0x800c51e0
.definelabel setTrigger,                0x801065c0
.definelabel unsetBubble,               0x800ece20
.definelabel addConditionBubble,        0x800ec538
.definelabel setButterfly,              0x800e9d24
.definelabel startAnimation,            0x800c1a04
.definelabel writePStat,                0x80106474
.definelabel updateTimeOfDay,           0x800d676c
.definelabel isInDaytimeTransition,     0x800d7618
.definelabel removeObject,              0x800a3008
.definelabel Partner_tick,              0x800dd7ac
.definelabel projectPosition,           0x800e1074
.definelabel renderObject,              0x800e215c
.definelabel loadPartnerSounds,         0x800c64e0
.definelabel learnMove,                 0x800e5f14
.definelabel readFile,                  0x800a3410
.definelabel initializeConditionBubbles,0x800ec520
.definelabel initializeButterfly,       0x800e9ce8
.definelabel add3DSpritePrim,           0x800da1fc
.definelabel setupModelMatrix,          0x800c19a4
.definelabel renderDropShadow,          0x800a3f60
.definelabel setRotTransMatrix,         0x800a3924
.definelabel readFileSectors,           0x800a361c
.definelabel updateTMDTextureData,      0x800a3e74
.definelabel lookupFileSize,            0x800a32cc
.definelabel addFileReadRequest2,       0x800e3938

.definelabel DIGIMON_SKELETONS, 0x8011ce60
.definelabel PARTNER_MOMENTUM_DATA, 0x80185be8
.definelabel PARTNER_POSITION_DATA, 0x801866cc
.definelabel UNKNOWN_MODEL, 0x80137218
.definelabel UNKNOWN_MODEL_TAKEN, 0x801373d8
.definelabel NPC_MODEL, 0x80137140
.definelabel NPC_MODEL_TAKEN, 0x801371cc
.definelabel TAMER_MODEL, 0x801371e0
.definelabel TAMER_MODEL_BUFFER, 0x801a1398
.definelabel PARTNER_MODEL, 0x801371fc
.definelabel PARTNER_MODEL_BUFFER, 0x801878dc
.definelabel PTR_DIGIMON_FILE_NAMES, 0x8011d19c
.definelabel NPC_ENTITIES, 0x80155828

.definelabel TAMER_MOMENTUM_DATA, 0x801a00dc
.definelabel TAMER_POSITION_DATA, 0x801a07e8

.definelabel STATUS_UI_OFFSET_X, 0x80134c50
.definelabel IS_NATURAL_DEATH, 0x80134c4c
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
.definelabel PARTNER_SUB_STATE, 0x80134c5c
.definelabel TAMER_STATE, 0x80134c91
.definelabel TAMER_SUB_STATE, 0x80134c92
.definelabel PARTNER_ANIMATION, 0x80134e2c
.definelabel POOP_TO_EAT, 0x80134c68
.definelabel GAME_STATE, 0x80134f0a
.definelabel FADE_DATA, 0x80134cac
.definelabel IS_GAMETIME_RUNNING, 0x80134e20
.definelabel TAMER_ENTITY,              0x8015576c
.definelabel DEATH_STATS,               0x801384e4
.definelabel REINCARNATION_MODEL_DATA,  0x8008788c
.definelabel ENTITY_TABLE,              0x8012f344
.definelabel STOP_DISTANCE_TIMER,       0x80134c5a
.definelabel EVO_SEQUENCE_DATA,         0x80154f90
.definelabel POOP_ROTATION_MATRIX,      0x801341ec
.definelabel MAP_LAYER_ENABLED,         0x80134ec8
.definelabel HAS_USED_EVOITEM,          0x80134e52
.definelabel PLAYER_SHADOW_ENABLED,     0x80133b3c
.definelabel ENTITY1_WIREFRAME_TOTAL,   0x80133b38
.definelabel PARTNER_WIREFRAME_TOTAL,   0x80133b34
.definelabel PARTNER_WIREFRAME_SUB,     0x8011d130
.definelabel WIREFRAME_COLOR_MIN, 0x80133b40
.definelabel WIREFRAME_COLOR_MAX, 0x80133b42
.definelabel WIREFRAME_RNG_TABLE, 0x8011d180

// libapi
.definelabel libapi_malloc3, 0x80090f84
.definelabel libapi_free3, 0x80091034

// libc
.definelabel strcat,  0x800911cc
.definelabel strcmp,  0x800911dc
.definelabel strncmp, 0x800911ec
.definelabel strcpy,  0x800911fc
.definelabel strncpy, 0x8009120c
.definelabel strlen,  0x8009121c
.definelabel strrchr, 0x8009122c
.definelabel memcpy,  0x8009124c
.definelabel memset,  0x8009125c
.definelabel sprintf, 0x8009129c
.definelabel printf,  0x8009128c
.definelabel rand,    0x8009127c

// libcd
.definelabel libcd_CdSearchFile, 0x800b23c0

// libetc
.definelabel libetc_InterruptCallback, 0x8009237c

// libgs
.definelabel libgs_GsGetWorkBase, 0x80098c38
.definelabel libgs_GsSetWorkBase, 0x80098c28
.definelabel libgs_GsMapModelingData, 0x80098158
.definelabel libgs_GsLinkObject4, 0x80099bf8
.definelabel libgs_GsInitCoordinate2, 0x80097d68
.definelabel libgs_GsGetLws, 0x800998d8
.definelabel libgs_GsSetLightMatrix, 0x80097e04
.definelabel libgs_GsSetLsMatrix, 0x80097dd8
.definelabel libgs_GsSortObject4, 0x80099d88
.definelabel libgs_GsGetTimInfo, 0x80098a68

.definelabel libgs_REFERENCE_MATRIX, 0x80136f84
.definelabel libgs_GsLIGHTWSMATRIX, 0x80136f24

// libgpu
.definelabel libgpu_ClearImage, 0x80094818
.definelabel libgpu_LoadImage, 0x800948a8
.definelabel libgpu_AddPrim, 0x80092ad4
.definelabel libgpu_GetClut, 0x80092abc
.definelabel libgpu_SetPolyFT4, 0x80092b88
.definelabel libgpu_SetLineF2, 0x80092bc4
.definelabel libgpu_SetLineF3, 0x80092bd8
.definelabel libgpu_SetLineF4, 0x80092bf8
.definelabel libgpu_DrawSync, 0x800947b0

// libgte
.definelabel libgte_setRotMatrix, 0x8009b200
.definelabel libgte_setTransMatrix, 0x8009b290
.definelabel libgte_RotMatrix, 0x8009b804
.definelabel libgte_ScaleMatrix, 0x8009b0c0
.definelabel libgte_TransMatrix, 0x8009b090
.definelabel libgte_ApplyMatrixSV, 0x8009b030
.definelabel libgte_MulMatrix0, 0x8009a8a0
.definelabel libgte_RotNclip3, 0x8009b544
.definelabel libgte_RotNclip4, 0x8009b5d4
.definelabel libgte_NormalColorCol3, 0x8009b390
.definelabel libgte_NormalColorCol, 0x8009b370
.definelabel libgte_SetLightMatrix, 0x8009b230
.definelabel libgte_CompMatrix, 0x8009a9b0

// PSX internal
.definelabel TIMER0, 0x1F801100
.definelabel TIMER1, 0x1F801110
.definelabel TIMER2, 0x1F801120
.definelabel SCRATCHPAD, 0x1F800000