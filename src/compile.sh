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
"$MIPS_CXX" MenuTab.cpp ConditionBubble.cpp VanillaText.cpp Fishing.cpp Matrix.cpp Utils.cpp Files.cpp EFE.cpp MapObjects.cpp Partner.cpp DOOA/DOOA.cpp CombatCommon.cpp Inventory.cpp Sound.cpp Math.cpp Camera.cpp Battle.cpp Tournament.cpp DigimonData.cpp Transformation.cpp Evolution.cpp DigimonMenu.cpp -o ../compiled/Cave3.lib $FLAGS
"$MIPS_CXX" Butterfly.cpp -o ../compiled/Cave4.lib $FLAGS
"$MIPS_CXX" Textbox.cpp UIBox.cpp AtlasFont.cpp BuffModel.cpp ThrownItem.cpp Main.cpp BattleEndBox.cpp VS/Intro.cpp VS/InitVS.cpp VS/DigimonAI.cpp VS/TimeoutWindow.cpp VS/SelectDigimon.cpp VS/SelectMapMode.cpp ItemMenu.cpp MonochromonMoodBubble.cpp Misc.cpp RecycleShop.cpp BitsBox.cpp ItemMenuAmountBox.cpp SingleCardConfirmMenu.cpp ItemDescriptionBox.cpp Script.cpp GameState.cpp VanillaTextbox.cpp ScriptEngine.cpp -o ../compiled/Cave5.lib $FLAGS
"$MIPS_CXX" DebugMenu.cpp -o ../compiled/Cave6.lib $FLAGS
"$MIPS_CXX" \
    KAR/CurlingAimScroll.cpp \
    KAR/CurlingAiming.cpp \
    KAR/CurlingBounce.cpp \
    KAR/CurlingCollision.cpp \
    KAR/CurlingCollisionFlow.cpp \
    KAR/CurlingGeometry.cpp \
    KAR/CurlingHints.cpp \
    KAR/CurlingHud.cpp \
    KAR/CurlingMatch.cpp \
    KAR/CurlingMatchState.cpp \
    KAR/CurlingMovement.cpp \
    KAR/CurlingOpponent.cpp \
    KAR/CurlingOpponentShot.cpp \
    KAR/CurlingPhysics.cpp \
    KAR/CurlingRender.cpp \
    KAR/CurlingRingMarkers.cpp \
    KAR/CurlingScoring.cpp \
    KAR/CurlingWalls.cpp \
    -o ../compiled/KAR.lib $FLAGS

"$MIPS_CXX" MOV/Movie.cpp -o ../compiled/MOV.lib $FLAGS

# Native entries use the three disjoint operand regions in each owning overlay.
"$MIPS_CXX" BTL/EFEOperands.cpp -o ../compiled/BTLEFEOperands.lib $FLAGS
"$MIPS_CXX" BTL/EFEIndexedOperand.cpp -o ../compiled/BTLEFEIndexedOperand.lib $FLAGS
"$MIPS_CXX" BTL/EFEAddressOperand.cpp -o ../compiled/BTLEFEAddressOperand.lib $FLAGS
"$MIPS_CXX" STD/EFEOperands.cpp -o ../compiled/STDEFEOperands.lib $FLAGS
"$MIPS_CXX" STD/EFEIndexedOperand.cpp -o ../compiled/STDEFEIndexedOperand.lib $FLAGS
"$MIPS_CXX" STD/EFEAddressOperand.cpp -o ../compiled/STDEFEAddressOperand.lib $FLAGS
"$MIPS_CXX" VS/EFEOperands.cpp -o ../compiled/VSEFEOperands.lib $FLAGS
"$MIPS_CXX" VS/EFEIndexedOperand.cpp -o ../compiled/VSEFEIndexedOperand.lib $FLAGS
"$MIPS_CXX" VS/EFEAddressOperand.cpp -o ../compiled/VSEFEAddressOperand.lib $FLAGS

# Shared control source is emitted into each overlay's disjoint owned regions.
"$MIPS_CXX" BTL/EFEDispatch.cpp -o ../compiled/BTLEFEDispatch.lib $FLAGS
"$MIPS_CXX" BTL/EFEControl.cpp -o ../compiled/BTLEFEControl.lib $FLAGS
"$MIPS_CXX" BTL/EFERun.cpp -o ../compiled/BTLEFERun.lib $FLAGS
"$MIPS_CXX" STD/EFEDispatch.cpp -o ../compiled/STDEFEDispatch.lib $FLAGS
"$MIPS_CXX" STD/EFEControl.cpp -o ../compiled/STDEFEControl.lib $FLAGS
"$MIPS_CXX" STD/EFERun.cpp -o ../compiled/STDEFERun.lib $FLAGS
"$MIPS_CXX" VS/EFEDispatch.cpp -o ../compiled/VSEFEDispatch.lib $FLAGS
"$MIPS_CXX" VS/EFEControl.cpp -o ../compiled/VSEFEControl.lib $FLAGS
"$MIPS_CXX" VS/EFERun.cpp -o ../compiled/VSEFERun.lib $FLAGS

# Shared EFE source; each overlay owns one emitted copy and its borrowed pointer.
for mode in BTL STD VS; do
    "$MIPS_CXX" EFE/Particles.cpp -o "../compiled/${mode}EFEParticles.lib" $FLAGS \
        -DEFE_initializeParticleEmitters=${mode}_initializeParticleEmitters \
        -DEFE_addParticleEmitter=${mode}_addParticleEmitter \
        -DEFE_tickParticleEmitters=${mode}_tickParticleEmitters \
        -DEFE_renderParticleEmitters=${mode}_renderParticleEmitters
done

cd -
