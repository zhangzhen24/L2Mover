// Fill out your copyright notice in the Description page of Project Settings.


#include "Data/L2MovementDefinition.h"

UL2MovementDefinition::UL2MovementDefinition()
{
}

FL2Mover_AnimData_Movement* UL2MovementDefinition::FindMovementSet(const FGameplayTag& InMovementState)
{
	if (FL2Mover_AnimData_Movement* Result = MovementSet.Find(InMovementState))
	{
		return Result;
	}
	
	return nullptr;
}
