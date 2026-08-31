.open "work/DIGIMON/SLUS_010.32",0x80090000
.psx

; patches padWithSpaces function to deal with the case of currentLength being larger than targetLength
.org padWithSpaces
.area 64
  sll a1,a1,0x1     ; targetLength * 2
  b @@loopCondition
  li v1,0x20

@@loopBody:
  sb v1,0x00(a0)
  addiu a0,a0,0x01

@@loopCondition:
  slt v0,a2,a1
  bne v0,zero,@@loopBody
  addiu a2,a2,0x01
  jr r31
  move v0,a0
.endarea

; Changes the track names to be accurate and shorter
.org 0x8012f6d0
.area 1544
addString MUSIC_NATIVE_FOREST_DAY,"Native Forest Day"
addString MUSIC_NATIVE_FOREST_NIGHT,"Native Forest Night"
addString MUSIC_JUNGLE_DAY,"Tropical Jungle Day"
addString MUSIC_JUNGLE_NIGHT,"Tropical Jungle Night"
addString MUSIC_PANORAMA_DAY,"Mt. Panorama Day"
addString MUSIC_PANORAMA_NIGHT,"Mt. Panorama Night"
addString MUSIC_DRILL_TUNNEL,"Drill Tunnel"
addString MUSIC_OGREMON_THEME,"Ogremon Theme"
addString MUSIC_OVERDELL,"Overdell Cemetary"
addString MUSIC_CANYON_DAY,"Great Canyon Day"
addString MUSIC_CANYON_NIGHT,"Great Canyon Night"
addString MUSIC_MONOCHROMON,"Monochromone Shop"
addString MUSIC_OGRE_FORTRESS,"Ogre Fortress"
addString MUSIC_LAVA_CAVE,"Lave Cave"
addString MUSIC_MANSION,"Grey Lords Mansion"
addString MUSIC_UNDERGROUND_LAB,"Underground Lab"
addString MUSIC_SAVANNA_DAY,"Gear Savanna Day"
addString MUSIC_SAVANNA_NIGHT,"Gear Savanna Night"
addString MUSIC_LEOMON_THEME,"Leomon Theme"
addString MUSIC_AMIDA_FOREST_DAY,"Amida Forest Day"
addString MUSIC_AMIDA_FOREST_NIGHT,"Amida Forest Night"
addString MUSIC_GLACIAL_REGION_DAY,"Glacial Region Day"
addString MUSIC_GLACIAL_REGION_NIGHT,"Glacial Region Night"
addString MUSIC_SPEEDY_REGION_DAY,"Speedy Region Day"
addString MUSIC_SPEEDY_REGION_NIGHT,"Speedy Region Night"
addString MUSIC_FREEZELAND_DAY,"Freezeland Day"
addString MUSIC_FREEZELAND_NIGHT,"Freezeland Night"
addString MUSIC_IGLOO_THEME,"Igloo Theme"
addString MUSIC_CURLING_THEME,"Curling Theme"
addString MUSIC_ICE_SANCTUARY_ENTRANCE,"Ice Sanctuary Hall"
addString MUSIC_ICE_SANCTUARY_BASEMENT,"Ice Sanctuary Dungeon"
addString MUSIC_GEKO_SWAMP_DAY,"Gecko Swamp Day"
addString MUSIC_GEKO_SWAMP_NIGHT,"Gecko Swamp Night"
addString MUSIC_MISTY_TREES_DAY,"Misty Trees Day"
addString MUSIC_MISTY_TREES_NIGHT,"Misty Trees Night"
addString MUSIC_WARUMONZAEMON_THEME,"WaruMonzaemon Theme"
addString MUSIC_TOY_TOWN,"Toy Town"
addString MUSIC_FACTORIAL_TOWN_DAY,"Factorial Town Day"
addString MUSIC_FACTORIAL_TOWN_NIGHT,"Factorial Town Night"
addString MUSIC_SEWERS,"Sewers"
addString MUSIC_TRASH_MOUNTAIN_DAY,"Trash Mountain Day"
addString MUSIC_TRASH_MOUNTAIN_NIGHT,"Trash Mountain Night"
addString MUSIC_BEATLELAND_DAY,"Beatleland Day"
addString MUSIC_BEATLELAND_NIGHT,"Beatleland Night"
addString MUSIC_SECRET_BEACH_CAVE,"Secret Beach Cave"
addString MUSIC_MT_INFINITY,"Mt. Infinity"
addString MUSIC_ANALOGMAN_THEME,"Analogman Theme"
addString MUSIC_FILE_CITY_DAY,"File City Day"
addString MUSIC_FILE_CITY_NIGHT,"File City Night"
addString MUSIC_TOURNAMENT_OPENING,"Tournament Opening"
addString MUSIC_TOURNAMENT_PROGRESS,"Tournament Progress"
addString MUSIC_TOURNAMENT_CHAMPION,"Tournament Champion"
addString MUSIC_PARTNER_ENTRY_THEME,"Partner Entry Theme"
addString MUSIC_ENEMY_ENTRY_THEME,"Opponent Entry Theme"
addString MUSIC_TOURNAMENT_BATTLE_1,"Tournament Battle 1"
addString MUSIC_TOURNAMENT_WIN,"Tournament Win"
addString MUSIC_TOURNAMENT_LOSE,"Tournament Lose"
addString MUSIC_TOURNAMENT_BATTLE_2,"Tournament Battle 2"
addString MUSIC_TOURNAMENT_BATTLE_3,"Tournament Battle 3"
addString MUSIC_MAJOR_BATTLE,"Major Battle"
addString MUSIC_NORMAL_BATTLE_1,"Normal Battle 1"
addString MUSIC_NORMAL_BATTLE_2,"Normal Battle 2"
addString MUSIC_MACHINEDRAMON_THEME,"Machinedramon Theme"

.notice "Empty space left: " + (1544+0x8012f6d0-.) + " bytes"
.fill 1544+0x8012f6d0-.
.endarea

.org 0x8012fec8;
.area 63 * 4
.word MUSIC_NATIVE_FOREST_DAY
.word MUSIC_NATIVE_FOREST_NIGHT
.word MUSIC_JUNGLE_DAY
.word MUSIC_JUNGLE_NIGHT
.word MUSIC_PANORAMA_DAY
.word MUSIC_PANORAMA_NIGHT
.word MUSIC_DRILL_TUNNEL
.word MUSIC_OGREMON_THEME
.word MUSIC_OVERDELL
.word MUSIC_CANYON_DAY
.word MUSIC_CANYON_NIGHT
.word MUSIC_MONOCHROMON
.word MUSIC_OGRE_FORTRESS
.word MUSIC_LAVA_CAVE
.word MUSIC_MANSION
.word MUSIC_UNDERGROUND_LAB
.word MUSIC_SAVANNA_DAY
.word MUSIC_SAVANNA_NIGHT
.word MUSIC_LEOMON_THEME
.word MUSIC_AMIDA_FOREST_DAY
.word MUSIC_AMIDA_FOREST_NIGHT
.word MUSIC_GLACIAL_REGION_DAY
.word MUSIC_GLACIAL_REGION_NIGHT
.word MUSIC_SPEEDY_REGION_DAY
.word MUSIC_SPEEDY_REGION_NIGHT
.word MUSIC_FREEZELAND_DAY
.word MUSIC_FREEZELAND_NIGHT
.word MUSIC_IGLOO_THEME
.word MUSIC_CURLING_THEME
.word MUSIC_ICE_SANCTUARY_ENTRANCE
.word MUSIC_ICE_SANCTUARY_BASEMENT
.word MUSIC_GEKO_SWAMP_DAY
.word MUSIC_GEKO_SWAMP_NIGHT
.word MUSIC_MISTY_TREES_DAY
.word MUSIC_MISTY_TREES_NIGHT
.word MUSIC_WARUMONZAEMON_THEME
.word MUSIC_TOY_TOWN
.word MUSIC_FACTORIAL_TOWN_DAY
.word MUSIC_FACTORIAL_TOWN_NIGHT
.word MUSIC_SEWERS
.word MUSIC_TRASH_MOUNTAIN_DAY
.word MUSIC_TRASH_MOUNTAIN_NIGHT
.word MUSIC_BEATLELAND_DAY
.word MUSIC_BEATLELAND_NIGHT
.word MUSIC_SECRET_BEACH_CAVE
.word MUSIC_MT_INFINITY
.word MUSIC_ANALOGMAN_THEME
.word MUSIC_FILE_CITY_DAY
.word MUSIC_FILE_CITY_NIGHT
.word MUSIC_TOURNAMENT_OPENING
.word MUSIC_TOURNAMENT_PROGRESS
.word MUSIC_TOURNAMENT_CHAMPION
.word MUSIC_PARTNER_ENTRY_THEME
.word MUSIC_ENEMY_ENTRY_THEME
.word MUSIC_TOURNAMENT_BATTLE_1
.word MUSIC_TOURNAMENT_WIN
.word MUSIC_TOURNAMENT_LOSE
.word MUSIC_TOURNAMENT_BATTLE_2
.word MUSIC_TOURNAMENT_BATTLE_3
.word MUSIC_MAJOR_BATTLE
.word MUSIC_NORMAL_BATTLE_1
.word MUSIC_NORMAL_BATTLE_2
.word MUSIC_MACHINEDRAMON_THEME
.endarea

.close