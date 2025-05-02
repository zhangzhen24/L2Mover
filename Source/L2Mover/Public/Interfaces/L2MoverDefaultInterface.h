// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "L2MoverDefaultInterface.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UL2MoverDefaultInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class L2MOVER_API IL2MoverDefaultInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="L2Mover")
	USkeletalMeshComponent* GetMainMesh();
};
