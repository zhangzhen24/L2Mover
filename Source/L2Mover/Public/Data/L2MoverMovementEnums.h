// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "L2MoverMovementEnums.generated.h"


//运动方向
UENUM(BlueprintType)
enum class EL2MovementDirection : uint8
{
	Forward,
	Backward,
	Left,
	Right,
};


//Root Yaw Offset Mode
UENUM(BlueprintType)
enum class EL2RootYawOffsetMode : uint8
{
	BlendOut,
	Accumulate,
	Hold,
};


//空中旋转模式
UENUM(BlueprintType)
enum class EL2DesiredRotation : uint8
{
	// 根据角色状态自动确定轮换方式
	Automatic UMETA(DisplayName="AUTO"),
	// 只需使用当前旋转
	CurrentRotation UMETA(DisplayName="Current rotation"),
	// 用于旋转到 Actor 的运动输入（如果没有运动输入，则回退到“当前旋转”）
	MovementInput UMETA(DisplayName="Movement input"),
	// 面向目标：如果可行，请尝试旋转面向目标（如果没有有效目标，则退回到“移动输入”）
	FaceTarget UMETA(DisplayName="Face the target"),
	// 尝试旋转以面对当前的控制旋转
	ControlRotation UMETA(DisplayName="Control rotation"),
};
