#!/bin/bash

cd "$(dirname "$0")"
mkdir ../compiled/ -p

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

mips-g++ UIElements.cpp -o ../compiled/utils.lib $FLAGS
mips-g++ Font.cpp Font5px.cpp Font7px.cpp -o ../compiled/font.lib $FLAGS
mips-g++ CustomUI.cpp -o ../compiled/CustomUI.lib $FLAGS
mips-g++ GameData.cpp -o ../compiled/GameData.lib $FLAGS
mips-g++ MapData.cpp -o ../compiled/MapData.lib $FLAGS
mips-g++ ItemInfo.cpp -o ../compiled/ItemInfo.lib $FLAGS
mips-g++ ItemInfoDataA.cpp -o ../compiled/ItemInfoDataA.lib $FLAGS
mips-g++ ItemInfoDataB.cpp -o ../compiled/ItemInfoDataB.lib $FLAGS

mips-g++ Input.cpp GameTime.cpp FixedNumbers.cpp Fade.cpp GameObjects.cpp NPCEntity.cpp Entity.cpp Tamer.cpp Effects.cpp HealingParticles.cpp ParticleFX.cpp CloudFX.cpp Fishing.cpp UIBox.cpp -o ../compiled/Cave1.lib $FLAGS
mips-g++ InventoryUI.cpp -o ../compiled/InventoryUI.lib ${FLAGS/-O3/-Os}
mips-g++ NinjamonEffect.cpp MapName.cpp Map.cpp Model.cpp ItemEffects.cpp ItemFunctions.cpp GameMenu.cpp StatsView.cpp TechView.cpp PlayerInfoView.cpp PlayerCardView.cpp PlayerChartView.cpp PlayerMedalView.cpp Inventory.cpp -o ../compiled/Cave2.lib $FLAGS
mips-g++ PlayerMenu.cpp -o ../compiled/PlayerMenu.lib ${FLAGS/-O3/-Os}
mips-g++ MenuTab.cpp ConditionBubble.cpp VanillaText.cpp Matrix.cpp Utils.cpp Files.cpp EFE.cpp MapObjects.cpp Partner.cpp DOOA/DOOA.cpp CombatCommon.cpp InventoryUIHelpers.cpp Sound.cpp Math.cpp Camera.cpp Battle.cpp Tournament.cpp DigimonData.cpp Transformation.cpp Evolution.cpp -o ../compiled/Cave3.lib $FLAGS
mips-g++ DigimonMenu.cpp -o ../compiled/DigimonMenu.lib ${FLAGS/-O3/-Os}
mips-g++ Butterfly.cpp -o ../compiled/Cave4.lib $FLAGS
mips-g++ AtlasFont.cpp BuffModel.cpp Main.cpp MeramonShake.cpp EntityParticleFX.cpp -o ../compiled/Cave5.lib $FLAGS
mips-g++ BattleEndBox.cpp -o ../compiled/BattleEndBox.lib ${FLAGS/-O3/-Os}
mips-g++ DebugMenu.cpp Script.cpp -o ../compiled/Cave6.lib $FLAGS

# Spillover libs placed in memory-cave holes (see GameData.asm).
mips-g++ Pause.cpp      -o ../compiled/Pause.lib      $FLAGS
mips-g++ dw1.cpp        -o ../compiled/dw1.lib        $FLAGS
mips-g++ Helper.cpp     -o ../compiled/Helper.lib     $FLAGS
mips-g++ Timestamp.cpp  -o ../compiled/Timestamp.lib  $FLAGS
mips-g++ ThrownItem.cpp -o ../compiled/ThrownItem.lib $FLAGS

cd -