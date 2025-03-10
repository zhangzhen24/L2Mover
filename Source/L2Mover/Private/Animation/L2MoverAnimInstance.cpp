// Fill out your copyright notice in the Description page of Project Settings.

#include "Animation/L2MoverAnimInstance.h"

#include "AnimationStateMachineLibrary.h"
#include "AnimCharacterMovementLibrary.h"
#include "AnimDistanceMatchingLibrary.h"
#include "AnimExecutionContextLibrary.h"
#include "KismetAnimationLibrary.h"
#include "L2MoverComponent.h"
#include "LinkedAnimGraphLibrary.h"
#include "LogL2Mover.h"
#include "SequenceEvaluatorLibrary.h"
#include "SequencePlayerLibrary.h"
#include "Data/L2MovementDefinition.h"
#include "Data/L2MovementFragment.h"
#include "Data/L2MoverMovementEnums.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Utility/L2MoverUtility.h"


UL2MoverAnimInstance::UL2MoverAnimInstance(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UL2MoverAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();
}

void UL2MoverAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	GetWorld()->GetTimerManager().SetTimerForNextTick(this, &ThisClass::Initialization);
}

void UL2MoverAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	//更新bInWall
	if (bIsRunningIntoWall)
	{
		GetWorld()->GetTimerManager().SetTimerForNextTick(this, &ThisClass::UpdateInWall);
	}
	else
	{
		bInWall = false;
	}
}

void UL2MoverAnimInstance::NativeThreadSafeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeThreadSafeUpdateAnimation(DeltaSeconds);


	if (!IsValid(OwnerCharacter))
	{
		OwnerCharacter = Cast<ACharacter>(TryGetPawnOwner());
		return;
	}

	if (!IsValid(OwnerMesh))
	{
		OwnerMesh = OwnerCharacter->GetMesh();
		return;
	}

	if (!IsValid(OwnerCharacterMovement))
	{
		OwnerCharacterMovement = OwnerCharacter->GetCharacterMovement();
		return;
	}

	if (!IsValid(L2MoverComponent))
	{
		L2MoverComponent = OwnerCharacter->FindComponentByClass<UL2MoverComponent>();
		return;
	}


	//Character State
	Gait = L2MoverComponent->GetGait();
	RotationMode = L2MoverComponent->GetRotationMode();

	UpdateLocationData(DeltaSeconds);
	UpdateRotationData(DeltaSeconds);
	UpdateVelocityData(DeltaSeconds);
	UpdateAccelerationData(DeltaSeconds);
	UpdateCharacterStateData(DeltaSeconds);
	UpdateWallDetectionHeuristic(DeltaSeconds);
	UpdateUpperBodyAlpha(DeltaSeconds);
	UpdateRootYawOffset(DeltaSeconds);
	UpdateAimData(DeltaSeconds);
	UpdateJumpFallData(DeltaSeconds);
	CheckJumpAnimValid(DeltaSeconds);
	CheckAimOffsetAnimValid(DeltaSeconds);

	//是否开启ControlRig
	bEnableFootIK = GetCurveValue(L2Mover_AnimCurveName::DisableLegIK) > 0.f;
	IKAlpha = UKismetMathLibrary::FInterpTo(IKAlpha, bEnableFootIK ? 1.f : 0.f, DeltaSeconds, 10.f);

	bFirstUpdate = false;
}

void UL2MoverAnimInstance::UpdateInWall()
{
	if (bIsRunningIntoWall)
	{
		bInWall = true;
	}
}

void UL2MoverAnimInstance::Initialization()
{
	OwnerCharacter = Cast<ACharacter>(TryGetPawnOwner());
	if (OwnerCharacter)
	{
		OwnerMesh = OwnerCharacter->GetMesh();
		OwnerCharacterMovement = OwnerCharacter->GetCharacterMovement();
		if (!L2MoverComponent)
		{
			L2MoverComponent = OwnerCharacter->FindComponentByClass<UL2MoverComponent>();
		}

		if (!L2MoverComponent)
		{
			L2MOVER_LOG(Error, TEXT("%s: Can not found L2MoverComponent in %s."), ANSI_TO_TCHAR(__FUNCTION__), *OwnerCharacter->GetName());
			return;
		}

		if (L2MoverComponent)
		{
			L2MoverComponent->OnUpdateMovementDefinition.AddUniqueDynamic(this, &ThisClass::OnUpdateMovementDefinition);
			L2MoverComponent->OnUpdateRotationMode.AddUniqueDynamic(this, &ThisClass::OnUpdateRotationMode);
			L2MoverComponent->OnUpdateGait.AddUniqueDynamic(this, &ThisClass::OnUpdateGait);
			L2MoverComponent->OnUpdateAccelerationChanged.AddUniqueDynamic(this, &ThisClass::OnAccelerationChanged);
			L2MoverComponent->OnUpdateRootYawOffsetStatus.AddUniqueDynamic(this, &ThisClass::OnUpdateRootYawOffsetStatus);

			OwnerCharacter->MovementModeChangedDelegate.AddUniqueDynamic(this, &ThisClass::OnMovementModeChanged);

			InitEvents();
		}
	}
}

void UL2MoverAnimInstance::InitEvents()
{
	OnAccelerationChanged(bHasAcceleration, LocalVelocityDirectionAngleWithOffset);
	OnUpdateRotationMode(L2MoverComponent->RotationMode);
	OnUpdateMovementDefinition(L2MoverComponent->GetCurrentMovementDefinition());
	OnUpdateRootYawOffsetStatus(bEnableRootYawOffset);
}

void UL2MoverAnimInstance::SetAnimationData()
{
}

void UL2MoverAnimInstance::SetGroundDistance()
{
}

void UL2MoverAnimInstance::OnUpdateGait(const FGameplayTag& InGait)
{
}

void UL2MoverAnimInstance::OnUpdateRotationMode(const FGameplayTag& InRotationMode)
{
}

void UL2MoverAnimInstance::OnUpdateMovementDefinition(UL2MovementDefinition* InMovementDefinition)
{
}

void UL2MoverAnimInstance::OnAccelerationChanged(const bool InHasAcceleration, const double InLastVelocityDirection)
{
}

void UL2MoverAnimInstance::OnUpdateRootYawOffsetStatus(const bool bInEnableRootYawOffset)
{
}

void UL2MoverAnimInstance::OnMovementModeChanged(ACharacter* InCharacter, EMovementMode InPrevMovementMode, uint8 InPreviousCustomMode)
{
}

void UL2MoverAnimInstance::ResetMovementStateChanged()
{
}


void UL2MoverAnimInstance::UpdateLocationData(float DeltaTime)
{
	//自上次更新以来的位移
	DisplacementSinceLastUpdate = (OwnerCharacter->GetActorLocation() - WorldLocation).Size2D();
	//世界位置
	WorldLocation = OwnerCharacter->GetActorLocation();
	//位移速度
	DisplacementSpeed = UKismetMathLibrary::SafeDivide(DisplacementSinceLastUpdate, DeltaTime);

	if (bFirstUpdate)
	{
		DisplacementSinceLastUpdate = 0.f;
		DisplacementSpeed = 0.f;
	}
}

void UL2MoverAnimInstance::UpdateRotationData(float DeltaTime)
{
	//自上次更新以来的Yaw增量
	YawDeltaSinceLastUpdate = (OwnerCharacter->GetActorRotation().Yaw - WorldRotation.Yaw);
	//Yaw增量速度
	YawDeltaSpeed = UKismetMathLibrary::SafeDivide(YawDeltaSinceLastUpdate, DeltaTime);
	//世界旋转
	WorldRotation = OwnerCharacter->GetActorRotation();
	//附加倾斜角
	AdditiveLeanAngle = YawDeltaSpeed * bIsCrouching ? 0.025f : 0.0375f; //倾斜角度

	if (bFirstUpdate)
	{
		YawDeltaSinceLastUpdate = 0.f;
		YawDeltaSpeed = 0.f;
	}
}

void UL2MoverAnimInstance::UpdateVelocityData(float DeltaTime)
{
	bool bWasMovingLastUpdate = !UKismetMathLibrary::Vector_IsZero(LocalVelocity2D);
	//世界速度
	WorldVelocity = OwnerCharacter->GetVelocity();
	//世界速度2D
	FVector WorldVelocity2D = FVector(WorldVelocity.X, WorldVelocity.Y, 0.f);
	//局部速度2D
	LocalVelocity2D = UKismetMathLibrary::LessLess_VectorRotator(WorldVelocity2D, WorldRotation);
	//局部速度方向角
	LocalVelocityDirectionAngle = UKismetAnimationLibrary::CalculateDirection(WorldVelocity2D, WorldRotation);
	//具有偏移值的 局部速度方向角
	LocalVelocityDirectionAngleWithOffset = LocalVelocityDirectionAngle - RootYawOffset;
	//局部速度方向
	LocalVelocityDirection = UL2MoverUtility::SelectCardinalDirectionFromAngle(LocalVelocityDirectionAngleWithOffset, CardinalDirectionDeadZone, LocalVelocityDirection, bWasMovingLastUpdate);
	//没有偏移值的局部速度方向
	LocalVelocityDirectionNoOffset = UL2MoverUtility::SelectCardinalDirectionFromAngle(LocalVelocityDirectionAngle, CardinalDirectionDeadZone, LocalVelocityDirectionNoOffset, bWasMovingLastUpdate);
	//是否有速度
	bHasVelocity = !UKismetMathLibrary::NearlyEqual_FloatFloat(LocalVelocity2D.SizeSquared2D(), 0.0f, 0.000001);
}

void UL2MoverAnimInstance::UpdateAccelerationData(float DeltaTime)
{
	//世界加速度
	FVector WorldAcceleration = OwnerCharacterMovement->GetCurrentAcceleration();
	//世界加速度2D
	FVector WorldAcceleration2D = WorldAcceleration * FVector(1.f, 1.f, 0.f);
	//局部加速度2D
	LocalAcceleration2D = UKismetMathLibrary::LessLess_VectorRotator(WorldAcceleration2D, WorldRotation);
	//是否有加速度
	bHasAcceleration = !UKismetMathLibrary::NearlyEqual_FloatFloat(LocalAcceleration2D.SizeSquared2D(), 0.0f, 0.000001);
	//枢轴方向2D
	//tips: 从加速度计算一个用于轴的基本方向。加速度比速度更能传达玩家意图。
	PivotDirection2D = UKismetMathLibrary::VLerp(PivotDirection2D, WorldAcceleration2D.GetSafeNormal(0.0001), 0.5f).GetSafeNormal(0.0001);
	//加速度的基本方向
	const double Angle = UKismetAnimationLibrary::CalculateDirection(PivotDirection2D, WorldRotation);
	CardinalDirectionFromAcceleration = UL2MoverUtility::SelectCardinalDirectionFromAngle(Angle, CardinalDirectionDeadZone, EL2MovementDirection::Forward, false);
	CardinalDirectionFromAcceleration = UL2MoverUtility::GetOppositeCardinalDirection(CardinalDirectionFromAcceleration);
}

void UL2MoverAnimInstance::UpdateCharacterStateData(float DeltaTime)
{
	// On the ground
	bIsOnGround = L2MoverComponent->IsOnGround();
	bIsSprinting = L2MoverComponent->IsSprinting();

	// Crouching position
	bool WasCrouchingLastUpdate = bIsCrouching;
	bIsCrouching = OwnerCharacterMovement->IsCrouching();
	bCrouchStateChange = bIsCrouching != WasCrouchingLastUpdate;

	//In Air State
	bIsJumping = L2MoverComponent->IsJumping();
	bIsFalling = L2MoverComponent->IsFalling();
	const FCharacterGroundInfo GroundInfo = L2MoverComponent->GetGroundInfo();
	GroundDistance = GroundInfo.GroundDistance;
}

void UL2MoverAnimInstance::UpdateBlendWeightData(float DeltaTime)
{
	//上半身动态添加权重
	UpperBodyDynamicAdditiveWeight = (IsAnyMontagePlaying() && bIsOnGround) ? 1.0f : UKismetMathLibrary::FInterpTo(UpperBodyDynamicAdditiveWeight, 0.0f, DeltaTime, 6.0f);
}

void UL2MoverAnimInstance::UpdateAimData(float DeltaTime)
{
	AimPitch = UKismetMathLibrary::NormalizeAxis(OwnerCharacter->GetBaseAimRotation().Pitch);
}

void UL2MoverAnimInstance::UpdateJumpFallData(float DeltaTime)
{
	if (bIsJumping)
	{
		//Jumping
		TimeFalling = 0.f;
		TimeToJumpApex = (0 - WorldVelocity.Z) / OwnerCharacterMovement->GetGravityZ();
	}
	else
	{
		//Falling
		TimeFalling = TimeFalling + DeltaTime;
		TimeToJumpApex = 0.f;
	}
}

void UL2MoverAnimInstance::UpdateWallDetectionHeuristic(float DeltaTime)
{
	//tips: 这个逻辑通过检查速度和加速度之间是否存在较大角度来猜测角色是否会撞到墙上(例如，角色正在向墙壁推进，但实际上是向一侧滑动)，以及角色是否试图加速，但速度相对较低。
	bIsRunningIntoWall =
		LocalAcceleration2D.Size2D() > 0.1f &&
		LocalVelocity2D.Size2D() < 200.f &&
		UKismetMathLibrary::InRange_FloatFloat(
			UKismetMathLibrary::Dot_VectorVector(LocalAcceleration2D.GetSafeNormal(0.0001), LocalVelocity2D.GetSafeNormal(0.0001)),
			-0.6f, 0.6f, true, true);
}

void UL2MoverAnimInstance::UpdateUpperBodyAlpha(float DeltaTime)
{
	//Turn Yaw Weight
	float TurnYawWeightValue = GetCurveValue(L2Mover_AnimCurveName::TurnYawWeight);
	float TempValue = (TurnYawWeightValue > 0.f || bHasAcceleration) ? 1.f : 0.f;
	UpperBodyAlpha = UKismetMathLibrary::FInterpTo(UpperBodyAlpha, TempValue, DeltaTime, 10.f);
}

void UL2MoverAnimInstance::CheckJumpAnimValid(float DeltaTime)
{
	const FL2MovementFragment_Jump& JumpData = GetMovementFragment_Jump();
	bIsValid_JumpApex = IsValid(JumpData.Jump_Apex);
	bIsValid_JumpFallLand = IsValid(JumpData.Jump_FallLand);
	bIsValid_JumpStartLoop = IsValid(JumpData.Jump_StartLoop);
}

void UL2MoverAnimInstance::CheckAimOffsetAnimValid(float DeltaTime)
{
	const FL2MovementFragment_AimOffset& AimOffsetData = GetMovementFragment_AimOffset();
	bIsValid_AimOffset = IsValid(AimOffsetData.AimOffset);
}

bool UL2MoverAnimInstance::IsMovingPerpendicularToInitialPivot() const
{
	bool A = PivotInitialDirection == EL2MovementDirection::Forward || PivotInitialDirection == EL2MovementDirection::Backward;
	bool B = LocalVelocityDirection == EL2MovementDirection::Forward || LocalVelocityDirection == EL2MovementDirection::Backward;
	bool C = PivotInitialDirection == EL2MovementDirection::Left || PivotInitialDirection == EL2MovementDirection::Right;
	bool D = LocalVelocityDirection == EL2MovementDirection::Left || LocalVelocityDirection == EL2MovementDirection::Right;
	//速度方向垂直于折返运动方向
	return (A && !B) || (C && !D);
}

void UL2MoverAnimInstance::SetRootYawOffset(float InRootYawOffset)
{
	//Begin RootYawOffset
	if (bEnableRootYawOffset)
	{
		UL2MovementDefinition* MovementDefinition = GetCurrentMovementDefinition();
		if (IsValid(MovementDefinition))
		{
			FVector2D TempRootYawOffsetAngleClamp = bIsCrouching
				                                        ? MovementDefinition->BaseSettings.RootYawOffsetAngleClampCrouched
				                                        : MovementDefinition->BaseSettings.RootYawOffsetAngleClamp;
			if (TempRootYawOffsetAngleClamp.X == TempRootYawOffsetAngleClamp.Y)
			{
				RootYawOffset = UKismetMathLibrary::NormalizeAxis(InRootYawOffset);
			}
			else
			{
				RootYawOffset = UKismetMathLibrary::ClampAngle(UKismetMathLibrary::NormalizeAxis(InRootYawOffset),
				                                               TempRootYawOffsetAngleClamp.X,
				                                               TempRootYawOffsetAngleClamp.Y);
			}

			AimYaw = RootYawOffset * -1.f;
		}
	}
	else
	{
		//RootYawOffset Reset
		RootYawOffset = 0.f;
		AimYaw = 0.f;
	}
}

void UL2MoverAnimInstance::ProcessTurnYawCurve()
{
	float PreviousTurnYawCurveValue = TurnYawCurveValue;
	float ProcessTurnYawCurveValue = GetCurveValue(L2Mover_AnimCurveName::TurnYawWeight);
	if (UKismetMathLibrary::NearlyEqual_FloatFloat(ProcessTurnYawCurveValue, 0.f))
	{
		TurnYawCurveValue = 0.f;
	}
	else
	{
		TurnYawCurveValue = GetCurveValue(L2Mover_AnimCurveName::RemainingTurnYaw) / ProcessTurnYawCurveValue;

		if (PreviousTurnYawCurveValue != 0.f)
		{
			float NewRootYawOffset = RootYawOffset - (TurnYawCurveValue - PreviousTurnYawCurveValue);
			SetRootYawOffset(NewRootYawOffset);
		}
	}
}

void UL2MoverAnimInstance::ProcessTurnYawCurve_VelocityDirection()
{
	//上一个TurnYawCurve曲线值
	float PreviousTurnYawCurveValue = TurnYawCurveValue;
	//当前TurnYawCurve曲线值
	float ProcessTurnYawCurveValue = GetCurveValue(L2Mover_AnimCurveName::TurnYawWeight);

	if (UKismetMathLibrary::NearlyEqual_FloatFloat(ProcessTurnYawCurveValue, 0.f))
	{
		TurnYawCurveValue = 0.f;
		if (MaxTurnYawValue != 0)
		{
			bReachedEndOfTurn = true;
		}
	}
	else
	{
		TurnYawCurveValue = GetCurveValue(L2Mover_AnimCurveName::RemainingTurnYaw) / ProcessTurnYawCurveValue;
		if (FMath::Abs(TurnYawCurveValue) > FMath::Abs(MaxTurnYawValue))
		{
			MaxTurnYawValue = TurnYawCurveValue;
		}

		if (PreviousTurnYawCurveValue != 0.f)
		{
			float NewRootYawOffset = RootYawOffset -
				FMath::Clamp(UKismetMathLibrary::SafeDivide(LocalVelocityDirectionAngleWithOffset, TurnYawCurveValue) * (TurnYawCurveValue - PreviousTurnYawCurveValue),
				             FMath::Abs(LocalVelocityDirectionAngleWithOffset) * -1.f,
				             FMath::Abs(LocalVelocityDirectionAngleWithOffset));
			SetRootYawOffset(NewRootYawOffset);
		}
	}
}

void UL2MoverAnimInstance::UpdateRootYawOffset(float DeltaTime)
{
	//tips: 此函数根据Pawn的当前状态处理 RootYawOffset 的更新。

	//1.当脚不移动时(例如在空闲期间)，将根偏移到与Pawn所有者旋转相反的方向，以保持网格不随Pawn旋转。
	if (RootYawOffsetMode == EL2RootYawOffsetMode::Accumulate)
	{
		SetRootYawOffset(RootYawOffset - YawDeltaSinceLastUpdate);
	}

	//2.当脚移动时，根偏移将根据Pawn的旋转和速度方向进行插值。
	if (RootYawOffsetMode == EL2RootYawOffsetMode::BlendOut)
	{
		FFloatSpringState SpringState;
		//float NewRootYawOffset = UKismetMathLibrary::FInterpTo(RootYawOffset, 0.f, DeltaTime, 4.f);
		float NewRootYawOffset =
			UKismetMathLibrary::FloatSpringInterp(
				RootYawOffset, 0.0f, SpringState,
				1000.f, 0.f, DeltaTime, 1.f,
				0.5f, false, -1.f, 1.f, false);
		SetRootYawOffset(NewRootYawOffset);
	}

	//3.重置为混合偏航偏移量。每次更新，一个状态需要请求累积或保持偏移量。否则，偏移量将会消失。这主要是因为大多数状态都希望将偏移量混合在一起，这样就节省了标记每个状态的时间。
	RootYawOffsetMode = EL2RootYawOffsetMode::BlendOut;
}

void UL2MoverAnimInstance::OnUpdate_UpdateIdleState(const FAnimUpdateContext& Context, const FAnimNodeReference& Node)
{
	bool Result = false;
	FAnimationStateResultReference AnimationState;
	UAnimationStateMachineLibrary::ConvertToAnimationStateResultPure(Node, AnimationState, Result);

	//如果当前状态正在混出，则将偏移量设置为0。否则，根据当前的偏移量模式处理偏移量。
	if (UAnimationStateMachineLibrary::IsStateBlendingOut(Context, AnimationState))
	{
		if (RotationMode == L2MoverRotationMode::LookingDirection)
		{
			TurnYawCurveValue = 0.f;
		}
	}
	else
	{
		//Accumulate 累积
		RootYawOffsetMode = EL2RootYawOffsetMode::Accumulate;
		ProcessTurnYawCurve();
	}
}

void UL2MoverAnimInstance::OnBecomeRelevant_SetUpStartState(const FAnimUpdateContext& Context, const FAnimNodeReference& Node)
{
	//起步方向
	StartDirection = LocalVelocityDirection;

	//当旋转模式为速度方向时，重置转身的一些属性
	if (RotationMode == L2MoverRotationMode::VelocityDirection)
	{
		MaxTurnYawValue = 0.f;
		TurnYawCurveValue = 0.f;
		bReachedEndOfTurn = LocalVelocityDirection == EL2MovementDirection::Forward;
	}
}

void UL2MoverAnimInstance::OnUpdate_UpdateStartState(const FAnimUpdateContext& Context, const FAnimNodeReference& Node)
{
	bool Result = false;
	FAnimationStateResultReference AnimationState;
	UAnimationStateMachineLibrary::ConvertToAnimationStateResultPure(Node, AnimationState, Result);

	//退出起步状态时, 根据旋转模式处理偏移量
	if (!UAnimationStateMachineLibrary::IsStateBlendingOut(Context, AnimationState))
	{
		if (RotationMode == L2MoverRotationMode::LookingDirection)
		{
			RootYawOffsetMode = EL2RootYawOffsetMode::Hold;
		}
		else if (RotationMode == L2MoverRotationMode::VelocityDirection)
		{
			ProcessTurnYawCurve_VelocityDirection();
			RootYawOffsetMode = bReachedEndOfTurn ? EL2RootYawOffsetMode::BlendOut : EL2RootYawOffsetMode::Accumulate;
		}
	}
}

void UL2MoverAnimInstance::OnUpdate_UpdateStopState(const FAnimUpdateContext& Context, const FAnimNodeReference& Node)
{
	bool Result = false;
	FAnimationStateResultReference AnimationState;
	UAnimationStateMachineLibrary::ConvertToAnimationStateResultPure(Node, AnimationState, Result);

	//退出停步状态时，重新开始累积偏移量
	if (!UAnimationStateMachineLibrary::IsStateBlendingOut(Context, AnimationState))
	{
		RootYawOffsetMode = EL2RootYawOffsetMode::Accumulate;
	}
}

void UL2MoverAnimInstance::OnBecomeRelevant_SetUpPivotState(const FAnimUpdateContext& Context, const FAnimNodeReference& Node)
{
	//折返初始方向
	PivotInitialDirection = LocalVelocityDirection;
}

void UL2MoverAnimInstance::OnUpdate_UpdatePivotState(const FAnimUpdateContext& Context, const FAnimNodeReference& Node)
{
	//更新剩余折返时间
	if (LastPivotTime > 0.f)
	{
		LastPivotTime = LastPivotTime - UAnimExecutionContextLibrary::GetDeltaTime(Context);
	}
}

void UL2MoverAnimInstance::OnBecomeRelevant_SetUpIdleState_IdleSM(const FAnimUpdateContext& Context, const FAnimNodeReference& Node)
{
	//选择空闲动画的延迟时间
	ChooseIdleBreakDelayTime();

	//设置下次播放空闲动画的时间
	ResetIdleBreakTransitionLogic();
}


void UL2MoverAnimInstance::OnUpdate_UpdateIdleState_IdleSM(const FAnimUpdateContext& Context, const FAnimNodeReference& Node)
{
	bool Result = false;
	FAnimationStateResultReference AnimationState;
	UAnimationStateMachineLibrary::ConvertToAnimationStateResultPure(Node, AnimationState, Result);

	//当动画还未混出时，驱动空闲休息动画数据
	if (!UAnimationStateMachineLibrary::IsStateBlendingOut(Context, AnimationState))
	{
		ProcessIdleBreakTransitionLogic(UAnimExecutionContextLibrary::GetDeltaTime(Context));
	}
}

void UL2MoverAnimInstance::OnBecomeRelevant_LandRecoveryState(const FAnimUpdateContext& Context, const FAnimNodeReference& Node)
{
	const float TimeFallingClamp01 = UKismetMathLibrary::MapRangeClamped(TimeFalling, 0.f, 0.4f, 0.1f, 0.85f);
	LandRecoveryAdditiveAlpha = bIsCrouching ? TimeFallingClamp01 * 0.5f : TimeFallingClamp01;
}

void UL2MoverAnimInstance::OnBecomeRelevant_SetUpIdleTransition(const FAnimUpdateContext& Context, const FAnimNodeReference& Node)
{
	//设置蹲下 或 站起的动画
	bool bResult = false;
	FSequencePlayerReference SequencePlayer;
	USequencePlayerLibrary::ConvertToSequencePlayerPure(Node, SequencePlayer, bResult);
	if (bResult)
	{
		UL2MovementDefinition* MovementDefinition = GetCurrentMovementDefinition();
		if (IsValid(MovementDefinition))
		{
			UAnimSequenceBase* TempAnim = bIsCrouching ? MovementDefinition->GroundedSettings.Idle_To_Crouch : MovementDefinition->GroundedSettings.Crouch_To_Idle;
			USequencePlayerLibrary::SetSequence(SequencePlayer, TempAnim);
		}
	}
}


void UL2MoverAnimInstance::OnUpdate_UpdateLocomotionStateMachine_Implementation(const FAnimUpdateContext& Context, const FAnimNodeReference& Node)
{
	bool Result = false;
	FLinkedAnimGraphReference LinkedAnimGraph;

	//TODO:LayerNode, 查一下cpp代码写法 

	//TODO:这里的Node 本是需要引用 StartLayerNode，我们应该在动画蓝图中重写此函数。
	ULinkedAnimGraphLibrary::ConvertToLinkedAnimGraphPure(Node, LinkedAnimGraph, Result);
	UAnimInstance* NewAnimInstance = ULinkedAnimGraphLibrary::GetLinkedAnimInstance(LinkedAnimGraph);

	if (!bFirstUpdate)
	{
		bLinkedLayerChanged = (NewAnimInstance != LastLinkedLayer);
	}

	LastLinkedLayer = NewAnimInstance;
}

void UL2MoverAnimInstance::OnUpdate_UpdateIdleAnim(const FAnimUpdateContext& Context, const FAnimNodeReference& Node)
{
	bool bResult = false;
	FSequencePlayerReference SequencePlayer;
	USequencePlayerLibrary::ConvertToSequencePlayerPure(Node, SequencePlayer, bResult);
	if (bResult)
	{
		const FL2MovementFragment_Idle& IdleData = GetMovementFragment_Idle();
		if (IsValid(IdleData.Idle))
		{
			USequencePlayerLibrary::SetSequenceWithInertialBlending(Context, SequencePlayer, IdleData.Idle, 0.2f);
		}
	}
}

void UL2MoverAnimInstance::OnBecomeRelevant_SetUpIdleBreakAnim(const FAnimUpdateContext& Context, const FAnimNodeReference& Node)
{
	//设置 IdleBreaks 动画
	bool bResult = false;
	FSequencePlayerReference SequencePlayer;
	USequencePlayerLibrary::ConvertToSequencePlayerPure(Node, SequencePlayer, bResult);
	if (bResult)
	{
		const FL2MovementFragment_Idle& IdleData = GetMovementFragment_Idle();
		if (IdleData.Idle_Breaks.Num() > 0)
		{
			UAnimSequenceBase* TempAnim = IdleData.Idle_Breaks.IsValidIndex(CurrentIdleBreakIndex) ? IdleData.Idle_Breaks[CurrentIdleBreakIndex] : IdleData.Idle_Breaks[0];
			USequencePlayerLibrary::SetSequence(SequencePlayer, TempAnim);

			CurrentIdleBreakIndex++;
			if (CurrentIdleBreakIndex >= IdleData.Idle_Breaks.Num())
			{
				CurrentIdleBreakIndex = 0;
			}
		}
	}
}

void UL2MoverAnimInstance::OnBecomeRelevant_SetUpStartAnim(const FAnimUpdateContext& Context, const FAnimNodeReference& Node)
{
	//设置 Walk Start 动画

	bool bResult = false;
	FSequenceEvaluatorReference SequenceEvaluator;
	USequenceEvaluatorLibrary::ConvertToSequenceEvaluatorPure(Node, SequenceEvaluator, bResult);
	if (bResult)
	{
		UAnimSequenceBase* TempAnim = GetStartAnim();
		USequenceEvaluatorLibrary::SetSequence(SequenceEvaluator, TempAnim);
		//设置动画起始时间
		USequenceEvaluatorLibrary::SetExplicitTime(SequenceEvaluator, 0.0);
		//步幅匹配起始权重
		StrideWarpingStartAlpha = 0.f;
		//更新角色速度方向
		LocalVelocityDirection = RotationMode == L2MoverRotationMode::VelocityDirection
			                         ? EL2MovementDirection::Forward
			                         : LocalVelocityDirection;
	}
}

void UL2MoverAnimInstance::OnUpdate_UpdateStartAnim(const FAnimUpdateContext& Context, const FAnimNodeReference& Node)
{
	//更新 Walk Start 动画

	//这是一个使用距离匹配来确保开始动画移动的距离与Pawn所有者移动的距离匹配的例子。
	//这可以通过保持动画和运动模型同步来防止脚滑动。
	//这有效地控制了开始动画的播放速率。我们控制有效播放速率以防止动画播放太慢或太快。
	//如果有效游戏率受到限制，我们仍然会看到一些下滑。为了解决这个问题，我们稍后使用跨步扭曲来调整姿势以纠正剩余的差异。

	bool bResult = false;
	FSequenceEvaluatorReference SequenceEvaluator;
	USequenceEvaluatorLibrary::ConvertToSequenceEvaluatorPure(Node, SequenceEvaluator, bResult);
	if (bResult)
	{
		const FL2MovementFragment_Start& StartData = GetMovementFragment_Start();
		const FL2MovementFragment_Sprint& SprintData = GetMovementFragment_Sprint();
		if (StartData.IsAnimValid() && SprintData.IsAnimValid())
		{
			const float ExplicitTime = USequenceEvaluatorLibrary::GetAccumulatedTime(SequenceEvaluator);

			//Alpha = (ExplicitTime - Offset) / Duration
			StrideWarpingStartAlpha = FMath::GetMappedRangeValueClamped(FVector2D{0.f, StrideWarpingBlendInDurationScaled},
			                                                            FVector2D{0.f, 1.f},
			                                                            ExplicitTime - StrideWarpingBlendInStartOffset);

			FVector2D PlayRateClamp_Start = GetPlayRateClampStart();

			//播放速率限制
			//平滑地增加最小播放速率速度，因为我们混合了跨步扭曲
			FVector2D LocalPlayRateClamp = FVector2D(
				FMath::Lerp(StrideWarpingBlendInDurationScaled, PlayRateClamp_Start.X, StrideWarpingStartAlpha),
				PlayRateClamp_Start.Y);

			//距离匹配
			UAnimDistanceMatchingLibrary::AdvanceTimeByDistanceMatching(Context, SequenceEvaluator,
			                                                            DisplacementSinceLastUpdate,
			                                                            L2Mover_AnimCurveName::LocomotionDistance,
			                                                            LocalPlayRateClamp);
		}
	}
}

void UL2MoverAnimInstance::OnUpdate_UpdateCycleAnim(const FAnimUpdateContext& Context, const FAnimNodeReference& Node)
{
	//更新 Cycle 动画

	bool bResult = false;
	FSequencePlayerReference SequencePlayer;
	USequencePlayerLibrary::ConvertToSequencePlayerPure(Node, SequencePlayer, bResult);
	if (bResult)
	{
		//获取Cycle动画
		UAnimSequenceBase* TempAnim = GetCycleAnim();

		//设置Sequence并惯性混合
		USequencePlayerLibrary::SetSequenceWithInertialBlending(Context, SequencePlayer, TempAnim, 0.25f);

		//更新步幅匹配速率
		const FL2MovementFragment_Cycle& CycleData = GetMovementFragment_Cycle();
		const FL2MovementFragment_Sprint& SprintData = GetMovementFragment_Sprint();
		if (CycleData.IsAnimValid() && SprintData.IsAnimValid())
		{
			FVector2D PlayRateClamp_Cycle = GetPlayRateClampCycle();
			UAnimDistanceMatchingLibrary::SetPlayrateToMatchSpeed(SequencePlayer, DisplacementSpeed, PlayRateClamp_Cycle);
			StrideWarpingCycleAlpha = UKismetMathLibrary::FInterpTo(StrideWarpingCycleAlpha,
			                                                        bIsRunningIntoWall ? 0.5f : 1.f,
			                                                        UAnimExecutionContextLibrary::GetDeltaTime(Context),
			                                                        10.f);
		}
	}
}

void UL2MoverAnimInstance::OnBecomeRelevant_SetUpStopAnim(const FAnimUpdateContext& Context, const FAnimNodeReference& Node)
{
	//更新 Stop 动画

	bool bResult = false;
	FSequenceEvaluatorReference SequenceEvaluator;
	USequenceEvaluatorLibrary::ConvertToSequenceEvaluatorPure(Node, SequenceEvaluator, bResult);
	if (bResult)
	{
		UAnimSequenceBase* TempAnim = GetStopAnim();

		USequenceEvaluatorLibrary::SetSequence(SequenceEvaluator, TempAnim);

		if (!ShouldDistanceMatchStop())
		{
			UAnimDistanceMatchingLibrary::DistanceMatchToTarget(SequenceEvaluator, 0.f, L2Mover_AnimCurveName::LocomotionDistance);
		}
	}
}

void UL2MoverAnimInstance::OnUpdate_UpdateStopAnim(const FAnimUpdateContext& Context, const FAnimNodeReference& Node)
{
	bool bResult = false;
	FSequenceEvaluatorReference SequenceEvaluator;
	USequenceEvaluatorLibrary::ConvertToSequenceEvaluatorPure(Node, SequenceEvaluator, bResult);
	if (bResult)
	{
		//1.当玩家角色拥有当前速度且没有加速度时，尝试使用距离匹配停步。
		//2.当AI角色拥有当前速度
		if (ShouldDistanceMatchStop())
		{
			//获取预测停止距离
			float DistanceToMatch = GetPredictedStopDistance();
			if (DistanceToMatch > 0.f)
			{
				//如果预测停止距离大于0，则距离匹配到停止点
				UAnimDistanceMatchingLibrary::DistanceMatchToTarget(SequenceEvaluator, DistanceToMatch, L2Mover_AnimCurveName::LocomotionDistance);
			}
			else
			{
				//角色已停止或动画达到零速度，推进一次时间
				USequenceEvaluatorLibrary::AdvanceTime(Context, SequenceEvaluator, 1.f);
			}
		}
		else
		{
			//角色已停止或动画达到零速度，推进一次时间
			USequenceEvaluatorLibrary::AdvanceTime(Context, SequenceEvaluator, 1.f);
		}
	}
}

void UL2MoverAnimInstance::OnBecomeRelevant_SetUpPivotAnim(const FAnimUpdateContext& Context, const FAnimNodeReference& Node)
{
	//设置折返运动

	bool bResult = false;
	FSequenceEvaluatorReference SequenceEvaluator;
	USequenceEvaluatorLibrary::ConvertToSequenceEvaluatorPure(Node, SequenceEvaluator, bResult);
	if (bResult)
	{
		//折返运动加速度
		PivotStartingAcceleration = LocalAcceleration2D;

		//折返动画
		const FL2MovementFragment_Pivot& PivotData = GetMovementFragment_Pivot();
		if (PivotData.IsAnimValid())
		{
			UAnimSequenceBase* PivotAnim = GetPivotAnim();
			USequenceEvaluatorLibrary::SetSequence(SequenceEvaluator, PivotAnim);
			USequenceEvaluatorLibrary::SetExplicitTime(SequenceEvaluator, 0.f); //设置动画播放起点
			StrideWarpingPivotAlpha = 0.f; //折返运动 步幅匹配 权重
			TimeAtPivotStop = 0.f; //折返停止时间
			LastPivotTime = 0.2f; //上一次折返时间
		}
	}
}

void UL2MoverAnimInstance::OnUpdate_UpdatePivotAnim(const FAnimUpdateContext& Context, const FAnimNodeReference& Node)
{
	bool bResult = false;
	FSequenceEvaluatorReference SequenceEvaluator;
	USequenceEvaluatorLibrary::ConvertToSequenceEvaluatorPure(Node, SequenceEvaluator, bResult);
	if (bResult)
	{
		//起始时间
		float ExplicitTime = USequenceEvaluatorLibrary::GetAccumulatedTime(SequenceEvaluator);

		//允许在刚开始时短暂时间内切换折返运动
		if (LastPivotTime > 0.f)
		{
			//当折返运动和当前动画不同时，切换动画，并重新设置折返加速度
			UAnimSequenceBase* DesiredPivotAnim = GetDesiredPivotSequence(LocalVelocityDirection);
			if (DesiredPivotAnim && DesiredPivotAnim != USequenceEvaluatorLibrary::GetSequence(SequenceEvaluator))
			{
				USequenceEvaluatorLibrary::SetSequenceWithInertialBlending(Context, SequenceEvaluator, DesiredPivotAnim, 0.2f);
				PivotStartingAcceleration = LocalAcceleration2D; //更新折返加速度
			}
		}

		//-------------- 判断速度方向与加速度方向是否反向 --------------//
		if (UKismetMathLibrary::Dot_VectorVector(LocalVelocity2D, LocalAcceleration2D) < 0.f)
		{
			//距离匹配
			UAnimDistanceMatchingLibrary::DistanceMatchToTarget(SequenceEvaluator,
			                                                    UAnimCharacterMovementLibrary::PredictGroundMovementPivotLocation(
				                                                    OwnerCharacterMovement->GetCurrentAcceleration(),
				                                                    OwnerCharacterMovement->GetLastUpdateVelocity(),
				                                                    OwnerCharacterMovement->GroundFriction).Size2D(),
			                                                    L2Mover_AnimCurveName::LocomotionDistance);
			//设置折返停止时间
			TimeAtPivotStop = ExplicitTime;
		}
		else
		{
			const FL2MovementFragment_Pivot& PivotData = GetMovementFragment_Pivot();
			if (PivotData.IsAnimValid())
			{
				//更新步幅匹配折返权重
				StrideWarpingPivotAlpha = UKismetMathLibrary::MapRangeClamped(ExplicitTime - TimeAtPivotStop - StrideWarpingBlendInStartOffset,
				                                                              0.f, StrideWarpingBlendInDurationScaled,
				                                                              0.f, 1.f);

				FVector2D PlayRateClampPivots = PivotData.PlayRateClamp;
				//一旦加速度和速度对齐，角色就会加速远离枢轴点，因此我们只需按动画其余部分所行进的距离提前时间				
				//当我们融入步幅适配时，平滑地提高最低播放速度
				FVector2D NewPlayRateClamp = FVector2D(FMath::Lerp(0.2f, PlayRateClampPivots.X, StrideWarpingPivotAlpha),
				                                       PlayRateClampPivots.Y);
				UAnimDistanceMatchingLibrary::AdvanceTimeByDistanceMatching(Context, SequenceEvaluator,
				                                                            DisplacementSinceLastUpdate,
				                                                            L2Mover_AnimCurveName::LocomotionDistance, NewPlayRateClamp);
			}
		}
	}
}

void UL2MoverAnimInstance::OnBecomeRelevant_SetUpFallLandAnim(const FAnimUpdateContext& Context, const FAnimNodeReference& Node)
{
	bool bResult = false;
	FSequenceEvaluatorReference SequenceEvaluator;
	USequenceEvaluatorLibrary::ConvertToSequenceEvaluatorPure(Node, SequenceEvaluator, bResult);
	if (bResult)
	{
		//设置落地的动画时间
		USequenceEvaluatorLibrary::SetExplicitTime(SequenceEvaluator, 0.f);
	}
}

void UL2MoverAnimInstance::OnUpdate_UpdateFallLandAnim(const FAnimUpdateContext& Context, const FAnimNodeReference& Node)
{
	bool bResult = false;
	FSequenceEvaluatorReference SequenceEvaluator;
	USequenceEvaluatorLibrary::ConvertToSequenceEvaluatorPure(Node, SequenceEvaluator, bResult);
	if (bResult)
	{
		UAnimDistanceMatchingLibrary::DistanceMatchToTarget(SequenceEvaluator, GroundDistance, L2Mover_AnimCurveName::LocomotionDistance);
	}
}

void UL2MoverAnimInstance::OnBecomeRelevant_SetUpTurnInPlaceAnim(const FAnimUpdateContext& Context, const FAnimNodeReference& Node)
{
	bool bResult = false;
	FSequenceEvaluatorReference SequenceEvaluator;
	USequenceEvaluatorLibrary::ConvertToSequenceEvaluatorPure(Node, SequenceEvaluator, bResult);
	if (bResult)
	{
		TurnInPlaceAnimTime = 0.f;
		USequenceEvaluatorLibrary::SetExplicitTime(SequenceEvaluator, 0.f);
		bTurnInPlaceB = !bTurnInPlaceB;
	}
}

void UL2MoverAnimInstance::OnUpdate_UpdateTurnInPlaceAnim(const FAnimUpdateContext& Context, const FAnimNodeReference& Node)
{
	bool bResult = false;
	FSequenceEvaluatorReference SequenceEvaluator;
	USequenceEvaluatorLibrary::ConvertToSequenceEvaluatorPure(Node, SequenceEvaluator, bResult);
	if (bResult)
	{
		//根据旋转方向 更新 TurnInPlace 动画
		USequenceEvaluatorLibrary::SetSequenceWithInertialBlending(Context, SequenceEvaluator,
		                                                           SelectTurnInPlaceAnimation(TurnInPlaceRotationDirection),
		                                                           0.2f);
		//更新原地转身动画时间
		TurnInPlaceAnimTime = UAnimExecutionContextLibrary::GetDeltaTime(Context) + TurnInPlaceAnimTime;

		//更新播放时间
		USequenceEvaluatorLibrary::SetExplicitTime(SequenceEvaluator, TurnInPlaceAnimTime);
	}
}

void UL2MoverAnimInstance::OnBecomeRelevant_SetUpTurnInPlaceRotationState(const FAnimUpdateContext& Context, const FAnimNodeReference& Node)
{
	//更新 原地转动的旋转方向
	TurnInPlaceRotationDirection = UKismetMathLibrary::SignOfFloat(RootYawOffset) * -1.f;
}

void UL2MoverAnimInstance::OnBecomeRelevant_SetUpTurnInPlaceRecoveryState(const FAnimUpdateContext& Context, const FAnimNodeReference& Node)
{
	//原地转向的恢复方向
	TurnInPlaceRecoveryDirection = TurnInPlaceRotationDirection;
}

void UL2MoverAnimInstance::OnUpdate_UpdateTurnInPlaceRecoveryAnim(const FAnimUpdateContext& Context, const FAnimNodeReference& Node)
{
	bool bResult = false;
	FSequencePlayerReference SequencePlayer;
	USequencePlayerLibrary::ConvertToSequencePlayerPure(Node, SequencePlayer, bResult);
	if (bResult)
	{
		//更新原地转身动画
		USequencePlayerLibrary::SetSequenceWithInertialBlending(Context, SequencePlayer,
		                                                        SelectTurnInPlaceAnimation(TurnInPlaceRecoveryDirection),
		                                                        0.2f);
	}
}

UAnimSequence* UL2MoverAnimInstance::SelectTurnInPlaceAnimation(float Direction)
{
	UAnimSequence* TempAnim = nullptr;
	const FL2MovementFragment_TurnInPlace& TurnInPlaceData = GetMovementFragment_TurnInPlace();
	if (TurnInPlaceData.IsAnimValid())
	{
		TempAnim = Direction > 0.f ? TurnInPlaceData.TurnInPlace_R_90 : TurnInPlaceData.TurnInPlace_L_90;
	}

	return TempAnim;
}

bool UL2MoverAnimInstance::CanPlayIdleBreak() const
{
	const FL2MovementFragment_Idle& IdleData = GetMovementFragment_Idle();
	if (!IdleData.IsAnimValid())
		return false;

	const bool A = IdleData.bWantsIdleBreak;
	const bool B = IdleData.Idle_Breaks.Num() > 0;
	const bool C = bIsCrouching || IsAnyMontagePlaying() || bHasVelocity || bIsJumping;

	return A && B && !C;
}

void UL2MoverAnimInstance::ResetIdleBreakTransitionLogic()
{
	TimeUntilNextIdleBreak = IdleBreakDelayTime;
}

void UL2MoverAnimInstance::ProcessIdleBreakTransitionLogic(float DeltaTime)
{
	if (CanPlayIdleBreak())
	{
		TimeUntilNextIdleBreak -= DeltaTime;
	}
	else
	{
		ResetIdleBreakTransitionLogic();
	}
}

void UL2MoverAnimInstance::ChooseIdleBreakDelayTime()
{
	//"使用一些取决于 Pawn 所有者位置的逻辑，以便在客户端之间具有大致一致的行为，而不会让每个角色同时进行闲置休息。"
	IdleBreakDelayTime = 6 + UKismetMathLibrary::FTrunc(FMath::Abs(WorldLocation.X + WorldLocation.Y)) % 10;
}

float UL2MoverAnimInstance::GetPredictedStopDistance() const
{
	//预测地面运动停止位置
	FVector PredictGroundMovementStopLocation =
		UAnimCharacterMovementLibrary::PredictGroundMovementStopLocation(
			OwnerCharacterMovement->GetLastUpdateVelocity(),
			OwnerCharacterMovement->bUseSeparateBrakingFriction,
			OwnerCharacterMovement->BrakingFriction,
			OwnerCharacterMovement->GroundFriction,
			OwnerCharacterMovement->BrakingFrictionFactor,
			OwnerCharacterMovement->BrakingDecelerationWalking);
	return PredictGroundMovementStopLocation.Size2D();
}

bool UL2MoverAnimInstance::ShouldDistanceMatchStop() const
{
	//是否应该使用距离匹配来停止动画
	return bHasVelocity && !bHasAcceleration;
}

UAnimSequenceBase* UL2MoverAnimInstance::GetDesiredPivotSequence(EL2MovementDirection InDirection) const
{
	const FL2MovementFragment_Pivot& PivotData = GetMovementFragment_Pivot();
	return PivotData.GetSequenceByDirection(InDirection);
}

UL2MovementDefinition* UL2MoverAnimInstance::GetCurrentMovementDefinition() const
{
	if (!IsValid(L2MoverComponent))
	{
		return nullptr;
	}

	UL2MovementDefinition* MovementDefinition = L2MoverComponent->GetCurrentMovementDefinition();
	if (!IsValid(MovementDefinition))
	{
		return nullptr;
	}

	return MovementDefinition;
}

FL2MovementFragment_Idle UL2MoverAnimInstance::GetMovementFragment_Idle() const
{
	UL2MovementDefinition* MovementDefinition = GetCurrentMovementDefinition();
	if (!IsValid(MovementDefinition))
	{
		FL2MovementFragment_Idle Idle;
		return Idle;
	}

	const FL2MovementFragment_Movement& MovementSet = MovementDefinition->MovementSet;
	return MovementSet.Idle.Get<FL2MovementFragment_Idle>();
}

FL2MovementFragment_Start UL2MoverAnimInstance::GetMovementFragment_Start() const
{
	UL2MovementDefinition* MovementDefinition = GetCurrentMovementDefinition();
	if (!IsValid(MovementDefinition))
	{
		FL2MovementFragment_Start Start;
		return Start;
	}

	const FL2MovementFragment_Movement& MovementSet = MovementDefinition->MovementSet;
	return MovementSet.Start.Get<FL2MovementFragment_Start>();
}

FL2MovementFragment_Start_4Way UL2MoverAnimInstance::GetMovementFragment_StartForwardFacing() const
{
	UL2MovementDefinition* MovementDefinition = GetCurrentMovementDefinition();
	if (!IsValid(MovementDefinition))
	{
		FL2MovementFragment_Start_4Way StartForwardFacing;
		return StartForwardFacing;
	}

	const FL2MovementFragment_Movement& MovementSet = MovementDefinition->MovementSet;
	return MovementSet.StartForwardFacing.Get<FL2MovementFragment_Start_4Way>();
}

FL2MovementFragment_Cycle UL2MoverAnimInstance::GetMovementFragment_Cycle() const
{
	UL2MovementDefinition* MovementDefinition = GetCurrentMovementDefinition();
	if (!IsValid(MovementDefinition))
	{
		FL2MovementFragment_Cycle Cycle;
		return Cycle;
	}
	const FL2MovementFragment_Movement& MovementSet = MovementDefinition->MovementSet;
	return MovementSet.Cycle.Get<FL2MovementFragment_Cycle>();
}

FL2MovementFragment_Stop UL2MoverAnimInstance::GetMovementFragment_Stop() const
{
	UL2MovementDefinition* MovementDefinition = GetCurrentMovementDefinition();
	if (!IsValid(MovementDefinition))
	{
		FL2MovementFragment_Stop Stop;
		return Stop;
	}
	const FL2MovementFragment_Movement& MovementSet = MovementDefinition->MovementSet;
	return MovementSet.Stop.Get<FL2MovementFragment_Stop>();
}

FL2MovementFragment_Sprint UL2MoverAnimInstance::GetMovementFragment_Sprint() const
{
	UL2MovementDefinition* MovementDefinition = GetCurrentMovementDefinition();
	if (!IsValid(MovementDefinition))
	{
		FL2MovementFragment_Sprint Sprint;
		return Sprint;
	}
	const FL2MovementFragment_Movement& MovementSet = MovementDefinition->MovementSet;
	return MovementSet.Sprint.Get<FL2MovementFragment_Sprint>();
}

FL2MovementFragment_Pivot UL2MoverAnimInstance::GetMovementFragment_Pivot() const
{
	UL2MovementDefinition* MovementDefinition = GetCurrentMovementDefinition();
	if (!IsValid(MovementDefinition))
	{
		FL2MovementFragment_Pivot Pivot;
		return Pivot;
	}
	const FL2MovementFragment_Movement& MovementSet = MovementDefinition->MovementSet;
	return MovementSet.Pivot.Get<FL2MovementFragment_Pivot>();
}

FL2MovementFragment_TurnInPlace UL2MoverAnimInstance::GetMovementFragment_TurnInPlace() const
{
	UL2MovementDefinition* MovementDefinition = GetCurrentMovementDefinition();
	if (!IsValid(MovementDefinition))
	{
		FL2MovementFragment_TurnInPlace TurnInPlace;
		return TurnInPlace;
	}
	const FL2MovementFragment_Movement& MovementSet = MovementDefinition->MovementSet;
	return MovementSet.TurnInPlace.Get<FL2MovementFragment_TurnInPlace>();
}

FL2MovementFragment_Jump UL2MoverAnimInstance::GetMovementFragment_Jump() const
{
	UL2MovementDefinition* MovementDefinition = GetCurrentMovementDefinition();
	if (!IsValid(MovementDefinition))
	{
		FL2MovementFragment_Jump Jump;
		return Jump;
	}
	const FL2MovementFragment_Movement& MovementSet = MovementDefinition->MovementSet;
	return MovementSet.Jump.Get<FL2MovementFragment_Jump>();
}

FL2MovementFragment_AimOffset UL2MoverAnimInstance::GetMovementFragment_AimOffset() const
{
	UL2MovementDefinition* MovementDefinition = GetCurrentMovementDefinition();
	if (!IsValid(MovementDefinition))
	{
		FL2MovementFragment_AimOffset AimOffset;
		return AimOffset;
	}
	const FL2MovementFragment_Movement& MovementSet = MovementDefinition->MovementSet;
	return MovementSet.AimOffset.Get<FL2MovementFragment_AimOffset>();
}

TArray<UAnimSequence*> UL2MoverAnimInstance::GetIdleBreaks() const
{
	const FL2MovementFragment_Idle& IdleData = GetMovementFragment_Idle();
	return IdleData.Idle_Breaks;
}

UAnimSequenceBase* UL2MoverAnimInstance::GetStartAnim() const
{
	UAnimSequence* OutSequence = nullptr;
	const FL2MovementFragment_Sprint& SprintData = GetMovementFragment_Sprint();
	const FL2MovementFragment_Start& StartData = GetMovementFragment_Start();
	const FL2MovementFragment_Start_4Way& StartForwardFacingData = GetMovementFragment_StartForwardFacing();

	if (RotationMode == L2MoverRotationMode::VelocityDirection)
	{
		if (bIsSprinting)
		{
			if (SprintData.IsAnimValid())
			{
				OutSequence = SprintData.Sprint_Start;
			}
		}
		else
		{
			if (StartForwardFacingData.IsAnimValid())
			{
				OutSequence = StartForwardFacingData.GetSequenceByDirection(LocalVelocityDirection,
				                                                            LocalVelocityDirectionAngleWithOffset);
			}
		}
	}
	else if (RotationMode == L2MoverRotationMode::LookingDirection)
	{
		if (StartData.IsAnimValid())
		{
			OutSequence = StartData.GetSequenceByDirection(LocalVelocityDirection);
		}
	}

	return OutSequence;
}

UAnimSequenceBase* UL2MoverAnimInstance::GetCycleAnim() const
{
	UAnimSequence* OutSequence = nullptr;
	const FL2MovementFragment_Sprint& SprintData = GetMovementFragment_Sprint();
	const FL2MovementFragment_Cycle& CycleData = GetMovementFragment_Cycle();

	if (bIsSprinting)
	{
		if (SprintData.IsAnimValid())
		{
			OutSequence = SprintData.Sprint_Cycle;
		}
	}
	else
	{
		if (CycleData.IsAnimValid())
		{
			OutSequence = CycleData.GetSequenceByDirection(LocalVelocityDirectionNoOffset);
		}
	}

	return OutSequence;
}

UAnimSequenceBase* UL2MoverAnimInstance::GetStopAnim() const
{
	UAnimSequence* OutSequence = nullptr;
	const FL2MovementFragment_Sprint& SprintData = GetMovementFragment_Sprint();
	const FL2MovementFragment_Stop& StopData = GetMovementFragment_Stop();
	if (bIsSprinting)
	{
		if (SprintData.IsAnimValid())
		{
			OutSequence = SprintData.Sprint_Stop;
		}
	}
	else
	{
		if (StopData.IsAnimValid())
		{
			OutSequence = StopData.GetSequenceByDirection(LocalVelocityDirection);
		}
	}

	return OutSequence;
}

UAnimSequenceBase* UL2MoverAnimInstance::GetPivotAnim() const
{
	UAnimSequence* OutSequence = nullptr;
	const FL2MovementFragment_Pivot& PivotData = GetMovementFragment_Pivot();
	if (PivotData.IsAnimValid())
	{
		OutSequence = PivotData.GetSequenceByDirection(CardinalDirectionFromAcceleration);
	}
	return OutSequence;
}

FVector2D UL2MoverAnimInstance::GetPlayRateClampStart() const
{
	const FL2MovementFragment_Start& StartData = GetMovementFragment_Start();
	const FL2MovementFragment_Sprint& SprintData = GetMovementFragment_Sprint();
	if (StartData.IsAnimValid() && SprintData.IsAnimValid())
	{
		return bIsSprinting ? SprintData.PlayRateClamp : StartData.PlayRateClamp;
	}

	return FVector2D(0.8f, 5.f);
}

FVector2D UL2MoverAnimInstance::GetPlayRateClampCycle() const
{
	const FL2MovementFragment_Cycle& CycleData = GetMovementFragment_Cycle();
	const FL2MovementFragment_Sprint& SprintData = GetMovementFragment_Sprint();
	if (CycleData.IsAnimValid() && SprintData.IsAnimValid())
	{
		return bIsSprinting ? SprintData.PlayRateClamp : CycleData.PlayRateClamp;
	}

	return FVector2D(0.8f, 3.f);
}

FVector2D UL2MoverAnimInstance::GetPlayRateStopCycle() const
{
	const FL2MovementFragment_Stop& StopData = GetMovementFragment_Stop();
	if (StopData.IsAnimValid())
	{
		return StopData.PlayRateClamp;
	}
	return FVector2D(0.8f, 3.f);
}

FVector2D UL2MoverAnimInstance::GetPlayRateClampPivot() const
{
	const FL2MovementFragment_Pivot& PivotData = GetMovementFragment_Pivot();
	const FL2MovementFragment_Sprint& SprintData = GetMovementFragment_Sprint();
	if (PivotData.IsAnimValid() && SprintData.IsAnimValid())
	{
		return bIsSprinting ? SprintData.PlayRateClamp : PivotData.PlayRateClamp;
	}
	return FVector2D(0.8f, 3.f);
}
