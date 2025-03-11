// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Animation/AnimInstanceProxy.h"
#include "L2MoverAnimInstanceProxy.generated.h"

USTRUCT(BlueprintType)
struct L2MOVER_API FL2MoverAnimaInstanceProxy : public FAnimInstanceProxy
{
	GENERATED_BODY()

public:
	FL2MoverAnimaInstanceProxy() = default;

	explicit FL2MoverAnimaInstanceProxy(UAnimInstance* AnimationInstance);

protected:
	virtual void PostUpdate(UAnimInstance* AnimationInstance) const override;
};
