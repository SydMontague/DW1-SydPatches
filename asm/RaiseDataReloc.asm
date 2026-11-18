.open "work/DIGIMON/SLUS_010.32",0x80090000
.psx

.close

.open "work/DIGIMON/TRN_REL.BIN",0x80088800
.psx

.org 0x80089e28
  li.u v0,RAISE_DATA+0x14
  .skip 4
  li.l v0,RAISE_DATA+0x14
.org 0x8008a554
  li v0,RAISE_DATA+0x09
.org 0x8008a5c0
  li v0,RAISE_DATA+0x0A
  
.close

.open "work/DIGIMON/TRN2_REL.BIN",0x80088800
.psx

.org 0x8008b780
  li.u v0,RAISE_DATA+0x14
  .skip 4
  li.l v0,RAISE_DATA+0x14
.org 0x8008bd00
  li v0,RAISE_DATA+0x09
.org 0x8008bd6c
  li v0,RAISE_DATA+0x0A

.close


.open "work/DIGIMON/EVL_REL.BIN",0x80060000
.psx

.org 0x8006376C
  li v0,RAISE_DATA+0x15

.close
