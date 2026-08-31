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
