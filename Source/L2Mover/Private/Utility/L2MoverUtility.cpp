// Fill out your copyright notice in the Description page of Project Settings.


#include "Utility/L2MoverUtility.h"

#include "Data/L2MoverMovementEnums.h"
#include "Kismet/KismetMathLibrary.h"

EL2MovementDirection UL2MoverUtility::SelectCardinalDirectionFromAngle(float Angle, float DeadZone, EL2MovementDirection CurrentDirection, bool bUseCurrentDirection)
{
	const float AbsAngle = UKismetMathLibrary::Abs(Angle);
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

	if (AbsAngle <= 45 + FwdDeadZone)
	{
		return EL2MovementDirection::Forward;
	}
	else if (AbsAngle >= 135 - BwdDeadZone)
	{
		return EL2MovementDirection::Backward;
	}
	else if (Angle > 0)
	{
		return EL2MovementDirection::Right;
	}
	else
	{
		return EL2MovementDirection::Left;
	}
}

EL2MovementDirection UL2MoverUtility::GetOppositeCardinalDirection(const EL2MovementDirection CurrentDirection)
{
	if (CurrentDirection == EL2MovementDirection::Forward)
	{
		return EL2MovementDirection::Backward;
	}
	else if (CurrentDirection == EL2MovementDirection::Backward)
	{
		return EL2MovementDirection::Forward;
	}
	else if (CurrentDirection == EL2MovementDirection::Left)
	{
		return EL2MovementDirection::Right;
	}
	else
	{
		return EL2MovementDirection::Left;
	}
}

bool UL2MoverUtility::IsSafeValid(const UObject* WorldContextObject)
{
	return IsValid(WorldContextObject);
}