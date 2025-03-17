// Fill out your copyright notice in the Description page of Project Settings.


#include "Data/L2MovementDefinition.h"

UL2MovementDefinition::UL2MovementDefinition()
{
}

FL2MovementFragment_Movement* UL2MovementDefinition::FindMovementSet(const FGameplayTag& InMovementSetName)
{
	if (FL2MovementFragment_Movement* Result = MovementSet.Find(InMovementSetName))
	{
		return Result;
	}
	
	return nullptr;
}
