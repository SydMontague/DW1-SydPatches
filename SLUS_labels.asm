.definelabel global_pointer, 0x8013bb2c
.definelabel padWithSpaces, 0x800ff51c
.definelabel updateNamingPreview, 0x8010aa60
.definelabel playSound, 0x800c6374
.definelabel clearTextSubArea, 0x8010cbc4
.definelabel renderNumber, 0x800e5444
.definelabel drawString, 0x8010cf24
.definelabel convertAsciiToGameChar, 0x800f18c8
.definelabel swapShortBytes, 0x800f1ab0
.definelabel convertAsciiToJis, 0x800f18c8
.definelabel renderString, 0x800e5b50
.definelabel setUVDataPolyFT4, 0x800e56b4
.definelabel setPosDataPolyFT4, 0x800e56ec
.definelabel clearTextArea,             0x8010cb90
.definelabel setTextColor,              0x8010cc0c
.definelabel callScriptSection,         0x80105b14
.definelabel addWithLimit,              0x800c563c
.definelabel addHealingParticleEffect,  0x800c1560
.definelabel removeTamerItem,           0x800c55fc
.definelabel hasDigimonRaised,          0x800ff824
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
.definelabel getItemCount,              0x800c51e0
.definelabel setTrigger,                0x801065c0
.definelabel unsetBubble,               0x800ece20
.definelabel addConditionBubble,        0x800ec538
.definelabel setButterfly,              0x800e9d24
.definelabel startAnimation,            0x800c1a04
.definelabel writePStat,                0x80106474
.definelabel updateTimeOfDay,           0x800d676c
.definelabel isInDaytimeTransition,     0x800d7618
.definelabel Partner_tick,              0x800dd7ac
.definelabel projectPosition,           0x800e1074
.definelabel renderObject,              0x800e215c
.definelabel loadPartnerSounds,         0x800c64e0
.definelabel learnMove,                 0x800e5f14
.definelabel initializeConditionBubbles,0x800ec520
.definelabel initializeButterfly,       0x800e9ce8
.definelabel add3DSpritePrim,           0x800da1fc
.definelabel setupModelMatrix,          0x800c19a4
.definelabel addFileReadRequest2,       0x800e3938
.definelabel getTileTrigger,            0x800c0acc
.definelabel main,                      0x800ee800
.definelabel Tamer_tickBattle,          0x800f1214
.definelabel tickAnimation,             0x800c1c64
.definelabel Tamer_tickFishing,         0x800e4e18
.definelabel isKeyDown,                 0x800fc054
.definelabel tickTamerWaypoints,        0x800d4adc
.definelabel addTriangleMenu,           0x800b7c28
.definelabel unsetCameraFollowPlayer,   0x800d8b3c
.definelabel stopGameTime,              0x800dd798
.definelabel setPartnerIdling,          0x800b7c18
.definelabel addMapNameObject,          0x800d8fb0
.definelabel changeMap,                 0x800d9020
.definelabel startGameTime,             0x800dd7a0
.definelabel getEntityScreenPos,        0x800e52d8
.definelabel createAnimatedUIBox,       0x800c3014
.definelabel giveItem,                  0x800c5240
.definelabel setCameraFollowPlayer,     0x800d8b44
.definelabel pickupItem,                0x800c5490
.definelabel unsetUIBoxAnimated,        0x800c31fc
.definelabel isUIBoxAvailable,          0x800ba1d4
.definelabel entityLookAtLocation,      0x800d459c
.definelabel tickOpenChestTray,         0x800e21d0
.definelabel tickCloseChestTray,        0x800e223c
.definelabel setMapLayerEnabled,        0x800e65b8
.definelabel isSoundLoaded,             0x800e90cc
.definelabel loadDynamicLibrary,        0x800e61e4
.definelabel entityCheckCollision,      0x800d45ec
.definelabel removeTriangleMenu,        0x800c0510
.definelabel closeInventoryBoxes,       0x800db238
.definelabel removeUIBox1,              0x800c053c
.definelabel collisionGrace,            0x800d4884
.definelabel unlockMedal,               0x800ff830
.definelabel hasMedal,                  0x800ff85c
.definelabel getCardAmount,             0x801067ec
.definelabel renderUIBox,               0x80101440
.definelabel readMapTFS,                0x800d6308
.definelabel playBGM,                   0x80106a80
.definelabel loadBattleData,            0x800e74d0
.definelabel handleBattleStart,         0x800e847c
.definelabel closeTriangleMenu,         0x800b7ffc
.definelabel updateMinuteHand,          0x800dd780
.definelabel initializeDaytimeTransition,0x800d7104
.definelabel moveCameraByDiff,          0x800d892c
.definelabel worldPosToScreenPos,       0x800da2ec
.definelabel lerp,                      0x800d990c
.definelabel renderFXParticle,          0x800daa00
.definelabel addFXPrim,                 0x800da3fc
.definelabel isTamerOnScreen,           0x800e67ec
.definelabel getDrawingOffsetCopy,      0x800d969c
.definelabel isTriggerSet,              0x8010643c
.definelabel entityIsOffScreen,         0x800d5430
.definelabel NPCEntity_tickBattle,      0x800f1430
.definelabel initializeLoadedNPCModels, 0x80105aa8

.definelabel NPC_COLLISION_STATE, 0x8013d190
.definelabel LOADED_DIGIMON_MODELS, 0x801be7ec
.definelabel MAP_DIGIMON_TABLE, 0x8013cb50
.definelabel LOCAL_MAP_OBJECT_INSTANCE, 0x8014f340
.definelabel NPC_ACTIVE_ANIM, 0x80134d1e
.definelabel MIST_CLUT_Y, 0x80134230
.definelabel MIST_Y_OFFSETS, 0x8013422c
.definelabel MIST_X_OFFSETS, 0x80134224
.definelabel CAMERA_X, 0x80150c38
.definelabel CAMERA_Y, 0x80150c3a
.definelabel CAMERA_X_PREVIOUS, 0x80134dcc
.definelabel CAMERA_Y_PREVIOUS, 0x80134dce
.definelabel EFE_SCRIPT_MEM1_DATA, 0x80139a30
.definelabel EFE_LOAD_STATE, 0x80139a2c
.definelabel EFE_LOADED_MOVE_DATA, 0x80139a0c
.definelabel SOME_IMAGE_DATA, 0x80138c0c
.definelabel VIEWPORT_DISTANCE, 0x80134ed4
.definelabel TRAINING_COMPLETE, 0x80134ca0
.definelabel PREVIOUS_CAMERA_POS, 0x801387a8
.definelabel NPC_IS_WALKING_TOWARDS, 0x8013d170
.definelabel TEXTBOX_OPEN_TIMER, 0x800135012
.definelabel MONEY, 0x80134eb8
.definelabel PREVIOUS_EXIT, 0x80134dab
.definelabel CURRENT_SCRIPT_ID, 0x80134fc0
.definelabel TALKED_TO_ENTITY, 0x80134c9c
.definelabel GS_VIEWPOINT, 0x80185ba8
.definelabel SOME_SCRIPT_SYNC_BIT, 0x80134e30
.definelabel DOOA_DATA_PTR, 0x8005b000
.definelabel CHEST_ARRAY, 0x80150f00
.definelabel ITEM_PARA, 0x801269dc
.definelabel DROPPED_ITEMS, 0x8013d4e0
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
.definelabel TEXTURE_BUFFER, 0x80088800
.definelabel SECTION_DATA, 0x80113a60
.definelabel MAP_WARPS, 0x80138730
.definelabel UI_BOX_DATA, 0x8013d390
.definelabel POLLED_INPUT, 0x80134ee4
.definelabel TARGET_MAP, 0x80134de0
.definelabel CURRENT_EXIT, 0x80134daa

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
.definelabel HAS_IMMORTAL_HOUR, 0x80134ca8
.definelabel IMMORTAL_HOUR, 0x80134ca4
.definelabel WORLD_POOP, 0x80138504
.definelabel CURRENT_POOP_ID, 0x80134c58
.definelabel PARTNER_AREA_RESPONSE, 0x80150c3e
.definelabel TAMER_ITEM, 0x8013d4d0
.definelabel EVOLUTION_ITEM_TARGET, 0x80127c5c
.definelabel PARTNER_STATE, 0x80134c5b
.definelabel PARTNER_SUB_STATE, 0x80134c5c
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
.definelabel WORLD_OBJECTS, 0x80137418
.definelabel STORED_TAMER_POS, 0x80138720
.definelabel UNKNOWN_TAMER_DATA, 0x80138694
.definelabel IS_IN_MENU, 0x80134ef0
.definelabel SKIP_DAYTIME_TRANSITION, 0x80134db4
.definelabel GS_ORDERING_TABLE, 0x8015db68
.definelabel GS_WORK_BASES, 0x8015dba8
.definelabel ACTIVE_FRAMEBUFFER, 0x80134ecc
.definelabel ACTIVE_BGM_FONT, 0x80135006
.definelabel MAP_ENTRIES, 0x801292d4
.definelabel DRAWING_OFFSET_X, 0x80134ec0
.definelabel DRAWING_OFFSET_Y, 0x80134ec4
.definelabel EFE_DATA_STACK, 0x80134cc8
.definelabel EFE_FLASH_DATA, 0x80134cc0
.definelabel FLASH_INSTANCE, 0x80134cc4
.definelabel LOCAL_MAP_OBJECTS, 0x8013c060
.definelabel MAP_OBJECT_INSTANCE_COUNT, 0x80134d1c
.definelabel MAP_OBJECT_MOVE_TO_DATA, 0x8013c038

// BTL_REL
.definelabel BTL_battleMain, 0x8005b5f4

// ENDI_REL
.definelabel ENDI_tickEnding, 0x80060d00

// MURD_REL
.definelabel MURD_tick, 0x8007d628

// EAB_REL
.definelabel EaB_tick, 0x80061530

// DOOA_REL
.definelabel DoOA_tick, 0x80084644

// STD_REL
.definelabel STD_Tamer_tickTournament, 0x80069688
.definelabel STD_NPCEntity_tickTournament, 0x80069700

// KAR_REL
.definelabel KAR_tick, 0x800545c8

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
.definelabel libcd_CdControl, 0x800b0010
.definelabel libcd_CdRead, 0x800b2bb4
.definelabel libcd_CdReadSync, 0x800b2cb4
.definelabel libcd_CdPosToInt, 0x800b0554
.definelabel libcd_CdIntToPos, 0x800b0450

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
.definelabel libgs_GsClearOt, 0x80098838
.definelabel libgs_GsGetActiveBuffer, 0x80097af8
.definelabel libgs_GsSortSprite, 0x80096f90

.definelabel libgs_REFERENCE_MATRIX, 0x80136f84
.definelabel libgs_GsLIGHTWSMATRIX, 0x80136f24

// libgpu
.definelabel libgpu_StoreImage, 0x80094908
.definelabel libgpu_ClearImage, 0x80094818
.definelabel libgpu_LoadImage, 0x800948a8
.definelabel libgpu_LoadImage2, 0x80095290
.definelabel libgpu_AddPrim, 0x80092ad4
.definelabel libgpu_GetClut, 0x80092abc
.definelabel libgpu_SetPolyFT4, 0x80092b88
.definelabel libgpu_SetLineF2, 0x80092bc4
.definelabel libgpu_SetLineF3, 0x80092bd8
.definelabel libgpu_SetLineF4, 0x80092bf8
.definelabel libgpu_DrawSync, 0x800947b0
.definelabel libgpu_GetTPage, 0x80092a80
.definelabel libgpu_SetSemiTrans, 0x80092b10
.definelabel libgpu_OpenTIM, 0x80096bcc
.definelabel libgpu_ReadTIM, 0x80096bdc

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
.definelabel libgte_RotTransPers, 0x8009b3d4
.definelabel libgte_RotTransPers3, 0x8009b404
.definelabel libgte_RotTransPers4, 0x8009b464
.definelabel libgte_RotMatrixZYX, 0x8009bd24

.definelabel libgte_GsTMDdivTNF3, 0x8009de78
.definelabel libgte_GsTMDdivTF3NL, 0x8009dbc8
.definelabel libgte_GsTMDdivTNG4, 0x8009efc8
.definelabel libgte_GsTMDdivTF4L, 0x8009e598
.definelabel libgte_GsTMDdivTG4NL, 0x8009ed38
.definelabel libgte_GsTMDdivTNF4, 0x8009eab8
.definelabel libgte_GsTMDdivTF4NL, 0x8009e838
.definelabel libgte_GsTMDdivTNG3, 0x8009e358
.definelabel libgte_GsTMDdivTG3NL, 0x8009e128
.definelabel libgte_GsTMDfastTG4L, 0x8009d9bc
.definelabel libgte_GsTMDfastG4L, 0x8009c6d8
.definelabel libgte_GsTMDfastTNG4, 0x8009d668
.definelabel libgte_GsTMDfastTG3L, 0x8009d4b4
.definelabel libgte_GsTMDfastTF4L, 0x8009d034
.definelabel libgte_GsTMDfastTNF4, 0x8009ccf8
.definelabel libgte_GsTMDfastTF4NL, 0x8009ce98
.definelabel libgte_GsTMDfastTG4NL, 0x8009d824
.definelabel libgte_GsTMDfastG3L, 0x8009c548
.definelabel libgte_GsTMDfastTNG3, 0x8009d1e0
.definelabel libgte_GsTMDfastF4L, 0x8009c3d0
.definelabel libgte_GsTMDfastTF3L, 0x8009cb88
.definelabel libgte_GsTMDfastTNF3, 0x8009c8b8
.definelabel libgte_GsTMDfastF4NL, 0x8009c264
.definelabel libgte_GsTMDfastNF4, 0x8009c0fc
.definelabel libgte_GsTMDfastTF3NL, 0x8009ca24
.definelabel libgte_GsTMDfastTG3NL, 0x8009d358
.definelabel libgte_GsTMDfastF3L, 0x8009bfb4

.definelabel GS_TMD_MAP, 0x80137000

// PSX internal
.definelabel TIMER0, 0x1F801100
.definelabel TIMER1, 0x1F801110
.definelabel TIMER2, 0x1F801120
.definelabel SCRATCHPAD, 0x1F800000