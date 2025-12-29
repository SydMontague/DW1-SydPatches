.open "work/DIGIMON/SLUS_010.32",0x80090000
.psx

.org 0x80104618
  jal startBattle

.close


.open "work/DIGIMON/BTL_REL.BIN",0x80052ae0
.psx

.org 0x8005d1e8
  jal damageTick
.org 0x80064730
  jal damageTick

.org 0x80063140
  jal swapByte

.org 0x8005f978
  jal swapInt
.org 0x8005f984
  jal swapInt
.org 0x8005fa70
  jal swapInt
.org 0x8005fa7c
  jal swapInt

.close


.open "work/DIGIMON/STD_REL.BIN",0x80052ae0
.psx

.org 0x8006ae60
  jal damageTick

.org 0x8005737c
  jal swapByte
.org 0x80057420
  jal swapByte
.org 0x8005da58
  jal swapByte
.org 0x80069764
  jal swapByte
.org 0x8006c754
  jal swapByte

.org 0x8005d73c
  jal swapShort
.org 0x8005d748
  jal swapShort
.org 0x8005e318
  jal swapShort
.org 0x8005e324
  jal swapShort
.org 0x8005e46c
  jal swapShort
.org 0x8005e478
  jal swapShort

.org 0x800671ac
  jal swapInt
.org 0x800671b8
  jal swapInt
.org 0x800672a4
  jal swapInt
.org 0x800672b0
  jal swapInt

.close


.open "work/DIGIMON/VS_REL.BIN",0x80052ae0
.psx

.org 0x800621f4
  jal damageTick


.org 0x8005eb38
  jal swapInt
.org 0x8005eb44
  jal swapInt
.org 0x8005ec30
  jal swapInt
.org 0x8005ec3c
  jal swapInt
  
.org 0x80060a3c
  jal swapByte

.close
