// // Fill out your copyright notice in the Description page of Project Settings.


#include "Data/L2MovementFragment.h"

#include "Data/L2MoverMovementEnums.h"

FL2MovementFragment_Base::~FL2MovementFragment_Base()
{
}

bool FL2MovementFragment_Base::IsAnimValid() const
{
	return false;
}

bool FL2MovementFragment_Idle::IsAnimValid() const
{
	return IsValid(Idle);
}

bool FL2MovementFragment_Start::IsAnimValid() const
{
	return IsValid(Forward) && IsValid(Backward) && IsValid(Left) && IsValid(Right);
}

UAnimSequence* FL2MovementFragment_Start::GetSequenceByDirection(EL2MovementDirection Direction) const
{
	UAnimSequence* OutSequence = nullptr;
	switch (Direction)
	{
	case EL2MovementDirection::Forward:
		OutSequence = Forward;
		break;
	case EL2MovementDirection::Backward:
		OutSequence = Backward;
		break;
	case EL2MovementDirection::Left:
		OutSequence = Left;
		break;
	case EL2MovementDirection::Right:
		OutSequence = Right;
		break;
	}
	return OutSequence;
}

bool FL2MovementFragment_Start_4Way::IsAnimValid() const
{
	return IsValid(Forward) && IsValid(Forward_L90) && IsValid(Forward_R90) && IsValid(Forward_L180) && IsValid(Forward_R180);
}

UAnimSequence* FL2MovementFragment_Start_4Way::GetSequenceByDirection(EL2MovementDirection Direction, float VelocityDirectionAngleWithOffset) const
{
	UAnimSequence* OutSequence = nullptr;
	switch (Direction)
	{
	case EL2MovementDirection::Forward:
		OutSequence = Forward;
		break;
	case EL2MovementDirection::Backward:
		OutSequence = VelocityDirectionAngleWithOffset > 0.f ? Forward_R180 : Forward_L180;
		break;
	case EL2MovementDirection::Left:
		OutSequence = Forward_L90;
		break;
	case EL2MovementDirection::Right:
		OutSequence = Forward_R90;
		break;
	}

	return OutSequence;
}

bool FL2MovementFragment_Cycle::IsAnimValid() const
{
	return IsValid(Forward) && IsValid(Backward) && IsValid(Left) && IsValid(Right);
}

UAnimSequence* FL2MovementFragment_Cycle::GetSequenceByDirection(EL2MovementDirection Direction) const
{
	UAnimSequence* OutSequence = nullptr;
	switch (Direction)
	{
	case EL2MovementDirection::Forward:
		OutSequence = Forward;
		break;
	case EL2MovementDirection::Backward:
		OutSequence = Backward;
		break;
	case EL2MovementDirection::Left:
		OutSequence = Left;
		break;
	case EL2MovementDirection::Right:
		OutSequence = Right;
		break;
	}
	return OutSequence;
}

bool FL2MovementFragment_Stop::IsAnimValid() const
{
	return IsValid(Forward) && IsValid(Backward) && IsValid(Left) && IsValid(Right);
}

UAnimSequence* FL2MovementFragment_Stop::GetSequenceByDirection(EL2MovementDirection Direction) const
{
	UAnimSequence* OutSequence = nullptr;
	switch (Direction)
	{
	case EL2MovementDirection::Forward:
		OutSequence = Forward;
		break;
	case EL2MovementDirection::Backward:
		OutSequence = Backward;
		break;
	case EL2MovementDirection::Left:
		OutSequence = Left;
		break;
	case EL2MovementDirection::Right:
		OutSequence = Right;
		break;
	}
	return OutSequence;
}

UAnimSequence* FL2MovementFragment_Pivot::GetSequenceByDirection(EL2MovementDirection Direction) const
{
	UAnimSequence* OutSequence = nullptr;
	switch (Direction)
	{
	case EL2MovementDirection::Forward:
		OutSequence = Backward;
		break;
	case EL2MovementDirection::Backward:
		OutSequence = Forward;
		break;
	case EL2MovementDirection::Left:
		OutSequence = Right;
		break;
	case EL2MovementDirection::Right:
		OutSequence = Left;
		break;
	}
	return OutSequence;
}

bool FL2MovementFragment_Sprint::IsAnimValid() const
{
	return IsValid(Sprint_Start) && IsValid(Sprint_Cycle) && IsValid(Sprint_Stop);
}

bool FL2MovementFragment_Jump::IsAnimValid() const
{
	const bool A = IsValid(Jump_Start) && IsValid(Jump_FallLoop) && IsValid(Jump_FallLand);
	const bool B = IsValid(Jump_StartLoop) && IsValid(Jump_FallLoop) && IsValid(Jump_FallLand);
	return A || B;
}

bool FL2MovementFragment_TurnInPlace::IsAnimValid() const
{
	return IsValid(TurnInPlace_L_90) && IsValid(TurnInPlace_R_90);
}

bool FL2MovementFragment_AimOffset::IsAnimValid() const
{
	return IsValid(AimOffset);
}
