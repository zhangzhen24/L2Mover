// Fill out your copyright notice in the Description page of Project Settings.

#include "L2MoverComponent.h"

#include "L2MovementDefinition.h"
#include "LogL2Mover.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Settings/L2MoverInAirSettings.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(L2MoverComponent)

namespace L2Mover
{
	static float GroundTraceDistance = 100000.0f;
	FAutoConsoleVariableRef CVar_GroundTraceDistance(TEXT("L2Mover.GroundTraceDistance"), GroundTraceDistance,
	                                                 TEXT("Distance to trace down when generating ground information."),
	                                                 ECVF_Cheat);
};

UL2MoverComponent::UL2MoverComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	SetIsReplicatedByDefault(true);
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = true;
}

void UL2MoverComponent::BeginPlay()
{
	Super::BeginPlay();

	GetWorld()->GetTimerManager().SetTimerForNextTick(this, &ThisClass::InitializeMovement);
}

void UL2MoverComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                      FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (bInitialized)
	{
		RefreshLocomotionData(DeltaTime);
		RefreshGait(DeltaTime);
		RefreshRotationMode(DeltaTime);
	}
}

void UL2MoverComponent::InitializeMovement()
{
	AActor* OwnerActor = GetOwner();
	if (!IsValid(OwnerActor))
	{
		return;
	}

	OwnerCharacter = Cast<ACharacter>(OwnerActor);
	if (!IsValid(OwnerCharacter))
	{
		return;
	}

	CharacterMovement = OwnerCharacter->GetCharacterMovement();
	if (!IsValid(CharacterMovement))
	{
		return;
	}

	OwnerCharacter->LandedDelegate.AddUniqueDynamic(this, &ThisClass::OnLanded);

	OnGaltChangedDalegate.AddDynamic(this, &ThisClass::OnGaitChanged);

	OnMovementDefinitionChangedDalegate.AddDynamic(this, &ThisClass::OnMovementDefinitionChanged);

	// 设置MovementDefinition
	SetCurrentMovementDefinition(MovementDefinitionTag);

	// 设置步态
	SetGait(DesiredGait);

	// 设置RotationMode
	SetRotationMode(DesiredRotationMode);

	// 设置JumpingSettings
	RefreshJumpingSettings();

	bInitialized = true;
}

void UL2MoverComponent::OnLanded(const FHitResult& Hit)
{
	// 落地时的回调
}

void UL2MoverComponent::OnGaitChanged(const FGameplayTag& InGait)
{
	RefreshGaitSpeedToMovement();
}

void UL2MoverComponent::OnMovementDefinitionChanged(UL2MovementDefinition* InMovementDefinition)
{
	RefreshJumpingSettings();
}

void UL2MoverComponent::RefreshLocomotionData(float DeltaTime)
{
	if (OwnerCharacter && CharacterMovement)
	{
		Velocity = OwnerCharacter->GetVelocity();
		Speed = OwnerCharacter->GetVelocity().Size2D();
		bIsSprinting = Gait == L2MoverGait::Sprinting;

		bShouldMove = Speed > 3 && !UKismetMathLibrary::EqualEqual_VectorVector(
			CharacterMovement->GetCurrentAcceleration(), FVector(0));
		bIsJumping = false;
		bIsFalling = false;
		bIsOnGround = CharacterMovement->IsMovingOnGround();
		bIsJumping = CharacterMovement->IsFalling() && Velocity.Z > 0;
		bIsFalling = CharacterMovement->IsFalling() && Velocity.Z <= 0;
		bIsCrouching = CharacterMovement->IsCrouching();
	}
}

void UL2MoverComponent::RefreshGait(float DeltaTime)
{
	if (DesiredGait != Gait)
	{
		Gait = DesiredGait;
		OnGaltChangedDalegate.Broadcast(Gait);
	}
}

void UL2MoverComponent::RefreshRotationMode(float DeltaTime)
{
	if (DesiredRotationMode != RotationMode)
	{
		RotationMode = DesiredRotationMode;
		RotationMode = IsAiming() ? L2MoverRotationMode::LookingDirection : DesiredRotationMode;
		OnRotationModeChangedDalegate.Broadcast(RotationMode);

		RefreshOrientRotationToMovement();
	}
}

inline UL2MovementDefinition* UL2MoverComponent::GetCurrentMovementDefinition() const
{
	return CurrentMovementDefinition;
}

inline bool UL2MoverComponent::IsShouldMove() const
{
	return bShouldMove;
}

inline bool UL2MoverComponent::IsOnGround() const
{
	return bIsOnGround;
}

inline bool UL2MoverComponent::IsJumping() const
{
	return bIsJumping;
}

inline bool UL2MoverComponent::IsFalling() const
{
	return bIsFalling;
}

inline bool UL2MoverComponent::IsSprinting() const
{
	return bIsSprinting;
}

inline bool UL2MoverComponent::IsAiming() const
{
	return ExtendedPose == L2MoverExtendedPose::Aiming;
}

FGameplayTag UL2MoverComponent::GetMovementDefinitionTag() const
{
	return MovementDefinitionTag;
}

const FCharacterGroundInfo& UL2MoverComponent::GetGroundInfo()
{
	if (!IsValid(OwnerCharacter))
	{
		return GroundInfo;
	}

	if (!IsValid(CharacterMovement))
	{
		return GroundInfo;
	}

	if (!OwnerCharacter || (GFrameCounter == GroundInfo.LastUpdateFrame))
	{
		return GroundInfo;
	}

	if (CharacterMovement->MovementMode == MOVE_Walking)
	{
		GroundInfo.GroundHitResult = CharacterMovement->CurrentFloor.HitResult;
		GroundInfo.GroundDistance = 0.0f;
	}
	else
	{
		const UCapsuleComponent* CapsuleComp = OwnerCharacter->GetCapsuleComponent();
		check(CapsuleComp);

		const float CapsuleHalfHeight = CapsuleComp->GetUnscaledCapsuleHalfHeight();
		const ECollisionChannel CollisionChannel = (CharacterMovement->UpdatedComponent
			                                            ? CharacterMovement->UpdatedComponent->GetCollisionObjectType()
			                                            : ECC_Pawn);
		const FVector TraceStart(CharacterMovement->GetActorLocation());
		const FVector TraceEnd(TraceStart.X, TraceStart.Y,
		                       (TraceStart.Z - L2Mover::GroundTraceDistance - CapsuleHalfHeight));

		FCollisionQueryParams QueryParams(SCENE_QUERY_STAT(L2MoverComponent_GetGroundInfo), false, OwnerCharacter);
		FCollisionResponseParams ResponseParam;
		CharacterMovement->InitCollisionParams(QueryParams, ResponseParam);

		FHitResult HitResult;
		GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, CollisionChannel, QueryParams,
		                                     ResponseParam);

		GroundInfo.GroundHitResult = HitResult;
		GroundInfo.GroundDistance = L2Mover::GroundTraceDistance;

		if (CharacterMovement->MovementMode == MOVE_NavWalking)
		{
			GroundInfo.GroundDistance = 0.0f;
		}
		else if (HitResult.bBlockingHit)
		{
			GroundInfo.GroundDistance = FMath::Max((HitResult.Distance - CapsuleHalfHeight), 0.0f);
		}
	}

	GroundInfo.LastUpdateFrame = GFrameCounter;

	return GroundInfo;
}

void UL2MoverComponent::SetCurrentMovementDefinition(const FGameplayTag InMovementDefinitionTag)
{
	if (MovementDefinitions.Num() <= 0)
	{
		return;
	}

	if (!InMovementDefinitionTag.IsValid())
	{
		return;
	}

	for (const auto& MovementDefinitionPtr : MovementDefinitions)
	{
		if (MovementDefinitionPtr.IsValid())
		{
			const UL2MovementDefinition* MovementDefinition = MovementDefinitionPtr.Get();
			if (MovementDefinition && MovementDefinition->MovementDefinitionTag == InMovementDefinitionTag)
			{
				RefreshMovementDefinition(const_cast<UL2MovementDefinition*>(MovementDefinition));
				return;
			}
		}
		else if (!MovementDefinitionPtr.IsNull())
		{
			// 仅在必要时进行同步加载
			const UL2MovementDefinition* MovementDefinition = MovementDefinitionPtr.LoadSynchronous();
			if (MovementDefinition && MovementDefinition->MovementDefinitionTag == InMovementDefinitionTag)
			{
				RefreshMovementDefinition(const_cast<UL2MovementDefinition*>(MovementDefinition));
				return;
			}
		}
	}
}

inline FGameplayTag UL2MoverComponent::GetGait() const
{
	return Gait;
}

inline FGameplayTag UL2MoverComponent::GetRotationMode() const
{
	return RotationMode;
}

inline void UL2MoverComponent::SetGait(FGameplayTag InGait)
{
	DesiredGait = InGait;
}

inline void UL2MoverComponent::SetRotationMode(FGameplayTag InRotationMode)
{
	DesiredRotationMode = InRotationMode;
}

inline void UL2MoverComponent::SetSprinting(bool bInIsSprinting)
{
	bIsSprinting = bInIsSprinting;
}

void UL2MoverComponent::RefreshOrientRotationToMovement() const
{
	if (CharacterMovement)
	{
		CharacterMovement->bUseControllerDesiredRotation = true;
		CharacterMovement->bOrientRotationToMovement = (RotationMode == L2MoverRotationMode::VelocityDirection);
		OwnerCharacter->bUseControllerRotationYaw = false;
		const FString DebugString = FString::Printf(TEXT("bOrientRotationToMovement: %s. bUseControllerDesiredRotation: %s. bUseControllerRotationYaw: %s"),
		                                            CharacterMovement->bOrientRotationToMovement ? TEXT("true") : TEXT("false"),
		                                            CharacterMovement->bUseControllerDesiredRotation ? TEXT("true") : TEXT("false"),
		                                            OwnerCharacter->bUseControllerRotationYaw ? TEXT("true") : TEXT("false"));
		UKismetSystemLibrary::PrintString(this, DebugString, true, false, FLinearColor::White, 10.f);
	}
}

void UL2MoverComponent::RefreshGaitSpeedToMovement() const
{
	if (CharacterMovement && IsValid(GetCurrentMovementDefinition()))
	{
		const FL2MoverGaitSpeedSettings GaitSpeedSettings
			= GetCurrentMovementDefinition()->ControlSettings.FindGaitSpeedSettings(Gait);
		CharacterMovement->MaxWalkSpeed = GaitSpeedSettings.MaxSpeed;
		CharacterMovement->MaxAcceleration = GaitSpeedSettings.MaxAcceleration;
		CharacterMovement->BrakingDecelerationWalking = GaitSpeedSettings.BreakingDecelerationWalking;
		CharacterMovement->RotationRate = GaitSpeedSettings.RotationRate;
	}
}

void UL2MoverComponent::RefreshJumpingSettings() const
{
	if (CharacterMovement && IsValid(GetCurrentMovementDefinition()))
	{
		const FL2MoverInAirSettings& InAirSettings = GetCurrentMovementDefinition()->InAirSettings;
		CharacterMovement->GravityScale = InAirSettings.GravityScale;
		CharacterMovement->JumpZVelocity = InAirSettings.JumpZVelocity;
		CharacterMovement->BrakingDecelerationFalling = InAirSettings.BrakingDecelerationFalling;
		CharacterMovement->AirControl = InAirSettings.AirControl;
		CharacterMovement->AirControlBoostMultiplier = InAirSettings.AirControlBoostMultiplier;
		CharacterMovement->AirControlBoostVelocityThreshold = InAirSettings.AirControlBoostVelocityThreshold;
		CharacterMovement->FallingLateralFriction = InAirSettings.FallingLateralFriction;
	}
}

void UL2MoverComponent::RefreshMovementDefinition(UL2MovementDefinition* InMovementDefinition)
{
	if (InMovementDefinition != CurrentMovementDefinition)
	{
		CurrentMovementDefinition = InMovementDefinition;
		OnMovementDefinitionChangedDalegate.Broadcast(CurrentMovementDefinition);
	}
}
