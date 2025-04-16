// Fill out your copyright notice in the Description page of Project Settings.


#include "L2MovementDefinition.h"

UL2MovementDefinition::UL2MovementDefinition()
{
}

FL2MoverMovementAnimSettings* UL2MovementDefinition::FindMovementSet(const FGameplayTag& InMovementState)
{
	if (FL2MoverMovementAnimSettings* Result = MovementSets.Find(InMovementState))
	{
		return Result;
	}

	return nullptr;
}
