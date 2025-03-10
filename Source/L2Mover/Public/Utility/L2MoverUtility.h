// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Data/L2MoverMovementEnums.h"
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
	/**
	 * 从角度选择基本方向
	 * @param Angle 角度
	 * @param DeadZone 死区值
	 * @param CurrentDirection 当前方向
	 * @param bUseCurrentDirection 是否使用当前方向
	 * @return 
	 */
	UFUNCTION(BlueprintPure, BlueprintCallable, meta = (BlueprintThreadSafe), Category = "L2Mover Utility")
	static EL2MovementDirection SelectCardinalDirectionFromAngle(float Angle, float DeadZone, EL2MovementDirection CurrentDirection, bool bUseCurrentDirection);


	/**
	 * 获取相反的基本方向
	 * @param CurrentDirection 基本方向
	 * @return 
	 */
	UFUNCTION(BlueprintPure, BlueprintCallable, meta = (BlueprintThreadSafe), Category = "L2Mover Utility")
	static EL2MovementDirection GetOppositeCardinalDirection(const EL2MovementDirection CurrentDirection);


	/**
	 * 是否安全有效
	 * @param WorldContextObject 世界上下文对象
	 * @return 
	 */
	UFUNCTION(BlueprintPure, BlueprintCallable, meta = (BlueprintThreadSafe), Category = "L2Mover Utility")
	static bool IsSafeValid(const UObject* WorldContextObject);
};
