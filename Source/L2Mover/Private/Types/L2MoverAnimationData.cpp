// Fill out your copyright notice in the Description page of Project Settings.


#include "Types/L2MoverAnimationData.h"
#include "Types/L2MoverMovementEnums.h"


bool FL2Mover_AnimData::IsAnimValid() const
{
	return false;
}

bool FL2Mover_AnimData_Idle::IsAnimValid() const
{
	return IsValid(Idle);
}

bool FL2Mover_AnimData_Start::IsAnimValid() const
{
	return Animations.IsAnimValid();
}

UAnimSequence* FL2Mover_AnimData_Start::GetSequenceByDirection(EL2MovementDirection Direction) const
{
	UAnimSequence* OutSequence = nullptr;
	switch (Direction)
	{
	case EL2MovementDirection::Forward:
		OutSequence = Animations.Forward;
		break;
	case EL2MovementDirection::Backward:
		OutSequence = Animations.Backward;
		break;
	case EL2MovementDirection::Left:
		OutSequence = Animations.Left;
		break;
	case EL2MovementDirection::Right:
		OutSequence = Animations.Right;
		break;
	}
	return OutSequence;
}

bool FL2Mover_AnimData_StartForwardFacing::IsAnimValid() const
{
	return Animations.IsAnimValid();
}

UAnimSequence* FL2Mover_AnimData_StartForwardFacing::GetSequenceByDirection(EL2MovementDirection Direction, float VelocityDirectionAngleWithOffset) const
{
	UAnimSequence* OutSequence = nullptr;
	switch (Direction)
	{
	case EL2MovementDirection::Forward:
		OutSequence = Animations.Forward;
		break;
	case EL2MovementDirection::Backward:
		OutSequence = VelocityDirectionAngleWithOffset > 0.f ? Animations.Forward_R180 : Animations.Forward_L180;
		break;
	case EL2MovementDirection::Left:
		OutSequence = Animations.Forward_L90;
		break;
	case EL2MovementDirection::Right:
		OutSequence = Animations.Forward_R90;
		break;
	}
	return OutSequence;
}

bool FL2Mover_AnimData_Cycle::IsAnimValid() const
{
	return Animations.IsAnimValid();
}

UAnimSequence* FL2Mover_AnimData_Cycle::GetSequenceByDirection(EL2MovementDirection Direction) const
{
	UAnimSequence* OutSequence = nullptr;
	switch (Direction)
	{
	case EL2MovementDirection::Forward:
		OutSequence = Animations.Forward;
		break;
	case EL2MovementDirection::Backward:
		OutSequence = Animations.Backward;
		break;
	case EL2MovementDirection::Left:
		OutSequence = Animations.Left;
		break;
	case EL2MovementDirection::Right:
		OutSequence = Animations.Right;
		break;
	}
	return OutSequence;
}

bool FL2Mover_AnimData_Stop::IsAnimValid() const
{
	return Animations.IsAnimValid();
}

UAnimSequence* FL2Mover_AnimData_Stop::GetSequenceByDirection(EL2MovementDirection Direction) const
{
	UAnimSequence* OutSequence = nullptr;
	switch (Direction)
	{
	case EL2MovementDirection::Forward:
		OutSequence = Animations.Forward;
		break;
	case EL2MovementDirection::Backward:
		OutSequence = Animations.Backward;
		break;
	case EL2MovementDirection::Left:
		OutSequence = Animations.Left;
		break;
	case EL2MovementDirection::Right:
		OutSequence = Animations.Right;
		break;
	}
	return OutSequence;
}

bool FL2Mover_AnimData_Pivot::IsAnimValid() const
{
	return Animations.IsAnimValid();
}

UAnimSequence* FL2Mover_AnimData_Pivot::GetSequenceByDirection(EL2MovementDirection Direction) const
{
	UAnimSequence* OutSequence = nullptr;
	switch (Direction)
	{
	case EL2MovementDirection::Forward:
		OutSequence = Animations.Backward;
		break;
	case EL2MovementDirection::Backward:
		OutSequence = Animations.Forward;
		break;
	case EL2MovementDirection::Left:
		OutSequence = Animations.Right;
		break;
	case EL2MovementDirection::Right:
		OutSequence = Animations.Left;
		break;
	}
	return OutSequence;
}

bool FL2Mover_AnimData_Sprint::IsAnimValid() const
{
	return IsValid(Sprint_Start)
		&& IsValid(Sprint_Cycle)
		&& IsValid(Sprint_Stop);
}

bool FL2Mover_AnimData_Jump::IsAnimValid() const
{
	const bool A = IsValid(Jump_Start)
		&& IsValid(Jump_FallLoop)
		&& IsValid(Jump_FallLand);
	const bool B = IsValid(Jump_StartLoop)
		&& IsValid(Jump_FallLoop)
		&& IsValid(Jump_FallLand);
	return A || B;
}

bool FL2Mover_AnimData_TurnInPlace::IsAnimValid() const
{
	return IsValid(TurnInPlace_L_90)
		&& IsValid(TurnInPlace_R_90);
}

bool FL2Mover_AnimData_AimOffset::IsAnimValid() const
{
	return IsValid(AimOffset);
}
