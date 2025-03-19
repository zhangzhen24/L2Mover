// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "L2MoverAnimData.h"
#include "Engine/DataAsset.h"
#include "Settings/L2MoverMovementControlSettings.h"
#include "Settings/L2MoverGroundedSettings.h"
#include "Settings/L2MoverInAirSettings.h"
#include "L2MovementDefinition.generated.h"


USTRUCT(BlueprintType)
struct FL2Mover_MovementSet
{
	GENERATED_BODY()

	//Settings
public:
	//Anim Data
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "L2Mover")
	TInstancedStruct<FL2Mover_AnimData_Idle> Idle;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "L2Mover")
	TInstancedStruct<FL2Mover_AnimData_Start_VelocityDirection> Start;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "L2Mover")
	TInstancedStruct<FL2Mover_AnimData_Start_LookingDirection> StartForwardFacing;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "L2Mover")
	TInstancedStruct<FL2Mover_AnimData_Cycle> Cycle;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "L2Mover")
	TInstancedStruct<FL2Mover_AnimData_Stop> Stop;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "L2Mover")
	TInstancedStruct<FL2Mover_AnimData_Sprint> Sprint;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "L2Mover")
	TInstancedStruct<FL2Mover_AnimData_TurnInPlace> TurnInPlace;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "L2Mover")
	TInstancedStruct<FL2Mover_AnimData_Pivot> Pivot;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "L2Mover")
	TInstancedStruct<FL2Mover_AnimData_Jump> Jump;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "L2Mover")
	TInstancedStruct<FL2Mover_AnimData_AimOffset> AimOffset;
};


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
	TMap<FGameplayTag, FL2Mover_MovementSet> MovementSets;

public:
	UL2MovementDefinition();

	FL2Mover_MovementSet* FindMovementSet(const FGameplayTag& InMovementState);
};
