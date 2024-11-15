.macro addString,name,string
  .align 4
  name:
  .asciiz string
.endmacro