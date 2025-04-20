// Fill out your copyright notice in the Description page of Project Settings.


#include "L2MovementDefinition.h"

UL2MovementDefinition::UL2MovementDefinition()
{
}

FL2MoverAnimaDataSettings* UL2MovementDefinition::FindMovementDefinitionByGait(const FGameplayTag& InGait)
{
	if (FL2MoverAnimaDataSettings* Result = MovementSets.Find(InGait))
	{
		return Result;
	}

	return nullptr;
}
