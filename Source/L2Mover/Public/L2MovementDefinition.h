// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Settings/L2MoverGroundedSettings.h"
#include "Settings/L2MoverInAirSettings.h"
#include "Settings/L2MoverMovementAnimSettings.h"
#include "Settings/L2MoverMovementControlSettings.h"
#include "L2MovementDefinition.generated.h"


UCLASS(Blueprintable)
class L2MOVER_API UL2MovementDefinition : public UDataAsset
{
	GENERATED_BODY()

public:
	// 控制设置
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BlueprintThreadSafe), Category = "Settings")
	FL2MoverMovementControlSettings ControlSettings;

	// 地面设置
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BlueprintThreadSafe), Category = "Settings")
	FL2MoverGroundedSettings GroundedSettings;

	// 空中设置
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BlueprintThreadSafe), Category = "Settings")
	FL2MoverInAirSettings InAirSettings;

	// MovementSet
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BlueprintThreadSafe), Category = "Settings")
	TMap<FGameplayTag, FL2MoverMovementAnimSettings> MovementSets;


	UL2MovementDefinition();
	
	FL2MoverMovementAnimSettings* FindMovementSet(const FGameplayTag& InMovementState);
};
