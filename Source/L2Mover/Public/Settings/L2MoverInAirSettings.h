// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "L2MoverInAirSettings.generated.h"

class UBlendSpace1D;
class UBlendSpace;

USTRUCT(BlueprintType)
struct FL2MoverInAirSettings
{
	GENERATED_BODY()

	// 使用空中倾斜1D
	UPROPERTY(EditDefaultsOnly, Category = "L2Mover", meta = (InlineEditConditionToggle))
	bool bUseAirLean1D{true};

	// 空中倾斜1D
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "L2Mover", meta = (EditCondition = "bUseAirLean1D"))
	TObjectPtr<UBlendSpace1D> AirLeans1D;

	// 空中倾斜
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "L2Mover", meta = (EditCondition = "!bUseAirLean1D"))
	TObjectPtr<UBlendSpace> AirLeans;

	// 重力大小
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "L2Mover")
	float GravityScale{4.f};

	// 跳跃速度
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "L2Mover")
	float JumpZVelocity{1650.f};

	// 下降制动减速度
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "L2Mover")
	float BrakingDecelerationFalling{0.0f};

	// 空中控制系数
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "L2Mover")
	float AirControl{0.35f};

	// 空气控制加速倍率
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "L2Mover")
	float AirControlBoostMultiplier{2.0f};

	// 空气控制升压速度阈值
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "L2Mover")
	float AirControlBoostVelocityThreshold{25.0f};

	// 下降横向摩擦
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "L2Mover")
	float FallingLateralFriction{0.0f};
};
