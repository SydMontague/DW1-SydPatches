#!/bin/bash
set -euo pipefail
source "$(dirname "$0")/../tools/build-env.sh"
require_tool "$MIPS_CXX"

cd "$(dirname "$0")"
mkdir -p ../compiled/

# TODO: evaluate -flto

# Flags explanation:
# -O3                               -> highest optimization level
# -std=c++23                        -> use C++23
# -T linker.ld                      -> custom linker script, merged .bss into .data section
# -mabi=32                          -> 32 bit ABI
# -march=r3000                      -> target R3000 CPU, used in PSX
# -r                                -> relocateable output/partial linking
# -mel                              -> little endian
# -nostdlib                         -> don't use stdlib, since we don't have one on PSX
# -mexplicit-relocs                 -> use explicit relocations, required by -mno-gpopt
# -mno-shared                       -> don't use position independent code, might be useless?
# -fno-zero-initialized-in-bss      -> prevent the use of GP relative memory and memory sections that are not inserted
# -mno-gpopt                        -> same as above
# -fno-inline-functions             -> disable automatic inlining of functions, to save space and guarantee the symbols
# -fno-inline-small-functions          use 'inline' or 'constexpr' keywords to mark functions for explicit inline
# -msoft-float                      -> we don't have a floating point unit, will cause linking errors when using floats
# -fno-exceptions                   -> we don't have exceptions
# -mno-check-zero-division          -> don't emit trap instructions, saves space
# -Wno-builtin-declaration-mismatch -> don't warn about custom implemented standard functions
# -fno-use-cxa-atexit               -> don't use __cxa_atexit(), we don't have it and never properly exit on PS1
FLAGS="-O3 -std=c++23 -T linker.ld -mabi=32 -march=r3000 -r -mel -nostdlib -mexplicit-relocs -mno-shared -fno-zero-initialized-in-bss -mno-gpopt -fno-inline-functions -msoft-float -fno-inline-small-functions -fno-exceptions -mno-check-zero-division -Wno-builtin-declaration-mismatch -fno-use-cxa-atexit"

"$MIPS_CXX" UIElements.cpp -o ../compiled/utils.lib $FLAGS
"$MIPS_CXX" Font.cpp Font5px.cpp Font7px.cpp -o ../compiled/font.lib $FLAGS
"$MIPS_CXX" CustomUI.cpp -o ../compiled/CustomUI.lib $FLAGS
"$MIPS_CXX" GameData.cpp -o ../compiled/GameData.lib $FLAGS
"$MIPS_CXX" MapData.cpp -o ../compiled/MapData.lib $FLAGS

"$MIPS_CXX" Pause.cpp Input.cpp dw1.cpp GameTime.cpp InventoryUI.cpp Timestamp.cpp FixedNumbers.cpp Fade.cpp GameObjects.cpp Helper.cpp NPCEntity.cpp Entity.cpp Tamer.cpp Effects.cpp HealingParticles.cpp CloudFX.cpp ParticleFX.cpp EntityParticleFX.cpp MeramonShake.cpp -o ../compiled/Cave1.lib $FLAGS
"$MIPS_CXX" NinjamonEffect.cpp MapName.cpp Map.cpp Model.cpp ItemEffects.cpp ItemFunctions.cpp GameMenu.cpp PlayerMenu.cpp StatsView.cpp TechView.cpp PlayerInfoView.cpp PlayerChartView.cpp PlayerMedalView.cpp PlayerCardView.cpp -o ../compiled/Cave2.lib $FLAGS
"$MIPS_CXX" MenuTab.cpp ConditionBubble.cpp VanillaText.cpp Fishing.cpp Matrix.cpp Utils.cpp Files.cpp EFE.cpp MapObjects.cpp Script.cpp Partner.cpp DOOA/DOOA.cpp CombatCommon.cpp Inventory.cpp Sound.cpp Math.cpp Camera.cpp Battle.cpp Tournament.cpp DigimonData.cpp Transformation.cpp Evolution.cpp DigimonMenu.cpp -o ../compiled/Cave3.lib $FLAGS
"$MIPS_CXX" Butterfly.cpp -o ../compiled/Cave4.lib $FLAGS
"$MIPS_CXX" UIBox.cpp AtlasFont.cpp BuffModel.cpp ThrownItem.cpp Main.cpp BattleEndBox.cpp VS/Intro.cpp VS/InitVS.cpp VS/DigimonAI.cpp VS/TimeoutWindow.cpp VS/SelectDigimon.cpp VS/SelectMapMode.cpp ItemMenu.cpp MonochromonMoodBubble.cpp Misc.cpp RecycleShop.cpp BitsBox.cpp ItemMenuAmountBox.cpp SingleCardConfirmMenu.cpp ItemDescriptionBox.cpp -o ../compiled/Cave5.lib $FLAGS
"$MIPS_CXX" DebugMenu.cpp -o ../compiled/Cave6.lib $FLAGS
"$MIPS_CXX" KAR/Curling.cpp -o ../compiled/KAR.lib $FLAGS
"$MIPS_CXX" KAR/CurlingCollision.cpp -o ../compiled/KARCollision.lib $FLAGS
"$MIPS_CXX" KAR/CurlingCollisionFlow.cpp -o ../compiled/KARCollisionFlow.lib $FLAGS
"$MIPS_CXX" KAR/CurlingWalls.cpp -o ../compiled/KARWalls.lib $FLAGS
"$MIPS_CXX" KAR/CurlingBounce.cpp -o ../compiled/KARBounce.lib $FLAGS
"$MIPS_CXX" KAR/CurlingGeometry.cpp -o ../compiled/KARGeometry.lib $FLAGS
"$MIPS_CXX" KAR/CurlingMovement.cpp -o ../compiled/KARMovement.lib $FLAGS
"$MIPS_CXX" KAR/CurlingPhysics.cpp -o ../compiled/KARPhysics.lib $FLAGS
"$MIPS_CXX" KAR/CurlingAimScroll.cpp -o ../compiled/KARAimScroll.lib $FLAGS
"$MIPS_CXX" KAR/CurlingAiming.cpp -o ../compiled/KARAiming.lib $FLAGS
"$MIPS_CXX" KAR/CurlingRingMarkers.cpp -o ../compiled/KARRingMarkers.lib $FLAGS
"$MIPS_CXX" KAR/CurlingScoring.cpp -o ../compiled/KARScoring.lib $FLAGS
"$MIPS_CXX" KAR/CurlingOpponent.cpp -o ../compiled/KAROpponent.lib $FLAGS
"$MIPS_CXX" KAR/CurlingOpponentShot.cpp -o ../compiled/KAROpponentShot.lib $FLAGS

"$MIPS_CXX" KAR/CurlingOrdering.cpp -o ../compiled/KAROrdering.lib $FLAGS
"$MIPS_CXX" KAR/CurlingRender.cpp -o ../compiled/KARRender.lib $FLAGS
"$MIPS_CXX" KAR/CurlingHud.cpp -o ../compiled/KARHud.lib $FLAGS
"$MIPS_CXX" KAR/CurlingHints.cpp -o ../compiled/KARHints.lib $FLAGS
"$MIPS_CXX" KAR/CurlingHintPages.cpp -o ../compiled/KARHintPages.lib $FLAGS
"$MIPS_CXX" KAR/CurlingSprite.cpp -o ../compiled/KARSprite.lib $FLAGS

cd -
