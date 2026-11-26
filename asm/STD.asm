.open "work/DIGIMON/STD_REL.BIN",0x80052ae0
.psx

; Shared EFE source is emitted only in this owning overlay.
.org 0x8006d168
.area 0x8006d4f4-.
  .importobj "compiled/STDEFEFrame.lib"
  .notice "STD EFE tick/render space left: " + (0x8006d4f4-.) + " bytes"
  .fill 0x8006d4f4-.
.endarea

; Keep the vanilla sound clear function before the replacement stop operation.
.org 0x8006d530
.area 0x8006d594-.
  .importobj "compiled/STDEFESound.lib"
  .notice "STD EFE stop sounds space left: " + (0x8006d594-.) + " bytes"
  .fill 0x8006d594-.
.endarea

.org 0x8006d594
.area 0x8006dee8-.
  .importobj "compiled/STDEFELifecycle.lib"
  .notice "STD EFE lifecycle space left: " + (0x8006dee8-.) + " bytes"
  .fill 0x8006dee8-.
.endarea

.org 0x8006dee8
.area 0x8006dff8-.
  .notice "STD retired EFE UV tick space left: " + (0x8006dff8-.) + " bytes"
  .fill 0x8006dff8-.
.endarea

.org 0x8006dff8
.area 0x8006e558-.
  .importobj "compiled/STDEFEParticles.lib"
  .notice "STD reclaimed EFE particles space left: " + (0x8006e558-.) + " bytes"
  .fill 0x8006e558-.
.endarea

.org 0x8006e558
.area 0x8006e880-.
  .importobj "compiled/STDEFEUVAnimation.lib"
  .notice "STD EFE UV animation space left: " + (0x8006e880-.) + " bytes"
  .fill 0x8006e880-.
.endarea

.org 0x8006e8fc
.area 0x8006e984-.
  .importobj "compiled/STDEFESlotScript.lib"
  .notice "STD EFE slot script space left: " + (0x8006e984-.) + " bytes"
  .fill 0x8006e984-.
.endarea

.org 0x8006eaec
.area 0x8006eb38-.
  .notice "STD EFE retired isTargetUnhit space left: " + (0x8006eb38-.) + " bytes"
  .fill 0x8006eb38-.
.endarea

.org 0x8006eb9c
.area 0x8006f2d8-.
  .importobj "compiled/STDEFEAttackCollision.lib"
  .notice "STD EFE attack and collision space left: " + (0x8006f2d8-.) + " bytes"
  .fill 0x8006f2d8-.
.endarea

.org 0x80070b80
.area 0x80070d38-.
  .notice "STD retired EFE screen-space model draw space left: " + (0x80070d38-.) + " bytes"
  .fill 0x80070d38-.
.endarea

; Reuse the vanilla YXZ draw and the adjoining retired camera rotation command.
.org 0x80070ea0
.area 0x800710a0-.
  .importobj "compiled/STDEFEModelDraw.lib"
  .notice "STD EFE model drawing space left: " + (0x800710a0-.) + " bytes"
  .fill 0x800710a0-.
.endarea

.org 0x800710a0
.area 0x800711c4-.
  .importobj "compiled/STDEFEEntityQueries.lib"
  .notice "STD EFE entity queries space left: " + (0x800711c4-.) + " bytes"
  .fill 0x800711c4-.
.endarea

.org 0x80072c94
.area 0x80072ff8-.
  .importobj "compiled/STDEFEVectorArithmetic.lib"
  .notice "STD EFE vector arithmetic space left: " + (0x80072ff8-.) + " bytes"
  .fill 0x80072ff8-.
.endarea

.org 0x80073098
.area 0x800731ec-.
  .importobj "compiled/STDEFEHitEntity.lib"
  .notice "STD EFE hit entity queries space left: " + (0x800731ec-.) + " bytes"
  .fill 0x800731ec-.
.endarea

.org 0x800734b4
.area 0x80073978-.
  .importobj "compiled/STDEFERotation.lib"
  .notice "STD EFE Rotation space left: " + (0x80073978-.) + " bytes"
  .fill 0x80073978-.
.endarea

.org 0x80073978
.area 0x800739c0-.
  .notice "STD retired EFE UV timer space left: " + (0x800739c0-.) + " bytes"
  .fill 0x800739c0-.
.endarea

.org 0x800739c0
.area 0x80073b48-.
  .importobj "compiled/STDEFETargetCollision.lib"
  .notice "STD EFE target collision space left: " + (0x80073b48-.) + " bytes"
  .fill 0x80073b48-.
.endarea

.org 0x80074a70
.area 0x80074ae8-.
  .notice "STD EFE retired getTargetDigimonSize space left: " + (0x80074ae8-.) + " bytes"
  .fill 0x80074ae8-.
.endarea

.org 0x80074fd4
.area 0x8007522c-.
  .importobj "compiled/STDEFETargetSelection.lib"
  .notice "STD EFE target selection and slot release space left: " + (0x8007522c-.) + " bytes"
  .fill 0x8007522c-.
.endarea

.org 0x800757e8
.area 0x80075914-.
  .notice "STD retired EFE default and target attacks space left: " + (0x80075914-.) + " bytes"
  .fill 0x80075914-.
.endarea

.org 0x80075bf8
.area 0x80075d8c-.
  .notice "STD retired EFE world model draw space left: " + (0x80075d8c-.) + " bytes"
  .fill 0x80075d8c-.
.endarea

.org 0x80075d8c
.area 0x80075dd8-.
  .notice "STD retired EFE subeffect setup space left: " + (0x80075dd8-.) + " bytes"
  .fill 0x80075dd8-.
.endarea

.org 0x80075dd8
.area 0x80075ed8-.
  .notice "STD retired EFE UV setup space left: " + (0x80075ed8-.) + " bytes"
  .fill 0x80075ed8-.
.endarea

.org 0x80075f70
.area 0x800760ec-.
  .importobj "compiled/STDEFESubEffect.lib"
  .notice "STD EFE subeffects space left: " + (0x800760ec-.) + " bytes"
  .fill 0x800760ec-.
.endarea

.org 0x800760ec
.area 0x80076c58-.
  .importobj "compiled/STDEFEAddressOperand.lib"
  .importobj "compiled/STDEFEDispatch.lib"
  .importobj "compiled/STDEFEIndexedOperand.lib"
  .importobj "compiled/STDEFEControl.lib"
  .importobj "compiled/STDEFEOperands.lib"
  .importobj "compiled/STDEFEArithmetic.lib"
  .notice "STD EFE operands, control and arithmetic space left: " + (0x80076c58-.) + " bytes"
  .fill 0x80076c58-.
.endarea

.org 0x80076c58
.area 0x80076d28-.
  .importobj "compiled/STDEFEInitialize.lib"
  .notice "STD EFE initialize space left: " + (0x80076d28-.) + " bytes"
  .fill 0x80076d28-.
.endarea

.org 0x80077148
.area 0x800772c4-.
  .importobj "compiled/STDEFERun.lib"
  .notice "STD EFERun space left: " + (0x800772c4-.) + " bytes"
  .fill 0x800772c4-.
.endarea

.org 0x8007ae68
.area 0x8007af08-.
  .notice "STD retired EFE operator table space left: " + (0x8007af08-.) + " bytes"
  .fill 0x8007af08-.
.endarea

; Angle and transform commands share these reclaimed regions.
.org 0x8006f2d8
.area 0x8006f364-.
  .notice "STD EFE retired faceTargetEntity space left: " + (0x8006f364-.) + " bytes"
  .fill 0x8006f364-.
.endarea

.org 0x800711c4
.area 0x80071430-.
  .importobj "compiled/STDEFEBonePosition.lib"
  .notice "STD EFE BonePosition space left: " + (0x80071430-.) + " bytes"
  .fill 0x80071430-.
.endarea

.org 0x8007171c
.area 0x80071840-.
  .notice "STD EFE retired setTransformToBoneMatrix space left: " + (0x80071840-.) + " bytes"
  .fill 0x80071840-.
.endarea

.org 0x8007241c
.area 0x800725d8-.
  .importobj "compiled/STDEFEInterpolation.lib"
  .notice "STD EFE Interpolation space left: " + (0x800725d8-.) + " bytes"
  .fill 0x800725d8-.
.endarea

.org 0x80072818
.area 0x80072c94-.
  .importobj "compiled/STDEFEMovement.lib"
  .notice "STD EFE Movement space left: " + (0x80072c94-.) + " bytes"
  .fill 0x80072c94-.
.endarea

.org 0x80072ff8
.area 0x80073098-.
  .notice "STD EFE retired normalizeRotationAngles space left: " + (0x80073098-.) + " bytes"
  .fill 0x80073098-.
.endarea

.org 0x800731ec
.area 0x80073264-.
  .notice "STD EFE retired getVectorEulerAngles space left: " + (0x80073264-.) + " bytes"
  .fill 0x80073264-.
.endarea

.org 0x800732e0
.area 0x800734b4-.
  .importobj "compiled/STDEFEBoneTransform.lib"
  .notice "STD EFE BoneTransform space left: " + (0x800734b4-.) + " bytes"
  .fill 0x800734b4-.
.endarea

.org 0x80073b48
.area 0x80073ce8-.
  .importobj "compiled/STDEFEBoneQuery.lib"
  .notice "STD EFE BoneQuery space left: " + (0x80073ce8-.) + " bytes"
  .fill 0x80073ce8-.
.endarea

.org 0x800743a4
.area 0x800744cc-.
  .notice "STD EFE retired setTransformToBoneOffset space left: " + (0x800744cc-.) + " bytes"
  .fill 0x800744cc-.
.endarea

.org 0x800745b8
.area 0x80074720-.
  .importobj "compiled/STDEFETrigonometry.lib"
  .notice "STD EFE Trigonometry space left: " + (0x80074720-.) + " bytes"
  .fill 0x80074720-.
.endarea

.org 0x80074c60
.area 0x80074d38-.
  .notice "STD EFE retired projectPositionToScreen space left: " + (0x80074d38-.) + " bytes"
  .fill 0x80074d38-.
.endarea

.org 0x80075314
.area 0x80075668-.
  .importobj "compiled/STDEFEEntityTransform.lib"
  .importobj "compiled/STDEFEProjection.lib"
  .notice "STD EFE EntityTransform and Projection space left: " + (0x80075668-.) + " bytes"
  .fill 0x80075668-.
.endarea

.org 0x8007568c
.area 0x800757e8-.
  .importobj "compiled/STDEFEParentTransfer.lib"
  .notice "STD EFE ParentTransfer space left: " + (0x800757e8-.) + " bytes"
  .fill 0x800757e8-.
.endarea

.org 0x80075914
.area 0x80075a5c-.
  .notice "STD EFE retired setTransformToTargetBone space left: " + (0x80075a5c-.) + " bytes"
  .fill 0x80075a5c-.
.endarea

.org 0x80075b38
.area 0x80075bf8-.
  .importobj "compiled/STDEFEVectorGeometry.lib"
  .notice "STD EFE VectorGeometry space left: " + (0x80075bf8-.) + " bytes"
  .fill 0x80075bf8-.
.endarea

; EFE UV animation subopcodes
.org 0x8007ab24
  .word STD_initializeUVAnim
.org 0x8007ac34
  .word STD_getUVAnimTimer

; STD_addParticleEmitter
.org 0x8007ab9c
  .word STD_addParticleEmitter

; STD_initializeEFEEngine
.org 0x8005f074
  jal STD_initializeEFEEngine

; STD_applyEFEVariableOperator
.org 0x8007ae30
  .word STD_applyEFEVariableOperator

; EFE model drawing subopcodes
.org 0x8007ab34
  .word STD_drawTMD
.org 0x8007ad84
  .word STD_drawTMDYXZ
.org 0x8007ad94
  .word STD_drawTMDScreenSpace

; EFE subeffects and attack collision
.org 0x8007ab2c
  .word STD_initializeSubEffectInstructions
.org 0x8007ab5c
  .word STD_addAttackObjectToTarget
.org 0x8007ab64
  .word STD_applyDefaultBoxAttackHit
.org 0x8007ac2c
  .word STD_checkTargetCollision
.org 0x8007ade4
  .word STD_applyLineAttackHit
.org 0x8007adec
  .word STD_applyRadiusAttackHit
.org 0x8007adf4
  .word STD_applyBoxAttackHit
.org 0x8007ae64
  .word STD_spawnSubEffect

; EFE source/target selection and entity queries
.org 0x8007aba4
  .word STD_selectNextTargetEntity
.org 0x8007abd4
  .word STD_getTargetDigimonSize
.org 0x8007ac44
  .word STD_getSourceDigimonSize
.org 0x8007ac7c
  .word STD_findHitEntity
.org 0x8007ac8c
  .word STD_setTargetToHitEntity
.org 0x8007ad74
  .word STD_selectRandomTargetEntity
.org 0x8007ae1c
  .word STD_isTargetUnhit

; EFE vector arithmetic subopcodes
.org 0x8007aca4
  .word STD_addVectors
.org 0x8007acac
  .word STD_subtractVectors
.org 0x8007acb4
  .word STD_multiplyVectors
.org 0x8007acbc
  .word STD_divideVectors
.org 0x8007accc
  .word STD_shiftVectorsRight
.org 0x8007ad54
  .word STD_multiplyVectorByScalar
.org 0x8007ad5c
  .word STD_divideVectorByScalar
.org 0x8007ad64
  .word STD_maskVectorByScalar

; STD_loadNextEFEFile
.org 0x8006e8dc
  jal STD_loadNextEFEFile

; STD_removeEFEEngine
.org 0x80060a84
  jal STD_removeEFEEngine

; STD_runEFESlotScript
.org 0x8006e9e4
  jal STD_runEFESlotScript

; Engine removal now owns slot unloading; the existing delay slot is nop.
.org 0x80060a7c
  nop

; STD_applyHomingMovement
.org 0x8007ac3c
  .word STD_applyHomingMovement

; STD_calculateCosine
.org 0x8007ac54
  .word STD_calculateCosine

; STD_calculatePolarOffset
.org 0x8007abe4
  .word STD_calculatePolarOffset

; STD_calculateSine
.org 0x8007ac4c
  .word STD_calculateSine

; STD_centerTransformOnEntities
.org 0x8007acd4
  .word STD_centerTransformOnEntities

; STD_combineRotations
.org 0x8007acf4
  .word STD_combineRotations

; STD_convertToViewSpace
.org 0x8007ad6c
  .word STD_convertToViewSpace

; STD_copyFromParentTransform
.org 0x8007abec
  .word STD_copyFromParentTransform

; STD_copyTargetEntityPosition
.org 0x8007ab8c
  .word STD_copyTargetEntityPosition

; STD_copyToParentTransform
.org 0x8007ad04
  .word STD_copyToParentTransform

; STD_copyVector
.org 0x8007ac9c
  .word STD_copyVector

; STD_faceTargetEntity
.org 0x8007addc
  .word STD_faceTargetEntity

; STD_getCameraRotation
.org 0x8007ad7c
  .word STD_getCameraRotation

; STD_getScatteredSpawnPosition
.org 0x8007ab6c
  .word STD_getScatteredSpawnPosition

; STD_getSourceBoneTransform
.org 0x8007ad0c
  .word STD_getSourceBoneTransform

; STD_getTargetBoneTransform
.org 0x8007acdc
  .word STD_getTargetBoneTransform

; STD_getVectorEulerAngles
.org 0x8007ac74
  .word STD_getVectorEulerAngles

; STD_getVectorLength
.org 0x8007ac94
  .word STD_getVectorLength

; STD_initializeEFETransform
.org 0x8007ab3c
  .word STD_initializeEFETransform

; STD_interpolateValue
.org 0x8007ac5c
  .word STD_interpolateValue

; STD_interpolateVector
.org 0x8007ab7c
  .word STD_interpolateVector

; STD_maskVectors
.org 0x8007acc4
  .word STD_maskVectors

; STD_normalizeRotationAngles
.org 0x8007ac84
  .word STD_normalizeRotationAngles
.org 0x8007acec
  .word STD_normalizeRotationAngles

; STD_projectPositionToScreen
.org 0x8007abc4
  .word STD_projectPositionToScreen

; STD_rotateShortVector
.org 0x8007ab44
  .word STD_rotateShortVector

; STD_rotateTransformTowardPoint
.org 0x8007ac24
  .word STD_rotateTransformTowardPoint

; STD_rotateVectorByAngles
.org 0x8007ace4
  .word STD_rotateVectorByAngles

; STD_setTransformToBoneMatrix
.org 0x8007ad44
  .word STD_setTransformToBoneMatrix

; STD_setTransformToBoneOffset
.org 0x8007ac04
  .word STD_setTransformToBoneOffset

; STD_setTransformToSourceBone
.org 0x8007ac1c
  .word STD_setTransformToSourceBone

; STD_setTransformToTargetBone
.org 0x8007ab54
  .word STD_setTransformToTargetBone

; STD_steerTransformTowardPoint
.org 0x8007ab84
  .word STD_steerTransformTowardPoint

.close
