.macro addString,name,string
  .align 4
  name:
  .asciiz string
.endmacro

.macro addStringSJIS,name,string
  .align 4
  name:
  .sjis string
.endmacro

.macro buildLine,x1,y1,x2,y2,clut
  .halfword x1
  .halfword y1
  .halfword x2
  .halfword y2
  .byte clut
  .byte 0
.endmacro