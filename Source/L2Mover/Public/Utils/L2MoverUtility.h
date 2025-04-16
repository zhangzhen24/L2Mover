// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "L2Mover/Public/Types/L2MoverMovementEnums.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "L2MoverUtility.generated.h"

/**
 * 
 */
UCLASS()
class L2MOVER_API UL2MoverUtility : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	// 通过角度选择移动方向
	UFUNCTION(BlueprintPure, BlueprintCallable, meta = (BlueprintThreadSafe), Category = "L2Mover Utility")
	static EL2MovementDirection SelectCardinalDirectionFromAngle(float LocalVelocityAngle, float DeadZone,
	                                                             EL2MovementDirection CurrentDirection,
	                                                             bool bUseCurrentDirection);

	// 获取相反的基本方向
	UFUNCTION(BlueprintPure, BlueprintCallable, meta = (BlueprintThreadSafe), Category = "L2Mover Utility")
	static EL2MovementDirection GetOppositeCardinalDirection(const EL2MovementDirection CurrentDirection);

	// 是否安全有效
	UFUNCTION(BlueprintPure, BlueprintCallable, meta = (BlueprintThreadSafe), Category = "L2Mover Utility")
	static bool IsSafeValid(const UObject* WorldContextObject);
};
