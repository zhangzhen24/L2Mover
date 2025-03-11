// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/L2MoverAnimInstanceProxy.h"

#include "Animation/L2MoverAnimInstance.h"

FL2MoverAnimaInstanceProxy::FL2MoverAnimaInstanceProxy(UAnimInstance* AnimationInstance)
	: FAnimInstanceProxy{AnimationInstance}
{
}

void FL2MoverAnimaInstanceProxy::PostUpdate(UAnimInstance* AnimationInstance) const
{
	FAnimInstanceProxy::PostUpdate(AnimationInstance);

	// Epic does not allow to override the UAnimInstance::PostUpdateAnimation()
	// function in child classes, so we have to resort to this workaround.

	auto* L2MoverAnimInstance{Cast<UL2MoverAnimInstance>(AnimationInstance)};
	if (IsValid(L2MoverAnimInstance))
	{
		L2MoverAnimInstance->NativePostUpdateAnimation();
	}
}
