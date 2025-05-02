// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "L2MoverAnimGraphSettings.generated.h"

USTRUCT(BlueprintType)
struct FL2MoverAnimGraphSettings
{
	GENERATED_BODY()

	FL2MoverAnimGraphSettings();
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "L2Mover",meta=(AllowEditInlineCustomization))
	TMap<FGameplayTag, TSoftClassPtr<UAnimInstance>> AnimLinkedLayers;
};
