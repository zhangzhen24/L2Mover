// Fill out your copyright notice in the Description page of Project Settings.


#include "L2MoverGameplayTags.h"


namespace L2MoverLocomotionMode
{
	UE_DEFINE_GAMEPLAY_TAG(Grounded, FName{TEXTVIEW("L2Mover.LocomotionMode.Grounded")})
	UE_DEFINE_GAMEPLAY_TAG(InAir, FName{TEXTVIEW("L2Mover.LocomotionMode.InAir")})
}

namespace L2MoverRotationMode
{
	UE_DEFINE_GAMEPLAY_TAG(VelocityDirection, FName{TEXTVIEW("L2Mover.RotationMode.VelocityDirection")})
	UE_DEFINE_GAMEPLAY_TAG(LookingDirection, FName{TEXTVIEW("L2Mover.RotationMode.LookingDirection")})
}

namespace L2MoverStance
{
	UE_DEFINE_GAMEPLAY_TAG(Standing, FName{TEXTVIEW("L2Mover.Stance.Standing")})
	UE_DEFINE_GAMEPLAY_TAG(Crouching, FName{TEXTVIEW("L2Mover.Stance.Crouching")})
}

namespace L2MoverGait
{
	UE_DEFINE_GAMEPLAY_TAG(Walking, FName{TEXTVIEW("L2Mover.Gait.Walking")})
	UE_DEFINE_GAMEPLAY_TAG(Running, FName{TEXTVIEW("L2Mover.Gait.Running")})
	UE_DEFINE_GAMEPLAY_TAG(Sprinting, FName{TEXTVIEW("L2Mover.Gait.Sprinting")})
}

namespace L2MoverExtendedPose
{
	UE_DEFINE_GAMEPLAY_TAG(Normal, FName{TEXTVIEW("L2Mover.ExtendedPose.Normal")})
	UE_DEFINE_GAMEPLAY_TAG(Blocked, FName{TEXTVIEW("L2Mover.ExtendedPose.Blocked")})
	UE_DEFINE_GAMEPLAY_TAG(Aiming, FName{TEXTVIEW("L2Mover.ExtendedPose.Aiming")})
	UE_DEFINE_GAMEPLAY_TAG(Custom, FName{TEXTVIEW("L2Mover.ExtendedPose.Custom")})
}

namespace L2MoverMovementSet
{
	UE_DEFINE_GAMEPLAY_TAG(Unarmed, FName{TEXTVIEW("L2Mover.MovementSet.Unarmed")})
}