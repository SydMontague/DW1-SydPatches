.open "work/DIGIMON/VS_REL.BIN",0x80052ae0
.psx

; Shared EFE source is emitted only in this owning overlay.
.org 0x80063238
.area 0x800635c4-.
  .importobj "compiled/VSEFEFrame.lib"
  .notice "VS EFE tick/render space left: " + (0x800635c4-.) + " bytes"
  .fill 0x800635c4-.
.endarea

; Keep the vanilla sound clear function before the replacement stop operation.
.org 0x80063600
.area 0x80063664-.
  .importobj "compiled/VSEFESound.lib"
  .notice "VS EFE stop sounds space left: " + (0x80063664-.) + " bytes"
  .fill 0x80063664-.
.endarea

.org 0x80063664
.area 0x80063fb8-.
  .importobj "compiled/VSEFELifecycle.lib"
  .notice "VS EFE lifecycle space left: " + (0x80063fb8-.) + " bytes"
  .fill 0x80063fb8-.
.endarea

.org 0x800640c8
.area 0x80064628-.
  .importobj "compiled/VSEFEParticles.lib"
  .notice "VS reclaimed EFE particles space left: " + (0x80064628-.) + " bytes"
  .fill 0x80064628-.
.endarea

.org 0x80064814
.area 0x80064950-.
  .notice "VS EFE render space left: " + (0x80064950-.) + " bytes"
  .fill 0x80064950-.
.endarea

.org 0x800649cc
.area 0x80064a54-.
  .importobj "compiled/VSEFESlotScript.lib"
  .notice "VS EFE slot script space left: " + (0x80064a54-.) + " bytes"
  .fill 0x80064a54-.
.endarea

.org 0x80064bbc
.area 0x80064c08-.
  .notice "VS EFE retired isTargetUnhit space left: " + (0x80064c08-.) + " bytes"
  .fill 0x80064c08-.
.endarea

.org 0x80064c6c
.area 0x800653a8-.
  .importobj "compiled/VSEFEAttackCollision.lib"
  .notice "VS EFE attack and collision space left: " + (0x800653a8-.) + " bytes"
  .fill 0x800653a8-.
.endarea

.org 0x80067170
.area 0x80067294-.
  .importobj "compiled/VSEFEEntityQueries.lib"
  .notice "VS EFE entity queries space left: " + (0x80067294-.) + " bytes"
  .fill 0x80067294-.
.endarea

.org 0x800673c8
.area 0x80067500-.
  .notice "VS retired EFE scalar vector space left: " + (0x80067500-.) + " bytes"
  .fill 0x80067500-.
.endarea

.org 0x80068d64
.area 0x80069048-.
  .importobj "compiled/VSEFEVectorArithmetic.lib"
  .notice "VS EFE vector arithmetic space left: " + (0x80069048-.) + " bytes"
  .fill 0x80069048-.
.endarea

.org 0x800690a0
.area 0x800690c8-.
  .notice "VS EFE retired setTargetToHitEntity space left: " + (0x800690c8-.) + " bytes"
  .fill 0x800690c8-.
.endarea

.org 0x80069168
.area 0x800692bc-.
  .importobj "compiled/VSEFEHitEntity.lib"
  .notice "VS EFE hit entity queries space left: " + (0x800692bc-.) + " bytes"
  .fill 0x800692bc-.
.endarea

.org 0x80069584
.area 0x800695fc-.
  .notice "VS EFE retired getSourceDigimonSize space left: " + (0x800695fc-.) + " bytes"
  .fill 0x800695fc-.
.endarea

.org 0x80069a90
.area 0x80069c18-.
  .importobj "compiled/VSEFETargetCollision.lib"
  .notice "VS EFE target collision space left: " + (0x80069c18-.) + " bytes"
  .fill 0x80069c18-.
.endarea

.org 0x8006ab40
.area 0x8006abb8-.
  .notice "VS EFE retired getTargetDigimonSize space left: " + (0x8006abb8-.) + " bytes"
  .fill 0x8006abb8-.
.endarea

.org 0x8006b0a4
.area 0x8006b2fc-.
  .importobj "compiled/VSEFETargetSelection.lib"
  .notice "VS EFE target selection and slot release space left: " + (0x8006b2fc-.) + " bytes"
  .fill 0x8006b2fc-.
.endarea

.org 0x8006b8b8
.area 0x8006b9e4-.
  .notice "VS retired EFE default and target attacks space left: " + (0x8006b9e4-.) + " bytes"
  .fill 0x8006b9e4-.
.endarea

.org 0x8006be5c
.area 0x8006bea8-.
  .notice "VS retired EFE subeffect setup space left: " + (0x8006bea8-.) + " bytes"
  .fill 0x8006bea8-.
.endarea

.org 0x8006c040
.area 0x8006c1bc-.
  .importobj "compiled/VSEFESubEffect.lib"
  .notice "VS EFE subeffects space left: " + (0x8006c1bc-.) + " bytes"
  .fill 0x8006c1bc-.
.endarea

.org 0x8006c1bc
.area 0x8006cd28-.
  .importobj "compiled/VSEFEAddressOperand.lib"
  .importobj "compiled/VSEFEDispatch.lib"
  .importobj "compiled/VSEFEIndexedOperand.lib"
  .importobj "compiled/VSEFEControl.lib"
  .importobj "compiled/VSEFEOperands.lib"
  .importobj "compiled/VSEFEArithmetic.lib"
  .notice "VS EFE operands, control and arithmetic space left: " + (0x8006cd28-.) + " bytes"
  .fill 0x8006cd28-.
.endarea

.org 0x8006cd28
.area 0x8006cdf8-.
  .importobj "compiled/VSEFEInitialize.lib"
  .notice "VS EFE initialize space left: " + (0x8006cdf8-.) + " bytes"
  .fill 0x8006cdf8-.
.endarea

.org 0x8006d218
.area 0x8006d394-.
  .importobj "compiled/VSEFERun.lib"
  .notice "VS EFERun space left: " + (0x8006d394-.) + " bytes"
  .fill 0x8006d394-.
.endarea

.org 0x80070e94
.area 0x80070f34-.
  .notice "VS retired EFE operator table space left: " + (0x80070f34-.) + " bytes"
  .fill 0x80070f34-.
.endarea

; VS__combatMain
.org 0x80058238
  jal VS__combatMain

; VS__runIntro
.org 0x80058228
  jal VS__runIntro

; VS__tickDigimon
.org 0x80056f94
  li.u a2, VS__tickDigimon
.org 0x80056fb0
  li.l a2, VS__tickDigimon
.org 0x8005700c
  li.u a2, VS__tickDigimon
.org 0x80057028
  li.l a2, VS__tickDigimon

; VS__tickVSInput
.org 0x80056e88
  li.u a2, VS__tickVSInput
.org 0x80056e98
  li.l a2, VS__tickVSInput
.org 0x80056ea4
  li.u a2, VS__tickVSInput
.org 0x80056eb0
  li.l a2, VS__tickVSInput

; VS_addParticleEmitter
.org 0x80070bc8
  .word VS_addParticleEmitter

; VS_applyEFEVariableOperator
.org 0x80070e5c
  .word VS_applyEFEVariableOperator

; EFE subeffects and attack collision
.org 0x80070b58
  .word VS_initializeSubEffectInstructions
.org 0x80070b88
  .word VS_addAttackObjectToTarget
.org 0x80070b90
  .word VS_applyDefaultBoxAttackHit
.org 0x80070c58
  .word VS_checkTargetCollision
.org 0x80070e10
  .word VS_applyLineAttackHit
.org 0x80070e18
  .word VS_applyRadiusAttackHit
.org 0x80070e20
  .word VS_applyBoxAttackHit
.org 0x80070e90
  .word VS_spawnSubEffect

; EFE source/target selection and entity queries
.org 0x80070bd0
  .word VS_selectNextTargetEntity
.org 0x80070c00
  .word VS_getTargetDigimonSize
.org 0x80070c70
  .word VS_getSourceDigimonSize
.org 0x80070ca8
  .word VS_findHitEntity
.org 0x80070cb8
  .word VS_setTargetToHitEntity
.org 0x80070da0
  .word VS_selectRandomTargetEntity
.org 0x80070e48
  .word VS_isTargetUnhit

; EFE vector arithmetic subopcodes
.org 0x80070cd0
  .word VS_addVectors
.org 0x80070cd8
  .word VS_subtractVectors
.org 0x80070ce0
  .word VS_multiplyVectors
.org 0x80070ce8
  .word VS_divideVectors
.org 0x80070cf8
  .word VS_shiftVectorsRight
.org 0x80070d80
  .word VS_multiplyVectorByScalar
.org 0x80070d88
  .word VS_divideVectorByScalar
.org 0x80070d90
  .word VS_maskVectorByScalar

; VS_loadNextEFEFile
.org 0x800649ac
  jal VS_loadNextEFEFile

; VS_runEFESlotScript
.org 0x80064ab4
  jal VS_runEFESlotScript

.close

.open "work/DIGIMON/SLUS_010.32",0x80090000
.psx

.org 0x80111b9c
  jal VS__initialize
  
.close
