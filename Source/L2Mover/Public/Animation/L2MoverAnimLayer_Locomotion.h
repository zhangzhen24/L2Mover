
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "L2MoverAnimLayer.h"
#include "L2MoverGameplayTags.h"
#include "Animation/AnimInstance.h"
#include "L2Mover/Public/Types/L2MoverMovementEnums.h"
#include "L2MoverAnimLayer_Locomotion.generated.h"


class ACharacter;
class UCharacterMovementComponent;
class UL2MoverComponent;
class UL2MovementDefinition;

struct FAnimUpdateContext;
struct FAnimNodeReference;
struct FL2Mover_AnimData;
struct FL2Mover_AnimData_Idle;
struct FL2Mover_AnimData_Start_VelocityDirection;
struct FL2Mover_AnimData_Start_LookingDirection;
struct FL2Mover_AnimData_Cycle;
struct FL2Mover_AnimData_Stop;
struct FL2Mover_AnimData_Sprint;
struct FL2Mover_AnimData_Pivot;
struct FL2Mover_AnimData_TurnInPlace;
struct FL2Mover_AnimData_Jump;
struct FL2Mover_AnimData_AimOffset;
struct FL2MoverMovementAnimSettings;


namespace L2Mover_AnimCurveName
{
	static const FName DisableLegIK = TEXT("DisableLegIK");
	static const FName DisableRHandIK = TEXT("DisableRHandIK");
	static const FName DisableLHandIK = TEXT("DisableLHandIK");
	static const FName TurnYawWeight = TEXT("TurnYawWeight");
	static const FName RemainingTurnYaw = TEXT("RemainingTurnYaw");
	static const FName LocomotionDistance = TEXT("Distance");
}

/**
 * 
 */
UCLASS()
class L2MOVER_API UL2MoverAnimLayer_Locomotion : public UL2MoverAnimLayer
{
	GENERATED_BODY()

public:
	UL2MoverAnimLayer_Locomotion(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	// -- Begin AnimInstance Interface
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUninitializeAnimation() override;
	virtual void NativeBeginPlay() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	virtual void NativeThreadSafeUpdateAnimation(float DeltaSeconds) override;
	// -- End AnimInstance Interface

protected:
	void Initialization();

	UFUNCTION()
	void OnUpdateGait(const FGameplayTag& InGait);

	UFUNCTION()
	void OnUpdateRotationMode(const FGameplayTag& InRotationMode);

	UFUNCTION()
	void OnUpdateMovementDefinition(UL2MovementDefinition* InMovementDefinition);

	UFUNCTION()
	void OnAccelerationChanged(const bool InHasAcceleration, const double InLastVelocityDirection);

	UFUNCTION()
	void OnUpdateRootYawOffsetStatus(const bool bInEnableRootYawOffset);

	UFUNCTION()
	void OnMovementModeChanged(ACharacter* InCharacter, EMovementMode InPrevMovementMode, uint8 InPreviousCustomMode);

	UFUNCTION()
	void ResetMovementStateChanged();

	UFUNCTION()
	void ResetCrouchStateChanged() { bCrouchStateChange = false; }

	UFUNCTION()
	void ResetRotationModeChanged() { bRotationModeChanged = false; }

	UFUNCTION()
	void ResetMovementModeChanged() { bMovementModeChanged = false; }

	UFUNCTION()
	void ResetLinkedLayerChanged() { bLinkedLayerChanged = false; }

protected:
	void UpdateLocationData(float DeltaTime);

	void UpdateRotationData(float DeltaTime);

	void UpdateVelocityData(float DeltaTime);

	void UpdateAccelerationData(float DeltaTime);

	void UpdateCharacterStateData(float DeltaTime);

	void UpdateBlendWeightData(float DeltaTime);

	void UpdateAimData(float DeltaTime);

	void UpdateJumpFallData(float DeltaTime);

	void UpdateWallDetectionHeuristic(float DeltaTime);

	void UpdateUpperBodyAlpha(float DeltaTime);

	void UpdateInWall();

	void UpdateCheckAnimValid(float DeltaTime);

protected:
	UPROPERTY(BlueprintReadOnly, meta=(BlueprintThreadSafe))
	TObjectPtr<ACharacter> OwnerCharacter;

	UPROPERTY(BlueprintReadOnly, meta=(BlueprintThreadSafe))
	TObjectPtr<UCharacterMovementComponent> OwnerCharacterMovement;

	UPROPERTY(BlueprintReadOnly, meta=(BlueprintThreadSafe))
	TObjectPtr<USkeletalMeshComponent> OwnerMesh;

	UPROPERTY(BlueprintReadOnly, meta = (BlueprintThreadSafe))
	FGameplayTag Gait = L2MoverGait::Running;

	UPROPERTY(BlueprintReadOnly, meta = (BlueprintThreadSafe))
	FGameplayTag RotationMode = L2MoverRotationMode::VelocityDirection;

	UPROPERTY(BlueprintReadOnly, meta = (BlueprintThreadSafe))
	bool bRotationModeChanged = false;

	UPROPERTY(BlueprintReadOnly, meta = (BlueprintThreadSafe))
	bool bMovementModeChanged = false;

	UPROPERTY(BlueprintReadOnly, meta = (BlueprintThreadSafe))
	FGameplayTag MovementSetName = FGameplayTag::EmptyTag;

	// --------------------------------------------- Functions --------------------------------------------- //
public:
	//当前角色速度方向是否与回转运动方向垂直
	UFUNCTION(BlueprintPure, meta = (BlueprintThreadSafe), Category = "Helper Functions")
	bool IsMovingPerpendicularToInitialPivot() const;

	UFUNCTION(BlueprintCallable, meta = (BlueprintThreadSafe), Category = "Turn In Place")
	void SetRootYawOffset(float InRootYawOffset);

	UFUNCTION(BlueprintCallable, meta = (BlueprintThreadSafe), Category = "Turn In Place")
	void ProcessTurnYawCurve();

	UFUNCTION(BlueprintCallable, meta = (BlueprintThreadSafe), Category = "Turn In Place")
	void ProcessTurnYawCurve_VelocityDirection();

	UFUNCTION(BlueprintCallable, meta = (BlueprintThreadSafe), Category = "Turn In Place")
	void UpdateRootYawOffset(float DeltaTime);
	// -- End Helper Functions

	// --  Begin State Node Functions
	UFUNCTION(BlueprintCallable, meta = (AnimBlueprintFunction, BlueprintThreadSafe), Category = "State Node Functions")
	void OnUpdate_UpdateIdleState(const FAnimUpdateContext& Context, const FAnimNodeReference& Node);

	UFUNCTION(BlueprintCallable, meta = (AnimBlueprintFunction, BlueprintThreadSafe), Category = "State Node Functions")
	void OnBecomeRelevant_SetUpStartState(const FAnimUpdateContext& Context, const FAnimNodeReference& Node);

	UFUNCTION(BlueprintCallable, meta = (AnimBlueprintFunction, BlueprintThreadSafe), Category = "State Node Functions")
	void OnUpdate_UpdateStartState(const FAnimUpdateContext& Context, const FAnimNodeReference& Node);

	UFUNCTION(BlueprintCallable, meta = (AnimBlueprintFunction, BlueprintThreadSafe), Category = "State Node Functions")
	void OnUpdate_UpdateStopState(const FAnimUpdateContext& Context, const FAnimNodeReference& Node);

	UFUNCTION(BlueprintCallable, meta = (AnimBlueprintFunction, BlueprintThreadSafe), Category = "State Node Functions")
	void OnBecomeRelevant_SetUpPivotState(const FAnimUpdateContext& Context, const FAnimNodeReference& Node);

	UFUNCTION(BlueprintCallable, meta = (AnimBlueprintFunction, BlueprintThreadSafe), Category = "State Node Functions")
	void OnUpdate_UpdatePivotState(const FAnimUpdateContext& Context, const FAnimNodeReference& Node);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, meta = (AnimBlueprintFunction, BlueprintThreadSafe),
		Category = "State Node Functions")
	void OnUpdate_UpdateLocomotionStateMachine(const FAnimUpdateContext& Context, const FAnimNodeReference& Node);

	UFUNCTION(BlueprintCallable, meta = (AnimBlueprintFunction, BlueprintThreadSafe), Category = "State Node Functions")
	void OnBecomeRelevant_SetUpIdleState_IdleSM(const FAnimUpdateContext& Context, const FAnimNodeReference& Node);

	UFUNCTION(BlueprintCallable, meta = (AnimBlueprintFunction, BlueprintThreadSafe), Category = "State Node Functions")
	void OnUpdate_UpdateIdleState_IdleSM(const FAnimUpdateContext& Context, const FAnimNodeReference& Node);

	UFUNCTION(BlueprintCallable, meta = (AnimBlueprintFunction, BlueprintThreadSafe), Category = "State Node Functions")
	void OnBecomeRelevant_LandRecoveryState(const FAnimUpdateContext& Context, const FAnimNodeReference& Node);

	UFUNCTION(BlueprintCallable, meta = (AnimBlueprintFunction, BlueprintThreadSafe), Category = "State Node Functions")
	void OnBecomeRelevant_SetUpIdleTransition(const FAnimUpdateContext& Context, const FAnimNodeReference& Node);
	// -- End State Node Functions


	// --  Begin Anim Node Functions
	UFUNCTION(BlueprintCallable, meta = (AnimBlueprintFunction, BlueprintThreadSafe), Category = "Anim Node Functions")
	void OnUpdate_UpdateIdleAnim(const FAnimUpdateContext& Context, const FAnimNodeReference& Node);

	UFUNCTION(BlueprintCallable, meta = (AnimBlueprintFunction, BlueprintThreadSafe), Category = "Anim Node Functions")
	void OnBecomeRelevant_SetUpIdleBreakAnim(const FAnimUpdateContext& Context, const FAnimNodeReference& Node);

	UFUNCTION(BlueprintCallable, meta = (AnimBlueprintFunction, BlueprintThreadSafe), Category = "Anim Node Functions")
	void OnBecomeRelevant_SetUpStartAnim(const FAnimUpdateContext& Context, const FAnimNodeReference& Node);

	UFUNCTION(BlueprintCallable, meta = (AnimBlueprintFunction, BlueprintThreadSafe), Category = "Anim Node Functions")
	void OnUpdate_UpdateStartAnim(const FAnimUpdateContext& Context, const FAnimNodeReference& Node);

	UFUNCTION(BlueprintCallable, meta = (AnimBlueprintFunction, BlueprintThreadSafe), Category = "Anim Node Functions")
	void OnUpdate_UpdateCycleAnim(const FAnimUpdateContext& Context, const FAnimNodeReference& Node);

	UFUNCTION(BlueprintCallable, meta = (AnimBlueprintFunction, BlueprintThreadSafe), Category = "Anim Node Functions")
	void OnBecomeRelevant_SetUpStopAnim(const FAnimUpdateContext& Context, const FAnimNodeReference& Node);

	UFUNCTION(BlueprintCallable, meta = (AnimBlueprintFunction, BlueprintThreadSafe), Category = "Anim Node Functions")
	void OnUpdate_UpdateStopAnim(const FAnimUpdateContext& Context, const FAnimNodeReference& Node);

	UFUNCTION(BlueprintCallable, meta = (AnimBlueprintFunction, BlueprintThreadSafe), Category = "Anim Node Functions")
	void OnBecomeRelevant_SetUpPivotAnim(const FAnimUpdateContext& Context, const FAnimNodeReference& Node);

	UFUNCTION(BlueprintCallable, meta = (AnimBlueprintFunction, BlueprintThreadSafe), Category = "Anim Node Functions")
	void OnUpdate_UpdatePivotAnim(const FAnimUpdateContext& Context, const FAnimNodeReference& Node);

	UFUNCTION(BlueprintCallable, meta = (AnimBlueprintFunction, BlueprintThreadSafe), Category = "Anim Node Functions")
	void OnBecomeRelevant_SetUpFallLandAnim(const FAnimUpdateContext& Context, const FAnimNodeReference& Node);

	UFUNCTION(BlueprintCallable, meta = (AnimBlueprintFunction, BlueprintThreadSafe), Category = "Anim Node Functions")
	void OnUpdate_UpdateFallLandAnim(const FAnimUpdateContext& Context, const FAnimNodeReference& Node);
	// -- End Anim Node Functions


	// --  Turn In Place
	UFUNCTION(BlueprintCallable, meta = (AnimBlueprintFunction, BlueprintThreadSafe), Category = "Turn In Place")
	void OnBecomeRelevant_SetUpTurnInPlaceAnim(const FAnimUpdateContext& Context, const FAnimNodeReference& Node);

	UFUNCTION(BlueprintCallable, meta = (AnimBlueprintFunction, BlueprintThreadSafe), Category = "Turn In Place")
	void OnUpdate_UpdateTurnInPlaceAnim(const FAnimUpdateContext& Context, const FAnimNodeReference& Node);

	UFUNCTION(BlueprintCallable, meta = (AnimBlueprintFunction, BlueprintThreadSafe), Category = "Turn In Place")
	void OnBecomeRelevant_SetUpTurnInPlaceRotationState(const FAnimUpdateContext& Context,
	                                                    const FAnimNodeReference& Node);

	UFUNCTION(BlueprintCallable, meta = (AnimBlueprintFunction, BlueprintThreadSafe), Category = "Turn In Place")
	void OnBecomeRelevant_SetUpTurnInPlaceRecoveryState(const FAnimUpdateContext& Context,
	                                                    const FAnimNodeReference& Node);

	UFUNCTION(BlueprintCallable, meta = (AnimBlueprintFunction, BlueprintThreadSafe), Category = "Turn In Place")
	void OnUpdate_UpdateTurnInPlaceRecoveryAnim(const FAnimUpdateContext& Context, const FAnimNodeReference& Node);

	UFUNCTION(BlueprintCallable, meta = (BlueprintThreadSafe), Category = "Turn In Place")
	UAnimSequence* SelectTurnInPlaceAnimation(float Direction);

	// --  Idle Breaks

	UFUNCTION(BlueprintPure, meta = (BlueprintThreadSafe), Category = "Idle Breaks")
	bool CanPlayIdleBreak() const;

	UFUNCTION(BlueprintCallable, meta = (BlueprintThreadSafe), Category = "Idle Breaks")
	void ResetIdleBreakTransitionLogic();

	UFUNCTION(BlueprintCallable, meta = (BlueprintThreadSafe), Category = "Idle Breaks")
	void ProcessIdleBreakTransitionLogic(float DeltaTime);

	UFUNCTION(BlueprintCallable, meta = (BlueprintThreadSafe), Category = "Idle Breaks")
	void ChooseIdleBreakDelayTime();


	// --  Distance Matching 距离匹配 
	UFUNCTION(BlueprintPure, meta = (BlueprintThreadSafe), Category = "Distance Matching")
	float GetPredictedStopDistance() const;

	UFUNCTION(BlueprintPure, meta = (BlueprintThreadSafe), Category = "Distance Matching")
	bool ShouldDistanceMatchStop() const;


	// --  Pivots

	UFUNCTION(BlueprintPure, meta = (BlueprintThreadSafe), Category = "Pivots")
	UAnimSequenceBase* GetDesiredPivotSequence(EL2MovementDirection InDirection) const;


	// --------------------------------------------- Anim Data --------------------------------------------- //
	// --  动画数据
	UFUNCTION(BlueprintPure, meta = (BlueprintThreadSafe), Category = "Animation Data")
	UL2MovementDefinition* GetCurrentMovementDefinition() const;

	FL2MoverMovementAnimSettings* GetCurrentMovementAnimSettings() const;

	UFUNCTION(BlueprintPure, meta = (BlueprintThreadSafe), Category = "Animation Data")
	FL2Mover_AnimData_Idle GetMovementFragment_Idle() const;

	UFUNCTION(BlueprintPure, meta = (BlueprintThreadSafe), Category = "Animation Data")
	FL2Mover_AnimData_Start_VelocityDirection GetMovementFragment_Start() const;

	UFUNCTION(BlueprintPure, meta = (BlueprintThreadSafe), Category = "Animation Data")
	FL2Mover_AnimData_Start_LookingDirection GetMovementFragment_StartForwardFacing() const;

	UFUNCTION(BlueprintPure, meta = (BlueprintThreadSafe), Category = "Animation Data")
	FL2Mover_AnimData_Cycle GetMovementFragment_Cycle() const;

	UFUNCTION(BlueprintPure, meta = (BlueprintThreadSafe), Category = "Animation Data")
	FL2Mover_AnimData_Stop GetMovementFragment_Stop() const;

	UFUNCTION(BlueprintPure, meta = (BlueprintThreadSafe), Category = "Animation Data")
	FL2Mover_AnimData_Sprint GetMovementFragment_Sprint() const;

	UFUNCTION(BlueprintPure, meta = (BlueprintThreadSafe), Category = "Animation Data")
	FL2Mover_AnimData_Pivot GetMovementFragment_Pivot() const;

	UFUNCTION(BlueprintPure, meta = (BlueprintThreadSafe), Category = "Animation Data")
	FL2Mover_AnimData_TurnInPlace GetMovementFragment_TurnInPlace() const;

	UFUNCTION(BlueprintPure, meta = (BlueprintThreadSafe), Category = "Animation Data")
	FL2Mover_AnimData_Jump GetMovementFragment_Jump() const;

	UFUNCTION(BlueprintPure, meta = (BlueprintThreadSafe), Category = "Animation Data")
	FL2Mover_AnimData_AimOffset GetMovementFragment_AimOffset() const;

	UFUNCTION(BlueprintPure, meta = (BlueprintThreadSafe), Category = "Animation Data")
	TArray<UAnimSequence*> GetIdleBreaks() const;

	UFUNCTION(BlueprintPure, meta = (BlueprintThreadSafe), Category = "Animation Data")
	UAnimSequenceBase* GetStartAnim() const;

	UFUNCTION(BlueprintPure, meta = (BlueprintThreadSafe), Category = "Animation Data")
	UAnimSequenceBase* GetCycleAnim() const;

	UFUNCTION(BlueprintPure, meta = (BlueprintThreadSafe), Category = "Animation Data")
	UAnimSequenceBase* GetStopAnim() const;

	UFUNCTION(BlueprintPure, meta = (BlueprintThreadSafe), Category = "Animation Data")
	UAnimSequenceBase* GetPivotAnim() const;

	FVector2D GetPlayRateClampStart() const;

	FVector2D GetPlayRateClampCycle() const;

	FVector2D GetPlayRateStopCycle() const;

	FVector2D GetPlayRateClampPivot() const;


	// --------------------------------------------- Property --------------------------------------------- //
	//Default
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BlueprintThreadSafe), Category = "Default")
	bool bFirstUpdate = true;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BlueprintThreadSafe), Category = "Default")
	bool bEnableFootIK = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BlueprintThreadSafe), Category = "Default")
	float IKAlpha = 0.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BlueprintThreadSafe), Category = "Default")
	bool bEnableRootYawOffset = true;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BlueprintThreadSafe), Category = "Default")
	float UpperBodyAlpha = 0.f;


	//Location Data
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BlueprintThreadSafe), Category = "Location Data")
	bool bShouldMove = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BlueprintThreadSafe), Category = "Location Data")
	FVector WorldLocation = FVector(0);

	//自上次更新以来的位移
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BlueprintThreadSafe), Category = "Location Data")
	float DisplacementSinceLastUpdate = 0.f;

	//位移速度 （平均速度）
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BlueprintThreadSafe), Category = "Location Data")
	float DisplacementSpeed = 0.f;


	//Rotation Data
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BlueprintThreadSafe), Category = "Rotation Data")
	FRotator WorldRotation = FRotator(0);

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BlueprintThreadSafe), Category = "Rotation Data")
	float YawDeltaSinceLastUpdate = 0.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BlueprintThreadSafe), Category = "Rotation Data")
	float AdditiveLeanAngle = 0.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BlueprintThreadSafe), Category = "Rotation Data")
	float YawDeltaSpeed = 0.f;


	//Velocity Data
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BlueprintThreadSafe), Category = "Velocity Data")
	bool bHasVelocity = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BlueprintThreadSafe), Category = "Velocity Data")
	FVector WorldVelocity = FVector(0);

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BlueprintThreadSafe), Category = "Velocity Data")
	FVector LocalVelocity2D = FVector(0);

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BlueprintThreadSafe), Category = "Velocity Data")
	float LocalVelocityDirectionAngle = 0.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BlueprintThreadSafe), Category = "Velocity Data")
	float LocalVelocityDirectionAngleWithOffset = 0.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BlueprintThreadSafe), Category = "Velocity Data")
	EL2MovementDirection LocalVelocityDirection = EL2MovementDirection::Forward;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BlueprintThreadSafe), Category = "Velocity Data")
	EL2MovementDirection LocalVelocityDirectionNoOffset = EL2MovementDirection::Forward;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BlueprintThreadSafe), Category = "Velocity Data")
	float StopLocalVelocityDirectionAngle = 0.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BlueprintThreadSafe), Category = "Velocity Data")
	EL2MovementDirection StopVelocityDirection = EL2MovementDirection::Forward;


	//Acceleration Data
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BlueprintThreadSafe), Category = "Acceleration Data")
	FVector LocalAcceleration2D = FVector(0);

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BlueprintThreadSafe), Category = "Acceleration Data")
	bool bHasAcceleration = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BlueprintThreadSafe), Category = "Acceleration Data")
	FVector PivotDirection2D = FVector(0);

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BlueprintThreadSafe), Category = "Acceleration Data")
	FVector LastVelocity = FVector(0);


	//Character State Data
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BlueprintThreadSafe), Category = "Character State Data")
	bool bIsOnGround = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BlueprintThreadSafe), Category = "Character State Data")
	bool bIsCrouching = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BlueprintThreadSafe), Category = "Character State Data")
	bool bCrouchStateChange = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BlueprintThreadSafe), Category = "Character State Data")
	bool bIsJumping = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BlueprintThreadSafe), Category = "Character State Data")
	bool bIsFalling = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BlueprintThreadSafe), Category = "Character State Data")
	bool bIsSprinting = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BlueprintThreadSafe), Category = "Character State Data")
	bool bIsRunningIntoWall = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BlueprintThreadSafe), Category = "Character State Data")
	bool bInWall = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BlueprintThreadSafe), Category = "Character State Data")
	float TimeToJumpApex = 0.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BlueprintThreadSafe), Category = "Character State Data")
	float GroundDistance = -1.f;


	//Locomotion SM Data
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BlueprintThreadSafe), Category = "Locomotion SM  Data")
	EL2MovementDirection StartDirection = EL2MovementDirection::Forward;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BlueprintThreadSafe), Category = "Locomotion SM  Data")
	EL2MovementDirection PivotInitialDirection = EL2MovementDirection::Forward;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BlueprintThreadSafe), Category = "Locomotion SM  Data")
	float LastPivotTime = 0.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BlueprintThreadSafe), Category = "Locomotion SM  Data")
	EL2MovementDirection CardinalDirectionFromAcceleration = EL2MovementDirection::Forward;


	//Blend Weight Data
public:
	//上半身动态附加权重
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BlueprintThreadSafe), Category = "Blend Weight Data")
	float UpperBodyDynamicAdditiveWeight = 0.f;


	//Aiming Data
public:
	UPROPERTY(BlueprintReadWrite, meta = (BlueprintThreadSafe), Category = "Aiming Data")
	float AimPitch = 0.f;

	UPROPERTY(BlueprintReadWrite, meta = (BlueprintThreadSafe), Category = "Aiming Data")
	float AimYaw = 0.f;

	UPROPERTY(BlueprintReadWrite, meta = (BlueprintThreadSafe), Category = "Aiming Data")
	float AimOffsetBlendWeight = 1.f;


	//Turn In Place
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BlueprintThreadSafe), Category = "Turn In Place")
	bool bTurnInPlaceB = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BlueprintThreadSafe), Category = "Turn In Place")
	bool bReachedEndOfTurn = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BlueprintThreadSafe), Category = "Turn In Place")
	float RootYawOffset = 0.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BlueprintThreadSafe), Category = "Turn In Place")
	float TurnYawCurveValue = 0.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BlueprintThreadSafe), Category = "Turn In Place")
	float MaxTurnYawValue = 0.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BlueprintThreadSafe))
	EL2RootYawOffsetMode RootYawOffsetMode = EL2RootYawOffsetMode::BlendOut;


	//Settings
public:
	//基本方向死区值
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BlueprintThreadSafe), Category = "Settings")
	float CardinalDirectionDeadZone = 10.f;


	//Linked Layer Data
public:
	//链接动画层发生变化
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BlueprintThreadSafe), Category = "Linked Layer Data")
	bool bLinkedLayerChanged = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BlueprintThreadSafe), Category = "Linked Layer Data")
	TObjectPtr<UAnimInstance> LastLinkedLayer = nullptr;


	//Check Anim Valid
public:
	UPROPERTY(BlueprintReadOnly, meta = (BlueprintThreadSafe), Category = "Check Anim Valid")
	bool bIsAnimValid_Idle = false;

	UPROPERTY(BlueprintReadOnly, meta = (BlueprintThreadSafe), Category = "Check Anim Valid")
	bool bIsAnimValid_Start = false;

	UPROPERTY(BlueprintReadOnly, meta = (BlueprintThreadSafe), Category = "Check Anim Valid")
	bool bIsAnimValid_StartForwardFacing = false;

	UPROPERTY(BlueprintReadOnly, meta = (BlueprintThreadSafe), Category = "Check Anim Valid")
	bool bIsAnimValid_Cycle = false;

	UPROPERTY(BlueprintReadOnly, meta = (BlueprintThreadSafe), Category = "Check Anim Valid")
	bool bIsAnimValid_Stop = false;

	UPROPERTY(BlueprintReadOnly, meta = (BlueprintThreadSafe), Category = "Check Anim Valid")
	bool bIsAnimValid_Pivot = false;

	UPROPERTY(BlueprintReadOnly, meta = (BlueprintThreadSafe), Category = "Check Anim Valid")
	bool bIsAnimValid_JumpApex = false;

	UPROPERTY(BlueprintReadOnly, meta = (BlueprintThreadSafe), Category = "Check Anim Valid")
	bool bIsAnimValid_JumpFallLand = false;

	UPROPERTY(BlueprintReadOnly, meta = (BlueprintThreadSafe), Category = "Check Anim Valid")
	bool bIsAnimValid_JumpStartLoop = false;

	UPROPERTY(BlueprintReadOnly, meta = (BlueprintThreadSafe), Category = "Check Anim Valid")
	bool bIsAnimValid_AimOffset = false;


	//Idle Breaks
public:
	//距离下一次空闲时间的时间
	UPROPERTY(BlueprintReadWrite, meta = (BlueprintThreadSafe), Category = "Idle Breaks")
	float TimeUntilNextIdleBreak = 0.f;

	//当前空闲休息动画的下标Index
	UPROPERTY(BlueprintReadWrite, meta = (BlueprintThreadSafe), Category = "Idle Breaks")
	int32 CurrentIdleBreakIndex = 0;

	//空闲休息延迟时间
	//tips: 此时间需要与角色的Location信息相关联，来产生不同的冷却时间，表达多个角色在空闲休息时的随机性
	UPROPERTY(BlueprintReadWrite, meta = (BlueprintThreadSafe), Category = "Idle Breaks")
	float IdleBreakDelayTime = 0.f;


	//Jump
public:
	//落地恢复叠加权重
	UPROPERTY(BlueprintReadWrite, meta = (BlueprintThreadSafe), Category = "Jump")
	float LandRecoveryAdditiveAlpha = 0.f;

	//下落时间
	UPROPERTY(BlueprintReadWrite, meta = (BlueprintThreadSafe), Category = "Jump")
	float TimeFalling = 0.f;


	//Pivot
public:
	//回转运动起步加速度
	UPROPERTY(BlueprintReadWrite, meta = (BlueprintThreadSafe), Category = "Pivot")
	FVector PivotStartingAcceleration = FVector::ZeroVector;

	//回转运动停止的时间
	UPROPERTY(BlueprintReadWrite, meta = (BlueprintThreadSafe), Category = "Pivot")
	float TimeAtPivotStop = 0.f;


	//Turn In Place
public:
	//原地转身动画时间
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BlueprintThreadSafe), Category = "Turn In Place")
	float TurnInPlaceAnimTime = 0.f;

	//原地转身旋转方向
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BlueprintThreadSafe), Category = "Turn In Place")
	float TurnInPlaceRotationDirection = 0.f;

	//原地转身恢复方向 （角色恢复到相机朝向的方向）
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BlueprintThreadSafe), Category = "Turn In Place")
	float TurnInPlaceRecoveryDirection = 0.f;


	//Stride Warping (步幅匹配)
public:
	UPROPERTY(BlueprintReadWrite, meta = (BlueprintThreadSafe), Category = "Stride Warping")
	float StrideWarpingStartAlpha = 0.f;

	UPROPERTY(BlueprintReadWrite, meta = (BlueprintThreadSafe), Category = "Stride Warping")
	float StrideWarpingCycleAlpha = 0.f;

	UPROPERTY(BlueprintReadWrite, meta = (BlueprintThreadSafe), Category = "Stride Warping")
	float StrideWarpingStopAlpha = 0.f;

	UPROPERTY(BlueprintReadWrite, meta = (BlueprintThreadSafe), Category = "Stride Warping")
	float StrideWarpingPivotAlpha = 0.f;

	//步幅匹配混合时间
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BlueprintThreadSafe), Category = "Data")
	float StrideWarpingBlendInDurationScaled = 0.2;

	//步幅匹配混合起始偏移
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BlueprintThreadSafe), Category = "Data")
	float StrideWarpingBlendInStartOffset = 0.15;
};
