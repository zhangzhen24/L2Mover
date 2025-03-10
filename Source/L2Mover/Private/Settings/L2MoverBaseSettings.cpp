// Fill out your copyright notice in the Description page of Project Settings.


#include "Settings/L2MoverBaseSettings.h"

#include "L2MoverGameplayTags.h"


/////////////////////////////////////////////////////
/// FL2MoverGaitSpeedSettings
FL2MoverGaitSpeedSettings::FL2MoverGaitSpeedSettings()
{
}

FL2MoverGaitSpeedSettings::FL2MoverGaitSpeedSettings(float InMaxSpeed, float InMaxAcceleration, float InBreakingDecelerationWalking, const FRotator& InRotationRate)
{
	MaxSpeed = InMaxSpeed;
	MaxAcceleration = InMaxAcceleration;
	BreakingDecelerationWalking = InBreakingDecelerationWalking;
	RotationRate = InRotationRate;
}

/////////////////////////////////////////////////////
/// FL2MoverBaseSettings
FL2MoverGaitSpeedSettings FL2MoverBaseSettings::GetGaitSpeedSettings(const FGameplayTag& Gait) const
{
	if (Gait == L2MoverGait::Walking)
	{
		return WalkSpeedSettings;
	}

	if (Gait == L2MoverGait::Running)
	{
		return RunSpeedSettings;
	}

	if (Gait == L2MoverGait::Sprinting)
	{
		return SprintSpeedSettings;
	}

	return FL2MoverGaitSpeedSettings();
}
