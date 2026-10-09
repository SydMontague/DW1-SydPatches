.open "work/DIGIMON/SLUS_010.32",0x80090000
.psx

.org 0x800fe394
  jal getShopkeeperLine
.org 0x800ff728
  jal getShopkeeperLine

;.org 0x800fbe14
;  jal resolveMapHeadEntry
.org 0x800fe3a4
  jal resolveMapHeadEntry
.org 0x800ff738
  jal resolveMapHeadEntry

.org 0x80105d14
  jal openShop

;.org 0x800fc580
;  jal allocateItemMenuBox
;.org 0x800fc5b0
;  jal allocateItemMenuBox
;.org 0x800fcb98
;  jal allocateItemMenuBox
.org 0x8010b6b4
  jal allocateItemMenuBox
.org 0x8010ba44
  jal allocateItemMenuBox
.org 0x8010bb78
  jal allocateItemMenuBox
.org 0x8010bc80
  jal allocateItemMenuBox
.org 0x8010bca8
  jal allocateItemMenuBox
.org 0x8010bfdc
  jal allocateItemMenuBox
.org 0x8010c010
  jal allocateItemMenuBox
.org 0x8010c1a8
  jal allocateItemMenuBox
.org 0x8010c2fc
  jal allocateItemMenuBox
.org 0x8010c518
  jal allocateItemMenuBox
.org 0x8010c538
  jal allocateItemMenuBox

;.org 0x800fc650
;  jal destroyItemMenuBox
;.org 0x800fc658
;  jal destroyItemMenuBox
;.org 0x800fcbe4
;  jal destroyItemMenuBox
.org 0x8010b720
  jal destroyItemMenuBox
.org 0x8010ba94
  jal destroyItemMenuBox
.org 0x8010bb98
  jal destroyItemMenuBox
.org 0x8010bcdc
  jal destroyItemMenuBox
.org 0x8010bce4
  jal destroyItemMenuBox
.org 0x8010c044
  jal destroyItemMenuBox
.org 0x8010c04c
  jal destroyItemMenuBox
.org 0x8010c1e4
  jal destroyItemMenuBox
.org 0x8010c338
  jal destroyItemMenuBox
.org 0x8010c580
  jal destroyItemMenuBox
.org 0x8010c588
  jal destroyItemMenuBox

;.org 0x800fbc3c
;  jal showShopkeeperTextbox
;.org 0x800fc608
;  jal showShopkeeperTextbox
;.org 0x800fc630
;  jal showShopkeeperTextbox
;.org 0x800fc6c4
;  jal showShopkeeperTextbox
;.org 0x800fc708
;  jal showShopkeeperTextbox
;.org 0x800fc744
;  jal showShopkeeperTextbox
;.org 0x800fc758
;  jal showShopkeeperTextbox
;.org 0x800fc7b0
;  jal showShopkeeperTextbox
;.org 0x800fc7c8
;  jal showShopkeeperTextbox
;.org 0x800fc808
;  jal showShopkeeperTextbox
.org 0x8010b6d8
  jal showShopkeeperTextbox
.org 0x8010b700
  jal showShopkeeperTextbox
.org 0x8010b788
  jal showShopkeeperTextbox
.org 0x8010b7c4
  jal showShopkeeperTextbox
.org 0x8010b7d8
  jal showShopkeeperTextbox
.org 0x8010b820
  jal showShopkeeperTextbox
.org 0x8010b834
  jal showShopkeeperTextbox
.org 0x8010b890
  jal showShopkeeperTextbox
.org 0x8010b8c8
  jal showShopkeeperTextbox

;.org 0x800fc668
;  jal createShopBitsBox
.org 0x801040b8
  jal createShopBitsBox
.org 0x8010b730
  jal createShopBitsBox
.org 0x8010bcfc
  jal createShopBitsBox
.org 0x8010c348
  jal createShopBitsBox

;.org 0x800fab3c
;  jal getItemMenuFromType
;.org 0x800faef8
;  jal getItemMenuFromType
;.org 0x800fafb8
;  jal getItemMenuFromType
;.org 0x800fb9b8
;  jal getItemMenuFromType
;.org 0x800fca80
;  jal getItemMenuFromType
;.org 0x800fcfc8
;  jal getItemMenuFromType
;.org 0x800fd254
;  jal getItemMenuFromType
.org 0x80107144
  jal getItemMenuFromType
.org 0x80107e78
  jal getItemMenuFromType
.org 0x80108174
  jal getItemMenuFromType
.org 0x8010823c
  jal getItemMenuFromType

.org 0x80105ce4
  jal openDiscardItem

;.org 0x800fc6b4
;  jal createItemMenu
;.org 0x800fc6f8
;  jal createItemMenu
;.org 0x800fcc04
;  jal createItemMenu
.org 0x8010b778
  jal createItemMenu
.org 0x8010bd9c
  jal createItemMenu

;.org 0x800fc67c
;  jal showShopkeepSelection
.org 0x8010b744
  jal showShopkeepSelection

;.org 0x800fab48
;  jal isItemMenuBoxBusy
.org 0x800fdcf4
  jal isItemMenuBoxBusy
.org 0x80107e84
  jal isItemMenuBoxBusy
.org 0x80108344
  jal isItemMenuBoxBusy
.org 0x80108350
  jal isItemMenuBoxBusy
.org 0x801091ec
  jal isItemMenuBoxBusy
.org 0x80109500
  jal isItemMenuBoxBusy
.org 0x80109800
  jal isItemMenuBoxBusy

;.org 0x800fcb20
;  jal updateItemMenuStrings
;.org 0x800fd408
;  jal updateItemMenuStrings
;.org 0x800fd514
;  jal updateItemMenuStrings
.org 0x801071e4
  jal updateItemMenuStrings
.org 0x80107aa0
  jal updateItemMenuStrings
.org 0x80107c30
  jal updateItemMenuStrings
.org 0x80108664
  jal updateItemMenuStrings
.org 0x801086bc
  jal updateItemMenuStrings
.org 0x801092f8
  jal updateItemMenuStrings
.org 0x80109c0c
  jal updateItemMenuStrings

;.org 0x800fcb10
;  jal initItemMenuBox
.org 0x801071d4
  jal initItemMenuBox
.org 0x801076e8
  jal initItemMenuBox
.org 0x80107764
  jal initItemMenuBox
.org 0x80107c20
  jal initItemMenuBox
.org 0x80107ddc
  jal initItemMenuBox

;.org 0x800fac18
;  jal createItemMenuAmountBox
.org 0x80107f98
  jal createItemMenuAmountBox

;.org 0x800fac38
;  jal createSingleCardConfirmBox
.org 0x80107f3c
  jal createSingleCardConfirmBox

;.org 0x800facf8
;  jal itemMenuCursorTop
.org 0x8010801c
  jal itemMenuCursorTop
.org 0x80109368
  jal itemMenuCursorTop
.org 0x8010966c
  jal itemMenuCursorTop

;.org 0x800fad08
;  jal itemMenuCursorUp
.org 0x8010802c
  jal itemMenuCursorUp
.org 0x80108540
  jal itemMenuCursorUp
.org 0x8010937c
  jal itemMenuCursorUp
.org 0x80109680
  jal itemMenuCursorUp
.org 0x8010997c
  jal itemMenuCursorUp

;.org 0x800fad44
;  jal itemMenuCursorBottom
.org 0x80108068
  jal itemMenuCursorBottom
.org 0x801093b8
  jal itemMenuCursorBottom
.org 0x801096bc
  jal itemMenuCursorBottom

;.org 0x800fad54
;  jal itemMenuCursorDown
.org 0x80108078
  jal itemMenuCursorDown
.org 0x80108590
  jal itemMenuCursorDown
.org 0x801093cc
  jal itemMenuCursorDown
.org 0x801096d0
  jal itemMenuCursorDown
.org 0x801099d0
  jal itemMenuCursorDown

;.org 0x800fadbc
;  jal createItemMenuDescriptionBox
.org 0x801085e0
  jal createItemMenuDescriptionBox

;.org 0x800fae34
;  jal renderItemMenuSprite
;.org 0x800fae70
;  jal renderItemMenuSprite
;.org 0x800faea0
;  jal renderItemMenuSprite
;.org 0x800faec8
;  jal renderItemMenuSprite
;.org 0x800faef0
;  jal renderItemMenuSprite
.org 0x801080e4
  jal renderItemMenuSprite
.org 0x80108114
  jal renderItemMenuSprite
.org 0x80108144
  jal renderItemMenuSprite
.org 0x8010816c
  jal renderItemMenuSprite
.org 0x80108fb8
  jal renderItemMenuSprite
.org 0x80108ff4
  jal renderItemMenuSprite
.org 0x80109438
  jal renderItemMenuSprite
.org 0x80109458
  jal renderItemMenuSprite
.org 0x8010973c
  jal renderItemMenuSprite
.org 0x8010975c
  jal renderItemMenuSprite

;.org 0x800faf04
;  jal renderItemMenuScrollBar
.org 0x80108180
  jal renderItemMenuScrollBar
.org 0x80108ffc
  jal renderItemMenuScrollBar
.org 0x80109464
  jal renderItemMenuScrollBar
.org 0x80109768
  jal renderItemMenuScrollBar

.close
