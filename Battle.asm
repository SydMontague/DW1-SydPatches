.open "work/DIGIMON/SLUS_010.32",0x80090000
.psx

.org 0x80104618
  jal startBattle

.org 0x800f3e2c
  jal handleBattleIdle
.org 0x800f4d48
  jal handleBattleIdle
.org 0x800f4d5c
  jal handleBattleIdle
.org 0x800f4e94
  jal handleBattleIdle
.org 0x800f4f64
  jal handleBattleIdle
.org 0x800f53a0
  jal handleBattleIdle
.org 0x800f5484
  jal handleBattleIdle
.org 0x800f558c
  jal handleBattleIdle
.org 0x800f5648
  jal handleBattleIdle
.org 0x800f56bc
  jal handleBattleIdle
.org 0x800f5700
  jal handleBattleIdle
.org 0x800f5774
  jal handleBattleIdle
.org 0x800f5814
  jal handleBattleIdle
.org 0x800f5940
  jal handleBattleIdle
.org 0x800f5a2c
  jal handleBattleIdle
.org 0x800f5b18
  jal handleBattleIdle
.org 0x800f5bd8
  jal handleBattleIdle
.org 0x800f5cb4
  jal handleBattleIdle
.org 0x800f5db4
  jal handleBattleIdle
.org 0x800f6030
  jal handleBattleIdle
.org 0x800f61dc
  jal handleBattleIdle
.org 0x800f6b24
  jal handleBattleIdle

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

.org 0x80057d8c
  jal handleBattleIdle
.org 0x800585a4
  jal handleBattleIdle
.org 0x800585f4
  jal handleBattleIdle
.org 0x800596ac
  jal handleBattleIdle
.org 0x80059790
  jal handleBattleIdle
.org 0x80059890
  jal handleBattleIdle
.org 0x80059944
  jal handleBattleIdle
.org 0x80059a08
  jal handleBattleIdle
.org 0x80059a7c
  jal handleBattleIdle
.org 0x80059b1c
  jal handleBattleIdle
.org 0x80059c48
  jal handleBattleIdle
.org 0x80059d34
  jal handleBattleIdle
.org 0x80059e20
  jal handleBattleIdle
.org 0x80059f50
  jal handleBattleIdle
.org 0x8005a02c
  jal handleBattleIdle
.org 0x8005a12c
  jal handleBattleIdle
.org 0x8005a528
  jal handleBattleIdle
.org 0x8005aea0
  jal handleBattleIdle

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

.org 0x8005f848
  jal handleBattleIdle
.org 0x8006003c
  jal handleBattleIdle
.org 0x8006008c
  jal handleBattleIdle
.org 0x80060ca0
  jal handleBattleIdle
.org 0x80060cb4
  jal handleBattleIdle
.org 0x80060dd4
  jal handleBattleIdle
.org 0x8006130c
  jal handleBattleIdle
.org 0x800613f0
  jal handleBattleIdle
.org 0x800614f0
  jal handleBattleIdle
.org 0x800615a4
  jal handleBattleIdle
.org 0x80061618
  jal handleBattleIdle
.org 0x8006165c
  jal handleBattleIdle
.org 0x800616d0
  jal handleBattleIdle
.org 0x80061770
  jal handleBattleIdle
.org 0x8006189c
  jal handleBattleIdle
.org 0x80061988
  jal handleBattleIdle
.org 0x80061a74
  jal handleBattleIdle
.org 0x80061b18
  jal handleBattleIdle
.org 0x80061bf4
  jal handleBattleIdle
.org 0x80061cf4
  jal handleBattleIdle
.org 0x80061f7c
  jal handleBattleIdle
.org 0x80062128
  jal handleBattleIdle
.org 0x80062aa0
  jal handleBattleIdle

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
