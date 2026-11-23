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

.org 0x80068d64
.area 0x800690c8-.
  .importobj "compiled/VSEFEVectorArithmetic.lib"
  .notice "VS EFE vector arithmetic space left: " + (0x800690c8-.) + " bytes"
  .fill 0x800690c8-.
.endarea

.org 0x80069168
.area 0x800692bc-.
  .importobj "compiled/VSEFEHitEntity.lib"
  .notice "VS EFE hit entity queries space left: " + (0x800692bc-.) + " bytes"
  .fill 0x800692bc-.
.endarea

.org 0x80069584
.area 0x80069a48-.
  .importobj "compiled/VSEFERotation.lib"
  .notice "VS EFE Rotation space left: " + (0x80069a48-.) + " bytes"
  .fill 0x80069a48-.
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

; Angle and transform commands share these reclaimed regions.
.org 0x800653a8
.area 0x80065434-.
  .notice "VS EFE retired faceTargetEntity space left: " + (0x80065434-.) + " bytes"
  .fill 0x80065434-.
.endarea

.org 0x80067104
.area 0x80067170-.
  .notice "VS EFE retired getCameraRotation space left: " + (0x80067170-.) + " bytes"
  .fill 0x80067170-.
.endarea

.org 0x80067294
.area 0x80067500-.
  .importobj "compiled/VSEFEBonePosition.lib"
  .notice "VS EFE BonePosition space left: " + (0x80067500-.) + " bytes"
  .fill 0x80067500-.
.endarea

.org 0x800677ec
.area 0x80067910-.
  .notice "VS EFE retired setTransformToBoneMatrix space left: " + (0x80067910-.) + " bytes"
  .fill 0x80067910-.
.endarea

.org 0x800684ec
.area 0x800686a8-.
  .importobj "compiled/VSEFEInterpolation.lib"
  .notice "VS EFE Interpolation space left: " + (0x800686a8-.) + " bytes"
  .fill 0x800686a8-.
.endarea

.org 0x800688e8
.area 0x80068d64-.
  .importobj "compiled/VSEFEMovement.lib"
  .notice "VS EFE Movement space left: " + (0x80068d64-.) + " bytes"
  .fill 0x80068d64-.
.endarea

.org 0x800690c8
.area 0x80069168-.
  .notice "VS EFE retired normalizeRotationAngles space left: " + (0x80069168-.) + " bytes"
  .fill 0x80069168-.
.endarea

.org 0x800692bc
.area 0x80069334-.
  .notice "VS EFE retired getVectorEulerAngles space left: " + (0x80069334-.) + " bytes"
  .fill 0x80069334-.
.endarea

.org 0x800693b0
.area 0x80069584-.
  .importobj "compiled/VSEFEBoneTransform.lib"
  .notice "VS EFE BoneTransform space left: " + (0x80069584-.) + " bytes"
  .fill 0x80069584-.
.endarea

.org 0x80069c18
.area 0x80069db8-.
  .importobj "compiled/VSEFEBoneQuery.lib"
  .notice "VS EFE BoneQuery space left: " + (0x80069db8-.) + " bytes"
  .fill 0x80069db8-.
.endarea

.org 0x8006a474
.area 0x8006a59c-.
  .notice "VS EFE retired setTransformToBoneOffset space left: " + (0x8006a59c-.) + " bytes"
  .fill 0x8006a59c-.
.endarea

.org 0x8006a688
.area 0x8006a7f0-.
  .importobj "compiled/VSEFETrigonometry.lib"
  .notice "VS EFE Trigonometry space left: " + (0x8006a7f0-.) + " bytes"
  .fill 0x8006a7f0-.
.endarea

.org 0x8006ad30
.area 0x8006ae08-.
  .notice "VS EFE retired projectPositionToScreen space left: " + (0x8006ae08-.) + " bytes"
  .fill 0x8006ae08-.
.endarea

.org 0x8006b3e4
.area 0x8006b738-.
  .importobj "compiled/VSEFEEntityTransform.lib"
  .importobj "compiled/VSEFEProjection.lib"
  .notice "VS EFE EntityTransform and Projection space left: " + (0x8006b738-.) + " bytes"
  .fill 0x8006b738-.
.endarea

.org 0x8006b75c
.area 0x8006b8b8-.
  .importobj "compiled/VSEFEParentTransfer.lib"
  .notice "VS EFE ParentTransfer space left: " + (0x8006b8b8-.) + " bytes"
  .fill 0x8006b8b8-.
.endarea

.org 0x8006b9e4
.area 0x8006bb2c-.
  .notice "VS EFE retired setTransformToTargetBone space left: " + (0x8006bb2c-.) + " bytes"
  .fill 0x8006bb2c-.
.endarea

.org 0x8006bc08
.area 0x8006bcc8-.
  .importobj "compiled/VSEFEVectorGeometry.lib"
  .notice "VS EFE VectorGeometry space left: " + (0x8006bcc8-.) + " bytes"
  .fill 0x8006bcc8-.
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

; VS_applyHomingMovement
.org 0x80070c68
  .word VS_applyHomingMovement

; VS_calculateCosine
.org 0x80070c80
  .word VS_calculateCosine

; VS_calculatePolarOffset
.org 0x80070c10
  .word VS_calculatePolarOffset

; VS_calculateSine
.org 0x80070c78
  .word VS_calculateSine

; VS_centerTransformOnEntities
.org 0x80070d00
  .word VS_centerTransformOnEntities

; VS_combineRotations
.org 0x80070d20
  .word VS_combineRotations

; VS_convertToViewSpace
.org 0x80070d98
  .word VS_convertToViewSpace

; VS_copyFromParentTransform
.org 0x80070c18
  .word VS_copyFromParentTransform

; VS_copyTargetEntityPosition
.org 0x80070bb8
  .word VS_copyTargetEntityPosition

; VS_copyToParentTransform
.org 0x80070d30
  .word VS_copyToParentTransform

; VS_copyVector
.org 0x80070cc8
  .word VS_copyVector

; VS_faceTargetEntity
.org 0x80070e08
  .word VS_faceTargetEntity

; VS_getCameraRotation
.org 0x80070da8
  .word VS_getCameraRotation

; VS_getScatteredSpawnPosition
.org 0x80070b98
  .word VS_getScatteredSpawnPosition

; VS_getSourceBoneTransform
.org 0x80070d38
  .word VS_getSourceBoneTransform

; VS_getTargetBoneTransform
.org 0x80070d08
  .word VS_getTargetBoneTransform

; VS_getVectorEulerAngles
.org 0x80070ca0
  .word VS_getVectorEulerAngles

; VS_getVectorLength
.org 0x80070cc0
  .word VS_getVectorLength

; VS_initializeEFETransform
.org 0x80070b68
  .word VS_initializeEFETransform

; VS_interpolateValue
.org 0x80070c88
  .word VS_interpolateValue

; VS_interpolateVector
.org 0x80070ba8
  .word VS_interpolateVector

; VS_maskVectors
.org 0x80070cf0
  .word VS_maskVectors

; VS_normalizeRotationAngles
.org 0x80070cb0
  .word VS_normalizeRotationAngles
.org 0x80070d18
  .word VS_normalizeRotationAngles

; VS_projectPositionToScreen
.org 0x80070bf0
  .word VS_projectPositionToScreen

; VS_rotateShortVector
.org 0x80070b70
  .word VS_rotateShortVector

; VS_rotateTransformTowardPoint
.org 0x80070c50
  .word VS_rotateTransformTowardPoint

; VS_rotateVectorByAngles
.org 0x80070d10
  .word VS_rotateVectorByAngles

; VS_setTransformToBoneMatrix
.org 0x80070d70
  .word VS_setTransformToBoneMatrix

; VS_setTransformToBoneOffset
.org 0x80070c30
  .word VS_setTransformToBoneOffset

; VS_setTransformToSourceBone
.org 0x80070c48
  .word VS_setTransformToSourceBone

; VS_setTransformToTargetBone
.org 0x80070b80
  .word VS_setTransformToTargetBone

; VS_steerTransformTowardPoint
.org 0x80070bb0
  .word VS_steerTransformTowardPoint

.close

.open "work/DIGIMON/SLUS_010.32",0x80090000
.psx

.org 0x80111b9c
  jal VS__initialize
  
.close
