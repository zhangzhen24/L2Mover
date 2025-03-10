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

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FL2Mover_OnUpdateGait, const FGameplayTag&, Gait);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FL2Mover_OnUpdateRotationMode, const FGameplayTag&, RotationMode);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FL2Mover_OnUpdateAccelerationChanged, const bool, HasAcceleration, const double, LastVelocityDirection);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FL2Mover_OnUpdateMovementDefinition, UL2MovementDefinition *, InMovementDefinition);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FL2Mover_OnUpdateRootYawOffsetStatus, const bool, Disable);

/**
 * L2Mover运动系统的核心组件，通过该组件可以控制角色的运动状态
 */
UCLASS(Blueprintable, BlueprintType, meta = (BlueprintSpawnableComponent))
class L2MOVER_API UL2MoverComponent : public UPawnComponent
{
	GENERATED_BODY()

	friend class UL2MoverAnimInstance;

protected:
	UPROPERTY(BlueprintAssignable, Category = "L2Mover")
	FL2Mover_OnUpdateGait OnUpdateGait;

	UPROPERTY(BlueprintAssignable, Category = "L2Mover")
	FL2Mover_OnUpdateRotationMode OnUpdateRotationMode;

	UPROPERTY(BlueprintAssignable, Category = "L2Mover")
	FL2Mover_OnUpdateAccelerationChanged OnUpdateAccelerationChanged;

	UPROPERTY(BlueprintAssignable, Category = "L2Mover")
	FL2Mover_OnUpdateMovementDefinition OnUpdateMovementDefinition;

	UPROPERTY(BlueprintAssignable, Category = "L2Mover")
	FL2Mover_OnUpdateRootYawOffsetStatus OnUpdateRootYawOffsetStatus;
	// End of Delegate

public:
	UL2MoverComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	// -- Begin UActorComponent Interface
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	// -- End UActorComponent Interface

protected:
	UFUNCTION()
	void MovementInit();

public:
	/** 获取当前的运动定义 */
	FORCEINLINE UL2MovementDefinition* GetCurrentMovementDefinition() const;
	FORCEINLINE bool IsShouldMove() const;
	FORCEINLINE bool IsOnGround() const;
	FORCEINLINE bool IsJumping() const;
	FORCEINLINE bool IsFalling() const;
	FORCEINLINE bool IsSprinting() const;
	FORCEINLINE bool IsAiming() const;
	FORCEINLINE FGameplayTag GetGait() const;
	FORCEINLINE FGameplayTag GetRotationMode() const;

	const FCharacterGroundInfo& GetGroundInfo();

	UFUNCTION(BlueprintCallable, Category = "L2Mover")
	void UpdateOrientRotationToMovement() const;

	UFUNCTION(BlueprintCallable, Category = "L2Mover")
	void UpdateGaitSpeedToMovement() const;

	UFUNCTION(BlueprintCallable, Category = "L2Mover")
	void UpdateJumpingSettings() const;

	UFUNCTION(BlueprintCallable, Category = "L2Mover")
	void SetGait(FGameplayTag InGait);

	UFUNCTION(BlueprintCallable, Category = "L2Mover")
	void SetRotationMode(FGameplayTag InRotationMode);

	UFUNCTION(BlueprintCallable, Category = "L2Mover")
	void SetSprinting(bool bInIsSprinting);


	//Tick Update
protected:
	UFUNCTION(BlueprintCallable, Category = "L2Mover")
	void UpdateLocomotionData(float DeltaTime);

	UFUNCTION(BlueprintCallable, Category = "L2Mover")
	void UpdateGait(float DeltaTime);

	UFUNCTION(BlueprintCallable, Category = "L2Mover")
	void UpdateRotationMode(float DeltaTime);

	UFUNCTION()
	void OnLanded(const FHitResult& Hit);

	UFUNCTION()
	void OnGaitChanged(const FGameplayTag& InGait);

public:
	// 默认运动类型
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "L2Mover")
	FGameplayTag DefaultMovementType = L2MoverMovementType::Unarmed;

	// 当前运动类型
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "L2Mover")
	FGameplayTag MovementType = FGameplayTag::EmptyTag;

	// 期望的旋转模式
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "L2Mover")
	FGameplayTag DesiredRotationMode = L2MoverRotationMode::VelocityDirection;

	// 当前旋转模式
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "L2Mover")
	FGameplayTag RotationMode = L2MoverRotationMode::VelocityDirection;

	// 扩展姿势
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "L2Mover")
	FGameplayTag ExtendedPose = L2MoverExtendedPose::Normal;

	// 期望的步态
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "L2Mover")
	FGameplayTag DesiredGait = L2MoverGait::Running;

	// 当前步态
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "L2Mover")
	FGameplayTag Gait = L2MoverGait::Running;

	// 当前运动定义
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "L2Mover")
	TObjectPtr<UL2MovementDefinition> MovementDefinition;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "L2Mover")
	bool bMovementDefinitionChanged = false;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "L2Mover")
	TObjectPtr<ACharacter> OwnerCharacter;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "L2Mover")
	TObjectPtr<UCharacterMovementComponent> CharacterMovement;

	// 速度
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "L2Mover")
	FVector Velocity = FVector(0);

	// 速度大小
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "L2Mover")
	float Speed = 0.f;

	// 是否应该移动
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "L2Mover")
	bool bShouldMove = false;

	// 是否在地面上
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "L2Mover")
	bool bIsOnGround = false;

	// 是否在跳跃
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "L2Mover")
	bool bIsJumping = false;

	// 是否在下落
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "L2Mover")
	bool bIsFalling = false;

	// 是否在冲刺
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "L2Mover")
	bool bIsSprinting = false;

	// 是否蹲下
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "L2Mover")
	bool bIsCrouching = false;

	// 地面信息
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "L2Mover")
	FCharacterGroundInfo GroundInfo;

private:
	// 更新当前使用的运动定义
	void UpdateCurrentMovementDefinition_Internal(UL2MovementDefinition* InMovementDefinition);
};
