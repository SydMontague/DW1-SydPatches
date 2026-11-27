.open "work/DIGIMON/BTL_REL.BIN",0x80052ae0
.psx

; Shared EFE source is emitted only in this owning overlay.
.org 0x80064a80
.area 0x80064e0c-.
  .importobj "compiled/BTLEFEFrame.lib"
  .notice "BTL EFE tick/render space left: " + (0x80064e0c-.) + " bytes"
  .fill 0x80064e0c-.
.endarea

; Keep the vanilla sound clear function before the replacement stop operation.
.org 0x80064e48
.area 0x80064eac-.
  .importobj "compiled/BTLEFESound.lib"
  .notice "BTL EFE stop sounds space left: " + (0x80064eac-.) + " bytes"
  .fill 0x80064eac-.
.endarea

.org 0x80064eac
.area 0x80065800-.
  .importobj "compiled/BTLEFELifecycle.lib"
  .notice "BTL EFE lifecycle space left: " + (0x80065800-.) + " bytes"
  .fill 0x80065800-.
.endarea

.org 0x80065800
.area 0x80065910-.
  .notice "BTL retired EFE UV tick space left: " + (0x80065910-.) + " bytes"
  .fill 0x80065910-.
.endarea

.org 0x80065910
.area 0x80065e70-.
  .importobj "compiled/BTLEFEParticles.lib"
  .notice "BTL reclaimed EFE particles space left: " + (0x80065e70-.) + " bytes"
  .fill 0x80065e70-.
.endarea

.org 0x80065e70
.area 0x80066198-.
  .importobj "compiled/BTLEFEUVAnimation.lib"
  .notice "BTL EFE UV animation space left: " + (0x80066198-.) + " bytes"
  .fill 0x80066198-.
.endarea

.org 0x800661d8
.area 0x80066260-.
  .importobj "compiled/BTLEFESlotScript.lib"
  .notice "BTL EFE slot script space left: " + (0x80066260-.) + " bytes"
  .fill 0x80066260-.
.endarea

.org 0x800663c8
.area 0x80066414-.
  .notice "BTL EFE retired isTargetUnhit space left: " + (0x80066414-.) + " bytes"
  .fill 0x80066414-.
.endarea

.org 0x800665a4
.area 0x80066ce0-.
  .importobj "compiled/BTLEFEAttackCollision.lib"
  .notice "BTL EFE attack and collision space left: " + (0x80066ce0-.) + " bytes"
  .fill 0x80066ce0-.
.endarea

.org 0x800685e8
.area 0x800687a0-.
  .notice "BTL retired EFE screen-space model draw space left: " + (0x800687a0-.) + " bytes"
  .fill 0x800687a0-.
.endarea

; Reuse the vanilla YXZ draw and the adjoining retired camera rotation command.
.org 0x80068908
.area 0x80068b08-.
  .importobj "compiled/BTLEFEModelDraw.lib"
  .notice "BTL EFE model drawing space left: " + (0x80068b08-.) + " bytes"
  .fill 0x80068b08-.
.endarea

.org 0x80068b08
.area 0x80068c2c-.
  .importobj "compiled/BTLEFEEntityQueries.lib"
  .notice "BTL EFE entity queries space left: " + (0x80068c2c-.) + " bytes"
  .fill 0x80068c2c-.
.endarea

.org 0x8006a688
.area 0x8006a9ec-.
  .importobj "compiled/BTLEFEVectorArithmetic.lib"
  .notice "BTL EFE vector arithmetic space left: " + (0x8006a9ec-.) + " bytes"
  .fill 0x8006a9ec-.
.endarea

.org 0x8006aa8c
.area 0x8006abe0-.
  .importobj "compiled/BTLEFEHitEntity.lib"
  .notice "BTL EFE hit entity queries space left: " + (0x8006abe0-.) + " bytes"
  .fill 0x8006abe0-.
.endarea

.org 0x8006aea8
.area 0x8006b36c-.
  .importobj "compiled/BTLEFERotation.lib"
  .notice "BTL EFE Rotation space left: " + (0x8006b36c-.) + " bytes"
  .fill 0x8006b36c-.
.endarea

.org 0x8006b36c
.area 0x8006b3b4-.
  .notice "BTL retired EFE UV timer space left: " + (0x8006b3b4-.) + " bytes"
  .fill 0x8006b3b4-.
.endarea

.org 0x8006b3b4
.area 0x8006b53c-.
  .importobj "compiled/BTLEFETargetCollision.lib"
  .notice "BTL EFE target collision space left: " + (0x8006b53c-.) + " bytes"
  .fill 0x8006b53c-.
.endarea

.org 0x8006c464
.area 0x8006c4dc-.
  .notice "BTL EFE retired getTargetDigimonSize space left: " + (0x8006c4dc-.) + " bytes"
  .fill 0x8006c4dc-.
.endarea

.org 0x8006c9f8
.area 0x8006cd38-.
  .importobj "compiled/BTLEFETargetSelection.lib"
  .importobj "compiled/BTLEFEModelColor.lib"
  .notice "BTL EFE target selection, slot release and model coloring space left: " + (0x8006cd38-.) + " bytes"
  .fill 0x8006cd38-.
.endarea

.org 0x8006d20c
.area 0x8006d338-.
  .notice "BTL retired EFE default and target attacks space left: " + (0x8006d338-.) + " bytes"
  .fill 0x8006d338-.
.endarea

.org 0x8006d61c
.area 0x8006d7b0-.
  .notice "BTL retired EFE world model draw space left: " + (0x8006d7b0-.) + " bytes"
  .fill 0x8006d7b0-.
.endarea

.org 0x8006d7b0
.area 0x8006d7fc-.
  .notice "BTL retired EFE subeffect setup space left: " + (0x8006d7fc-.) + " bytes"
  .fill 0x8006d7fc-.
.endarea

.org 0x8006d7fc
.area 0x8006d8fc-.
  .notice "BTL retired EFE UV setup space left: " + (0x8006d8fc-.) + " bytes"
  .fill 0x8006d8fc-.
.endarea

.org 0x8006d994
.area 0x8006db10-.
  .importobj "compiled/BTLEFESubEffect.lib"
  .notice "BTL EFE subeffects space left: " + (0x8006db10-.) + " bytes"
  .fill 0x8006db10-.
.endarea

.org 0x8006db10
.area 0x8006e67c-.
  .importobj "compiled/BTLEFEAddressOperand.lib"
  .importobj "compiled/BTLEFEDispatch.lib"
  .importobj "compiled/BTLEFEIndexedOperand.lib"
  .importobj "compiled/BTLEFEControl.lib"
  .importobj "compiled/BTLEFEOperands.lib"
  .importobj "compiled/BTLEFEArithmetic.lib"
  .notice "BTL EFE operands, control and arithmetic space left: " + (0x8006e67c-.) + " bytes"
  .fill 0x8006e67c-.
.endarea

.org 0x8006e67c
.area 0x8006e74c-.
  .importobj "compiled/BTLEFEInitialize.lib"
  .notice "BTL EFE initialize space left: " + (0x8006e74c-.) + " bytes"
  .fill 0x8006e74c-.
.endarea

.org 0x8006eba8
.area 0x8006ed24-.
  .importobj "compiled/BTLEFERun.lib"
  .notice "BTL EFERun space left: " + (0x8006ed24-.) + " bytes"
  .fill 0x8006ed24-.
.endarea

.org 0x8007364c
.area 0x800736ec-.
  .notice "BTL retired EFE operator table space left: " + (0x800736ec-.) + " bytes"
  .fill 0x800736ec-.
.endarea

.org 0x8006ffb4
.area 0x80070064-.
  .notice "BTL retired aura model coloring space left: " + (0x80070064-.) + " bytes"
  .fill 0x80070064-.
.endarea

; Angle and transform commands share these reclaimed regions.
.org 0x80066ce0
.area 0x80066d6c-.
  .notice "BTL EFE retired faceTargetEntity space left: " + (0x80066d6c-.) + " bytes"
  .fill 0x80066d6c-.
.endarea

.org 0x80068c2c
.area 0x80068e98-.
  .importobj "compiled/BTLEFEBonePosition.lib"
  .notice "BTL EFE BonePosition space left: " + (0x80068e98-.) + " bytes"
  .fill 0x80068e98-.
.endarea

.org 0x80069184
.area 0x800692a8-.
  .notice "BTL EFE retired setTransformToBoneMatrix space left: " + (0x800692a8-.) + " bytes"
  .fill 0x800692a8-.
.endarea

.org 0x80069e10
.area 0x80069fcc-.
  .importobj "compiled/BTLEFEInterpolation.lib"
  .notice "BTL EFE Interpolation space left: " + (0x80069fcc-.) + " bytes"
  .fill 0x80069fcc-.
.endarea

.org 0x8006a20c
.area 0x8006a688-.
  .importobj "compiled/BTLEFEMovement.lib"
  .notice "BTL EFE Movement space left: " + (0x8006a688-.) + " bytes"
  .fill 0x8006a688-.
.endarea

.org 0x8006a9ec
.area 0x8006aa8c-.
  .notice "BTL EFE retired normalizeRotationAngles space left: " + (0x8006aa8c-.) + " bytes"
  .fill 0x8006aa8c-.
.endarea

.org 0x8006abe0
.area 0x8006ac58-.
  .notice "BTL EFE retired getVectorEulerAngles space left: " + (0x8006ac58-.) + " bytes"
  .fill 0x8006ac58-.
.endarea

.org 0x8006acd4
.area 0x8006aea8-.
  .importobj "compiled/BTLEFEBoneTransform.lib"
  .notice "BTL EFE BoneTransform space left: " + (0x8006aea8-.) + " bytes"
  .fill 0x8006aea8-.
.endarea

.org 0x8006b53c
.area 0x8006b6dc-.
  .importobj "compiled/BTLEFEBoneQuery.lib"
  .notice "BTL EFE BoneQuery space left: " + (0x8006b6dc-.) + " bytes"
  .fill 0x8006b6dc-.
.endarea

.org 0x8006bd98
.area 0x8006bec0-.
  .notice "BTL EFE retired setTransformToBoneOffset space left: " + (0x8006bec0-.) + " bytes"
  .fill 0x8006bec0-.
.endarea

.org 0x8006bfac
.area 0x8006c114-.
  .importobj "compiled/BTLEFETrigonometry.lib"
  .notice "BTL EFE Trigonometry space left: " + (0x8006c114-.) + " bytes"
  .fill 0x8006c114-.
.endarea

.org 0x8006c654
.area 0x8006c72c-.
  .notice "BTL EFE retired projectPositionToScreen space left: " + (0x8006c72c-.) + " bytes"
  .fill 0x8006c72c-.
.endarea

.org 0x8006cd38
.area 0x8006d08c-.
  .importobj "compiled/BTLEFEEntityTransform.lib"
  .importobj "compiled/BTLEFEProjection.lib"
  .notice "BTL EFE EntityTransform and Projection space left: " + (0x8006d08c-.) + " bytes"
  .fill 0x8006d08c-.
.endarea

.org 0x8006d0b0
.area 0x8006d20c-.
  .importobj "compiled/BTLEFEParentTransfer.lib"
  .notice "BTL EFE ParentTransfer space left: " + (0x8006d20c-.) + " bytes"
  .fill 0x8006d20c-.
.endarea

.org 0x8006d338
.area 0x8006d480-.
  .notice "BTL EFE retired setTransformToTargetBone space left: " + (0x8006d480-.) + " bytes"
  .fill 0x8006d480-.
.endarea

.org 0x8006d55c
.area 0x8006d61c-.
  .importobj "compiled/BTLEFEVectorGeometry.lib"
  .notice "BTL EFE VectorGeometry space left: " + (0x8006d61c-.) + " bytes"
  .fill 0x8006d61c-.
.endarea

; EFE model color subopcode
.org 0x80073378
  .word BTL_setEFEModelObjectColor

; BTL_initializeFinisherAuraModel
.org 0x800709c4
  jal BTL_setTMDObjectColor
.org 0x800709d4
  jal BTL_setTMDObjectColor
.org 0x800709e4
  jal BTL_setTMDObjectColor
.org 0x800709f4
  jal BTL_setTMDObjectColor
.org 0x80070a04
  jal BTL_setTMDObjectColor

; EFE UV animation subopcodes
.org 0x80073308
  .word BTL_initializeUVAnim
.org 0x80073418
  .word BTL_getUVAnimTimer

; BTL_addParticleEmitter
.org 0x80073380
  .word BTL_addParticleEmitter

; BTL_applyEFEVariableOperator
.org 0x80073614
  .word BTL_applyEFEVariableOperator

; EFE model drawing subopcodes
.org 0x80073318
  .word BTL_drawTMD
.org 0x80073568
  .word BTL_drawTMDYXZ
.org 0x80073578
  .word BTL_drawTMDScreenSpace

; EFE subeffects and attack collision
.org 0x80073310
  .word BTL_initializeSubEffectInstructions
.org 0x80073340
  .word BTL_addAttackObjectToTarget
.org 0x80073348
  .word BTL_applyDefaultBoxAttackHit
.org 0x80073410
  .word BTL_checkTargetCollision
.org 0x800735c8
  .word BTL_applyLineAttackHit
.org 0x800735d0
  .word BTL_applyRadiusAttackHit
.org 0x800735d8
  .word BTL_applyBoxAttackHit
.org 0x80073648
  .word BTL_spawnSubEffect

; EFE source/target selection and entity queries
.org 0x80073388
  .word BTL_selectNextTargetEntity
.org 0x800733b8
  .word BTL_getTargetDigimonSize
.org 0x80073428
  .word BTL_getSourceDigimonSize
.org 0x80073460
  .word BTL_findHitEntity
.org 0x80073470
  .word BTL_setTargetToHitEntity
.org 0x80073558
  .word BTL_selectRandomTargetEntity
.org 0x80073600
  .word BTL_isTargetUnhit

; EFE vector arithmetic subopcodes
.org 0x80073488
  .word BTL_addVectors
.org 0x80073490
  .word BTL_subtractVectors
.org 0x80073498
  .word BTL_multiplyVectors
.org 0x800734a0
  .word BTL_divideVectors
.org 0x800734b0
  .word BTL_shiftVectorsRight
.org 0x80073538
  .word BTL_multiplyVectorByScalar
.org 0x80073540
  .word BTL_divideVectorByScalar
.org 0x80073548
  .word BTL_maskVectorByScalar

; BTL_loadNextEFEFile
.org 0x800661d0
  j BTL_loadNextEFEFile

; BTL_removeEFEEngine
.org 0x80059050
  jal BTL_removeEFEEngine

; BTL_runEFESlotScript
.org 0x800662c0
  jal BTL_runEFESlotScript

; Engine removal now owns slot unloading; the existing delay slot is nop.
.org 0x80059048
  nop

; BTL_applyHomingMovement
.org 0x80073420
  .word BTL_applyHomingMovement

; BTL_calculateCosine
.org 0x80073438
  .word BTL_calculateCosine

; BTL_calculatePolarOffset
.org 0x800733c8
  .word BTL_calculatePolarOffset

; BTL_calculateSine
.org 0x80073430
  .word BTL_calculateSine

; BTL_centerTransformOnEntities
.org 0x800734b8
  .word BTL_centerTransformOnEntities

; BTL_combineRotations
.org 0x800734d8
  .word BTL_combineRotations

; BTL_convertToViewSpace
.org 0x80073550
  .word BTL_convertToViewSpace

; BTL_copyFromParentTransform
.org 0x800733d0
  .word BTL_copyFromParentTransform

; BTL_copyTargetEntityPosition
.org 0x80073370
  .word BTL_copyTargetEntityPosition

; BTL_copyToParentTransform
.org 0x800734e8
  .word BTL_copyToParentTransform

; BTL_copyVector
.org 0x80073480
  .word BTL_copyVector

; BTL_faceTargetEntity
.org 0x800735c0
  .word BTL_faceTargetEntity

; BTL_getCameraRotation
.org 0x80073560
  .word BTL_getCameraRotation

; BTL_getScatteredSpawnPosition
.org 0x80073350
  .word BTL_getScatteredSpawnPosition

; BTL_getSourceBoneTransform
.org 0x800734f0
  .word BTL_getSourceBoneTransform

; BTL_getTargetBoneTransform
.org 0x800734c0
  .word BTL_getTargetBoneTransform

; BTL_getVectorEulerAngles
.org 0x80073458
  .word BTL_getVectorEulerAngles

; BTL_getVectorLength
.org 0x80073478
  .word BTL_getVectorLength

; BTL_initializeEFETransform
.org 0x80073320
  .word BTL_initializeEFETransform

; BTL_interpolateValue
.org 0x80073440
  .word BTL_interpolateValue

; BTL_interpolateVector
.org 0x80073360
  .word BTL_interpolateVector

; BTL_maskVectors
.org 0x800734a8
  .word BTL_maskVectors

; BTL_normalizeRotationAngles
.org 0x80073468
  .word BTL_normalizeRotationAngles
.org 0x800734d0
  .word BTL_normalizeRotationAngles

; BTL_projectPositionToScreen
.org 0x800733a8
  .word BTL_projectPositionToScreen

; BTL_rotateShortVector
.org 0x80073328
  .word BTL_rotateShortVector

; BTL_rotateTransformTowardPoint
.org 0x80073408
  .word BTL_rotateTransformTowardPoint

; BTL_rotateVectorByAngles
.org 0x800734c8
  .word BTL_rotateVectorByAngles

; BTL_setTransformToBoneMatrix
.org 0x80073528
  .word BTL_setTransformToBoneMatrix

; BTL_setTransformToBoneOffset
.org 0x800733e8
  .word BTL_setTransformToBoneOffset

; BTL_setTransformToSourceBone
.org 0x80073400
  .word BTL_setTransformToSourceBone

; BTL_setTransformToTargetBone
.org 0x80073338
  .word BTL_setTransformToTargetBone

; BTL_steerTransformTowardPoint
.org 0x80073368
  .word BTL_steerTransformTowardPoint

.close
