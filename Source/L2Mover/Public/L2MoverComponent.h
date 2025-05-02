// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "L2MoverGameplayTags.h"
#include "Components/PawnComponent.h"
#include "L2MoverComponent.generated.h"

class UL2MovementDefinition;
class ACharacter;
class UCharacterMovementComponent;

USTRUCT(BlueprintType)
struct FCharacterGroundInfo
{
	GENERATED_BODY()

	FCharacterGroundInfo()
		: LastUpdateFrame(0), GroundDistance(0.0f)
	{
	}

	uint64 LastUpdateFrame;

	UPROPERTY(BlueprintReadOnly)
	FHitResult GroundHitResult;

	UPROPERTY(BlueprintReadOnly)
	float GroundDistance;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FL2Mover_OnGaitChanged, const FGameplayTag&, Gait);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FL2Mover_OnRotationModeChanged, const FGameplayTag&, RotationMode);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FL2Mover_OnUAccelerationChanged, const bool, HasAcceleration, const double, LastVelocityDirection);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FL2Mover_OnMovementDefinition, UL2MovementDefinition*, InMovementDefinition);

/**
 * L2Mover运动系统的核心组件，通过该组件可以控制角色的运动状态
 */
UCLASS(Blueprintable, BlueprintType, meta = (BlueprintSpawnableComponent))
class L2MOVER_API UL2MoverComponent : public UPawnComponent
{
	GENERATED_BODY()

	friend class UL2MoverAnimLayer_Locomotion;

protected:
	UPROPERTY(BlueprintAssignable, Category = "L2Mover")
	FL2Mover_OnGaitChanged OnGaitChangedDelegate;

	UPROPERTY(BlueprintAssignable, Category = "L2Mover")
	FL2Mover_OnRotationModeChanged OnRotationModeChangedDelegate;

	UPROPERTY(BlueprintAssignable, Category = "L2Mover")
	FL2Mover_OnUAccelerationChanged OnAccelerationChangedDelegate;

	UPROPERTY(BlueprintAssignable, Category = "L2Mover")
	FL2Mover_OnMovementDefinition OnMovementDefinitionChangedDelegate;

public:
	UL2MoverComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	// -- Begin ActorComponent interface
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
	// -- End ActorComponent interface

public:
	FORCEINLINE UL2MovementDefinition* GetCurrentMovementDefinition() const;

	FORCEINLINE bool IsShouldMove() const;

	FORCEINLINE bool IsOnGround() const;

	FORCEINLINE bool IsJumping() const;

	FORCEINLINE bool IsFalling() const;

	FORCEINLINE bool IsSprinting() const;

	FORCEINLINE bool IsAiming() const;

	FORCEINLINE FGameplayTag GetMovementDefinitionTag() const;

	FORCEINLINE FGameplayTag GetGait() const;

	FORCEINLINE FGameplayTag GetRotationMode() const;

	const FCharacterGroundInfo& GetGroundInfo();

	UFUNCTION(BlueprintCallable, Category = "L2Mover")
	void SetCurrentMovementDefinition(const FGameplayTag InMovementDefinitionTag);

	UFUNCTION(BlueprintCallable, Category = "L2Mover")
	void SetGait(FGameplayTag InGait);

	UFUNCTION(BlueprintCallable, Category = "L2Mover")
	void SetRotationMode(FGameplayTag InRotationMode);

	UFUNCTION(BlueprintCallable, Category = "L2Mover")
	void SetSprinting(bool bInIsSprinting);

	UFUNCTION(BlueprintCallable, Category = "L2Mover")
	void RefreshLocomotionData(float DeltaTime);

	UFUNCTION(BlueprintCallable, Category = "L2Mover")
	void RefreshGait(float DeltaTime);

	UFUNCTION(BlueprintCallable, Category = "L2Mover")
	void RefreshRotationMode(float DeltaTime);

	UFUNCTION(BlueprintCallable, Category = "L2Mover")
	void RefreshOrientRotationToMovement() const;

	UFUNCTION(BlueprintCallable, Category = "L2Mover")
	void RefreshGaitSpeedToMovement() const;

	UFUNCTION(BlueprintCallable, Category = "L2Mover")
	void RefreshJumpingSettings() const;

	UFUNCTION(BlueprintCallable, Category = "L2Mover")
	void RefreshMovementDefinition(UL2MovementDefinition* InMovementDefinition);

	UFUNCTION(BlueprintCallable, Category = "L2Mover")
	void RefreshAnimGraphSettings() const;

protected:
	// 运动定义Tag
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "L2Mover")
	FGameplayTag MovementDefinitionTag{L2MoverMovementDefinitionTag::Unarmed};

	// 运动集
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "L2Mover", meta=(DisplayName="Movement Definitions"))
	TArray<TSoftObjectPtr<const UL2MovementDefinition>> MovementDefinitions;

	// 当前激活的运动定义
	UPROPERTY(BlueprintReadOnly, Category = "L2Mover")
	TObjectPtr<UL2MovementDefinition> CurrentMovementDefinition;

	// 期望的步态
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "L2Mover")
	FGameplayTag DesiredGait{L2MoverGait::Running};

	// 当前步态
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "L2Mover")
	FGameplayTag Gait{FGameplayTag::EmptyTag};

	// 以前的步态
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "L2Mover")
	FGameplayTag PreviousGait{FGameplayTag::EmptyTag};

	// 期望的旋转模式
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "L2Mover")
	FGameplayTag DesiredRotationMode{L2MoverRotationMode::VelocityDirection};

	// 当前旋转模式
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "L2Mover")
	FGameplayTag RotationMode{FGameplayTag::EmptyTag};

	// 扩展姿势
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "L2Mover")
	FGameplayTag ExtendedPose{FGameplayTag::EmptyTag};

	// Character
	UPROPERTY(BlueprintReadOnly, Category = "L2Mover")
	TObjectPtr<ACharacter> OwnerCharacter;

	// CharacterMovement
	UPROPERTY(BlueprintReadOnly, Category = "L2Mover")
	TObjectPtr<UCharacterMovementComponent> CharacterMovement;

	// 速度
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "L2Mover")
	FVector Velocity = FVector(0);

	// 速度大小
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "L2Mover")
	float Speed{0.f};

	// 是否应该移动
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "L2Mover")
	bool bShouldMove{false};

	// 是否在地面上
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "L2Mover")
	bool bIsOnGround{false};

	// 是否在跳跃
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "L2Mover")
	bool bIsJumping{false};

	// 是否在下落
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "L2Mover")
	bool bIsFalling{false};

	// 是否在冲刺
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "L2Mover")
	bool bIsSprinting{false};

	// 是否蹲下
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "L2Mover")
	bool bIsCrouching{false};

	// 地面信息
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "L2Mover")
	FCharacterGroundInfo GroundInfo;

	UFUNCTION()
	void InitializeMovement();

	UFUNCTION()
	void OnLanded(const FHitResult& Hit);

	UFUNCTION()
	void OnGaitChanged(const FGameplayTag& InGait);

	UFUNCTION()
	void OnMovementDefinitionChanged(UL2MovementDefinition* InMovementDefinition);

private:
	uint8 bInitialized : 1 {false};
};
