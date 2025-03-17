// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "NativeGameplayTags.h"

namespace L2MoverLocomotionMode
{
	L2MOVER_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Grounded);
	L2MOVER_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InAir);
}

namespace L2MoverRotationMode
{
	L2MOVER_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(VelocityDirection);
	L2MOVER_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(LookingDirection);
}

namespace L2MoverStance
{
	L2MOVER_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Standing);
	L2MOVER_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Crouching);
}

namespace L2MoverGait
{
	L2MOVER_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Walking);
	L2MOVER_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Running);
	L2MOVER_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Sprinting);
}

namespace L2MoverLocomotionAction
{
}

namespace L2MoverExtendedPose
{
	L2MOVER_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Normal);
	L2MOVER_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Aiming);
	L2MOVER_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Custom);
}

namespace L2MoverMovementSet
{
	L2MOVER_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Unarmed);
}