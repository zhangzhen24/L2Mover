// Fill out your copyright notice in the Description page of Project Settings.


#include "Utility/L2MoverUtility.h"

#include "Data/L2MoverMovementEnums.h"
#include "Kismet/KismetMathLibrary.h"

EL2MovementDirection UL2MoverUtility::SelectCardinalDirectionFromAngle(float LocalVelocityAngle, float DeadZone,
                                                                       EL2MovementDirection CurrentDirection,
                                                                       bool bUseCurrentDirection)
{
	const float AbsAngle = UKismetMathLibrary::Abs(LocalVelocityAngle);
	float FwdDeadZone = DeadZone;
	float BwdDeadZone = DeadZone;
	if (bUseCurrentDirection)
	{
		switch (CurrentDirection)
		{
		case EL2MovementDirection::Forward:
			FwdDeadZone = 2 * FwdDeadZone;
			break;
		case EL2MovementDirection::Backward:
			BwdDeadZone = 2 * BwdDeadZone;
			break;
		case EL2MovementDirection::Left:
			break;
		case EL2MovementDirection::Right:
			break;
		}
	}

	EL2MovementDirection Result;
	if (AbsAngle <= 45 + FwdDeadZone)
	{
		Result = EL2MovementDirection::Forward;
	}
	else if (AbsAngle >= 135 - BwdDeadZone)
	{
		Result = EL2MovementDirection::Backward;
	}
	else if (LocalVelocityAngle > 0)
	{
		Result = EL2MovementDirection::Right;
	}
	else
	{
		Result = EL2MovementDirection::Left;
	}
	return Result;
}

EL2MovementDirection UL2MoverUtility::GetOppositeCardinalDirection(const EL2MovementDirection CurrentDirection)
{
	EL2MovementDirection Result;
	switch (CurrentDirection)
	{
	case EL2MovementDirection::Forward:
		Result = EL2MovementDirection::Backward;
		break;
	case EL2MovementDirection::Backward:
		Result = EL2MovementDirection::Forward;
		break;
	case EL2MovementDirection::Left:
		Result = EL2MovementDirection::Right;
		break;
	case EL2MovementDirection::Right:
		Result = EL2MovementDirection::Left;
		break;
	default:
		Result = EL2MovementDirection::Backward;
		break;
	}
	return Result;
}

bool UL2MoverUtility::IsSafeValid(const UObject* WorldContextObject)
{
	return IsValid(WorldContextObject);
}
