// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "L2MoverAnimData.h"
#include "Engine/DataAsset.h"
#include "Settings/L2MoverBaseSettings.h"
#include "Settings/L2MoverGroundedSettings.h"
#include "Settings/L2MoverInAirSettings.h"
#include "L2MovementDefinition.generated.h"


UCLASS(Blueprintable)
class L2MOVER_API UL2MovementDefinition : public UDataAsset
{
	GENERATED_BODY()

public:
	UL2MovementDefinition();

	/** 基础设置 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BlueprintThreadSafe), Category = "Settings")
	FL2MoverBaseSettings BaseSettings;

	/** 地面设置 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BlueprintThreadSafe), Category = "Settings")
	FL2MoverGroundedSettings GroundedSettings;

	/** 空中设置 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BlueprintThreadSafe), Category = "Settings")
	FL2MoverInAirSettings InAirSettings;

	//MovementState
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BlueprintThreadSafe), Category = "Settings")
	TMap<FGameplayTag, FL2Mover_AnimData_Movement> MovementSet;

	FL2Mover_AnimData_Movement* FindMovementSet(const FGameplayTag& InMovementState);
};
