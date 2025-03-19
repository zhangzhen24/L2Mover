// Fill out your copyright notice in the Description page of Project Settings.


#include "Data/L2MovementDefinition.h"

UL2MovementDefinition::UL2MovementDefinition()
{
}

FL2Mover_MovementSet* UL2MovementDefinition::FindMovementSet(const FGameplayTag& InMovementState)
{
	if (FL2Mover_MovementSet* Result = MovementSets.Find(InMovementState))
	{
		return Result;
	}
	
	return nullptr;
}
