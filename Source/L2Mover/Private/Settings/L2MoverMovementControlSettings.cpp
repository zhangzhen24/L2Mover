// Fill out your copyright notice in the Description page of Project Settings.


#include "Settings/L2MoverMovementControlSettings.h"

#include "L2MoverGameplayTags.h"


/////////////////////////////////////////////////////
/// FL2MoverGaitSpeedSettings
FL2MoverGaitSpeedSettings::FL2MoverGaitSpeedSettings(const float InMaxSpeed,
                                                     const float InMaxAcceleration,
                                                     const float InBreakingDecelerationWalking,
                                                     const FRotator& InRotationRate)
{
	MaxSpeed = InMaxSpeed;
	MaxAcceleration = InMaxAcceleration;
	BreakingDecelerationWalking = InBreakingDecelerationWalking;
	RotationRate = InRotationRate;
}

/////////////////////////////////////////////////////
/// FL2MoverMovementControlSettings
FL2MoverGaitSpeedSettings FL2MoverMovementControlSettings::FindGaitSpeedSettings(const FGameplayTag& Gait) const
{
	if (Gait == L2MoverGait::Running)
	{
		return RunSpeedSettings;
	}

	if (Gait == L2MoverGait::Sprinting)
	{
		return SprintSpeedSettings;
	}

	if (Gait == L2MoverGait::Walking)
	{
		return WalkSpeedSettings;
	}

	return FL2MoverGaitSpeedSettings();
}
