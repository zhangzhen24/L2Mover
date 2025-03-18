// Fill out your copyright notice in the Description page of Project Settings.


#include "Data/L2MoverMovementStructs.h"

bool FL2Mover_Animation_4Direction::IsAnimValid() const
{
	return IsValid(Forward)
		&& IsValid(Backward)
		&& IsValid(Left)
		&& IsValid(Right);
}

bool FL2Mover_Animation_4Direction::HasRootMotion() const
{
	if (IsAnimValid())
	{
		return Forward->HasRootMotion()
			&& Backward->HasRootMotion()
			&& Left->HasRootMotion()
			&& Right->HasRootMotion();
	}
	return false;
}

bool FL2Mover_Animation_StartForwardFacing::IsAnimValid() const
{
	return IsValid(Forward)
		&& IsValid(Forward_L90)
		&& IsValid(Forward_R90)
		&& IsValid(Forward_L180)
		&& IsValid(Forward_R180);
}

bool FL2Mover_Animation_StartForwardFacing::HasRootMotion() const
{
	if (IsAnimValid())
	{
		return Forward->HasRootMotion()
			&& Forward_L90->HasRootMotion()
			&& Forward_R90->HasRootMotion()
			&& Forward_L180->HasRootMotion()
			&& Forward_R180->HasRootMotion();
	}
	return false;
}


bool FL2Mover_Animation_8Direction::IsAnimValid() const
{
	return IsValid(Forward)
		&& IsValid(ForwardLeft)
		&& IsValid(ForwardRight)
		&& IsValid(Backward)
		&& IsValid(BackwardLeft)
		&& IsValid(BackwardRight)
		&& IsValid(Left)
		&& IsValid(Right);
}

bool FL2Mover_Animation_8Direction::HasRootMotion() const
{
	if (IsAnimValid())
	{
		return Forward->HasRootMotion()
			&& ForwardLeft->HasRootMotion()
			&& ForwardRight->HasRootMotion()
			&& Backward->HasRootMotion()
			&& BackwardLeft->HasRootMotion()
			&& BackwardRight->HasRootMotion()
			&& Left->HasRootMotion()
			&& Right->HasRootMotion();
	}
	return false;
}
