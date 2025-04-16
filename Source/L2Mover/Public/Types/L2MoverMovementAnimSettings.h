// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "L2MoverAnimData.h"
#include "StructUtils/InstancedStruct.h"
#include "L2MoverMovementAnimSettings.generated.h"


USTRUCT(BlueprintType)
struct FL2MoverMovementAnimSettings
{
	GENERATED_BODY()

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