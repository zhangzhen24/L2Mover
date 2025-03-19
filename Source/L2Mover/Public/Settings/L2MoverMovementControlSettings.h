// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NativeGameplayTags.h"
#include "L2MoverMovementControlSettings.generated.h"


/**
 * 步态速度设置
 */
USTRUCT(BlueprintType)
struct FL2MoverGaitSpeedSettings
{
	GENERATED_BODY()

	FL2MoverGaitSpeedSettings() = default;

	FL2MoverGaitSpeedSettings(float InMaxSpeed, float InMaxAcceleration, float InBreakingDecelerationWalking,
	                          const FRotator& InRotationRate);

	// 最大速度
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Gait Settings")
	float MaxSpeed{175.0f};

	// 最大加速度
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Gait Settings")
	float MaxAcceleration{1024.0f};

	// 最大减速度
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Gait Settings")
	float BreakingDecelerationWalking{2048.f};

	// 旋转速度
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Gait Settings")
	FRotator RotationRate{0.f, 400.f, 0.f};
};


/**
 * 移动控制设置
 */
USTRUCT(BlueprintType)
struct FL2MoverMovementControlSettings
{
	GENERATED_BODY()

	// 行走速度设置
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Base Settings")
	FL2MoverGaitSpeedSettings WalkSpeedSettings{175.f, 512.f, 512.f, FRotator(0, 400, 0)};

	// 奔跑速度设置
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Base Settings")
	FL2MoverGaitSpeedSettings RunSpeedSettings{475.f, 2048.f, 2048.f, FRotator(0, 400, 0)};

	// 冲刺速度设置
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Base Settings")
	FL2MoverGaitSpeedSettings SprintSpeedSettings{700.f, 2048.f, 2048.f, FRotator(0, 400, 0)};

	// RootYawOffset角度限制
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BlueprintThreadSafe))
	FVector2D RootYawOffsetAngleClamp{-120.f, 100.f};

	// 下蹲时RootYawOffset角度限制
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BlueprintThreadSafe))
	FVector2D RootYawOffsetAngleClampCrouched{-90.f, 80.f};

	FL2MoverGaitSpeedSettings FindGaitSpeedSettings(const FGameplayTag& Gait) const;
};
