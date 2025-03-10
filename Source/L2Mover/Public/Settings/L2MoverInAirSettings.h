// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "L2MoverInAirSettings.generated.h"

class UBlendSpace1D;

USTRUCT(BlueprintType)
struct FL2MoverInAirSettings
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Data")
	TObjectPtr<UBlendSpace1D> Leans;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Data")
	float GravityScale = 4.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Data")
	float JumpZVelocity = 1650.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Data")
	float BrakingDecelerationFalling = 0.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Data")
	float AirControl = 0.35f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Data")
	float AirControlBoostMultiplier = 2.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Data")
	float AirControlBoostVelocityThreshold = 25.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Data")
	float FallingLateralFriction = 0.0f;
};
