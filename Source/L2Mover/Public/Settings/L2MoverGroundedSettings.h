// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "L2MoverGroundedSettings.generated.h"

class UBlendSpace1D;

USTRUCT(BlueprintType)
struct FL2MoverGroundedSettings
{
	GENERATED_BODY()

	// 冲刺倾斜
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Data", meta = (BlueprintThreadSafe))
	TObjectPtr<UBlendSpace1D> SprintLeans{nullptr};

	// 冲刺倾斜插值速度
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Data", Meta = (ClampMin = 0, BlueprintThreadSafe))
	float SprintLeanInterpolationSpeed{4.0f};

	// 进入Crouch
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Data", meta = (BlueprintThreadSafe))
	TObjectPtr<UAnimSequence> CrouchEntry{nullptr};

	// 退出Crouch
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Data", meta = (BlueprintThreadSafe))
	TObjectPtr<UAnimSequence> CrouchExit{nullptr};
};
