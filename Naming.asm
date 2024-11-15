.open "work/DIGIMON/SLUS_010.32",0x80090000
.psx

NAMING_BUFFER equ 0x801b1d1c
max_glyph_count equ 12
naming_buffer_length equ 20

// patch namingDeleteLast
.org 0x8010ab4c
.area 64
  li a0,NAMING_BUFFER
  jal jis_len
  nop
  beq v0,zero,@@_end
  li a0,NAMING_BUFFER
  jal jis_at_index
  addi a1,v0,-0x01
  sb zero,0x00(v0)
@@_end:
  li a0,NAMING_BUFFER
  jal jis_len
  nop
  sb v0,-0x6baa(gp)
@@_end2:
  .notice "Empty space left: " + (0x8010ab4c+64-.) + " bytes"
  .fill 0x8010ab4c+64-.
.endarea

// patch confirm handler for tickNamingBox
.org 0x8010a17c
.area 252
  ; s1 -> column
  ; v1 -> char page
  ; v0 -> column offset
  addu v0,v1,v0
  lw v0,0x00(v0)
  move a1,s1
  jal jis_at_index
  move a0,v0
  move s1,v0
  lbu v1,-0x6baa(gp)
  lbu a0,0x00(s1)
  bne v1,zero,@@_else2
  nop
  beq a0,0x20,@@_not_possible
  lbu a1,0x01(s1)
  bne a0,0x81,@@_else2
  nop
  beq a1,0x40,@@_not_possible
  nop
@@_else2:
  sgtu s0,a0,0x7F ; greater 0x7F -> two bytes
  li a0,NAMING_BUFFER
  jal jis_len
  addiu s0,s0,0x01
  bge v0,max_glyph_count,@@_not_possible
  addu s0,s0,v1
  blt s0,naming_buffer_length,@@_add_glyph
  nop
@@_not_possible:
  clear a0
  jal playSound
  li a1,0x0b
  b 0x8010a40c
  lw ra,0x1C(sp)
@@_add_glyph:
  lbu v1,-0x6baa(gp)
  nop
  addiu v1,v1,0x01
  seq a0,v1,max_glyph_count
  sub v1,v1,a0
  sb v1,-0x6baa(gp)
  li a0,NAMING_BUFFER
  jal jis_concat_glyph
  move a1,s1
  jal updateNamingPreview
  nop
  li a0,NAMING_BUFFER
  jal jis_len
  nop
  beq v0,max_glyph_count,@@_set_finish
  li a0,0x8001
  bne v1,naming_buffer_length-2,0x8010a278
  nop
@@_set_finish:
  sh a0,-0x6ba0(gp)
  .notice "Empty space left: " + (0x8010a17c+252-.) + " bytes"
  .fill 0x8010a17c+252-.
.endarea

; fix pad with spaces limit
.org 0x8010aaf4
li a1,max_glyph_count

; remove terminateNamingBuffer call
.org 0x8010a2d8
nop

; naming box width
.org 0x8010b410
li v0,0xA0
.org 0x8010b420
li v0,0xA0

; last glyph
.org 0x80109fd0
jal jis_len
.org 0x80109fd8
nop ; no division by 2 anymore
.org 0x80109fe4
li at,max_glyph_count

; crop name on registration (naive, needs better solution)
.org 0x801136a0
li a2,0x0d
.org 0x80113980
li a2,0x0d

; fix square not clearing the whole text area
.org 0x80100dac
b 0x80100ddc

.org 0x801306cc
.word NAMING_PAGE1_LEFT1
.word NAMING_PAGE1_LEFT2
.word NAMING_PAGE1_LEFT3
.word NAMING_PAGE1_LEFT4
.word NAMING_PAGE1_LEFT5
.word NAMING_PAGE1_LEFT6
.word NAMING_PAGE1_LEFT7
.word NAMING_PAGE1_LEFT8
.word NAMING_PAGE1_LEFT9

.org 0x80130750
.word NAMING_PAGE1_RIGHT1
.word NAMING_PAGE1_RIGHT2
.word NAMING_PAGE1_RIGHT3
.word NAMING_PAGE1_RIGHT4
.word NAMING_PAGE1_RIGHT5
.word NAMING_PAGE1_RIGHT6
.word NAMING_PAGE1_RIGHT7
.word NAMING_PAGE1_RIGHT8
.word NAMING_PAGE1_RIGHT9

; use non-bugged strncpy
.org 0x80112cf4
  j strncpy
  nop

;;; take savegame display names from save data instead of frame header
; increase stack size
.org 0x80112720
  addiu sp,sp,-0x550
.org 0x80112968
  addiu sp,sp,0x550
; data size read from memory card
.org 0x8011284c
  li v0,0x380
; data offset read from memory card
.org 0x80112864
  li a3,0x380
; new string offsets in read buffer
.org 0x80112800
  addiu s7,sp,0x50+0x67+0x280
  addiu s6,sp,0x50+0x38
; copy all 20 bytes of player name
.org 0x801128d0
  li a2,0x14
  .skip 4
  sb zero,0x17(v0)
; copy all 20 bytes of digimon type
.org 0x801128dc
li a1,DIGIMON_PARA
lbu v1,0x00(s6)
li a0,0x34
mult v1,a0
addiu a0,s1,0x18
mflo v1
add a1,a1,v1
jal strncpy
li a2,0x14
; terminate area name
.org 0x8011290c
  sb zero,0x2c(v0)
;;;
;;; adjust rendering, to put Digimon name on new line
; dont render name behind player
.org 0x8010fb3c
nop
.org 0x8010f050
nop
; render partner type below player name
.org 0x8010fb50
addiu a0,s0,0x18
.org 0x8010f080
addiu a0,v0,0x18
; draw more of the player name
.org 0x8010d974
li v0,0x80
.org 0x8010dcfc
li v0,0x80
; dont draw behind player name
.org 0x8010d9c4
nop
.org 0x8010dd48
nop
;;;

; TODO enlarge name boxes in tamer menu
; TODO check "is sick" dialogue
; TODO check arena
; TODO check speaker name
; TODO crop name on registration properly

.close
