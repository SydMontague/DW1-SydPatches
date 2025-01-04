.open "work/DIGIMON/SLUS_010.32",0x80090000
.psx



.org 0x800a4528
  li.u v0,RAISE_DATA+0x0C
  .skip 4
  li.l v0,RAISE_DATA+0x0C
.org 0x800a4594
  li v0,RAISE_DATA+0x09
.org 0x800a45b8
  li v0,RAISE_DATA+0x15
.org 0x800a480c
  li v1,RAISE_DATA+0x12
.org 0x800a489c
  li v0,RAISE_DATA+0x12
.org 0x800a4b2c
  li t4,RAISE_DATA
.org 0x800a4b74
  li t4,RAISE_DATA
.org 0x800a4bbc
  li t3,RAISE_DATA
.org 0x800a4c00
  li a1,RAISE_DATA
.org 0x800a4c74
  li v0,RAISE_DATA
.org 0x800a50b8
  li.u v0,RAISE_DATA+0x0C
  .skip 4
  li.l v0,RAISE_DATA+0x0C
.org 0x800a5158
  li.u v0,RAISE_DATA+0x15
  .skip 4
  li.l v0,RAISE_DATA+0x15
.org 0x800a53b4
  li.u v0,RAISE_DATA+0x15
  .skip 4
  li.l v0,RAISE_DATA+0x15
.org 0x800a5774
  li v0,RAISE_DATA+0x08
.org 0x800a5ccc
  li v0,RAISE_DATA+0x12
.org 0x800a6574
  li v0,RAISE_DATA+0x12
.org 0x800a66f0
  li v0,RAISE_DATA+0x0C
.org 0x800a6a8c
  li v1,RAISE_DATA+0x10
.org 0x800a6b20
  li v1,RAISE_DATA+0x10
.org 0x800a6bd4
  li v1,RAISE_DATA+0x10
.org 0x800a754c
  li v0,RAISE_DATA+0x11
.org 0x800a76c0
  li v0,RAISE_DATA+0x0A
.org 0x800a77d4
  li v0,RAISE_DATA+0x09
.org 0x800a7920
  li v0,RAISE_DATA+0x09
.org 0x800a7bf8
  li v0,RAISE_DATA+0x09
.org 0x800a7c98
  li v0,RAISE_DATA+0x11
.org 0x800a7cb4
  li v0,RAISE_DATA+0x09
.org 0x800a80b4
  li v0,RAISE_DATA+0x0C
.org 0x800a8124
  li v1,RAISE_DATA+0x10
.org 0x800a8210
  li v0,RAISE_DATA+0x0C
.org 0x800a8cd4
  li v0,RAISE_DATA+0x09
.org 0x800a8d50
  li v0,RAISE_DATA+0x0A
.org 0x800abef4
  li v0,RAISE_DATA+0x09

; Redundant, relocation dealt with in ItemEffects.cpp
;.org 0x800c432c
;  li a0,RAISE_DATA+0x11
;.org 0x800c5784
;  li v0,RAISE_DATA+0x0C
;.org 0x800c583c
;  li v0,RAISE_DATA+0x08

.org 0x800bbd1c
  li v0,RAISE_DATA+0x12
.org 0x800bbd44
  li v0,RAISE_DATA
.org 0x800bbcb4
  li v0,RAISE_DATA+0x12
.org 0x800bf040
  li v0,RAISE_DATA
.org 0x800d61e4
  li v0,RAISE_DATA+0x13
.org 0x80105354
  li v0,RAISE_DATA+0x08
.org 0x80105394
  li v0,RAISE_DATA+0x09

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