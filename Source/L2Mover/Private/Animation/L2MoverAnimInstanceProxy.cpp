// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/L2MoverAnimInstanceProxy.h"

#include "Animation/L2MoverAnimLayer_Locomotion.h"
#include "Animation/L2MoverMainAnimInstance.h"

FL2MoverAnimaInstanceProxy::FL2MoverAnimaInstanceProxy(UAnimInstance* AnimationInstance)
	: FAnimInstanceProxy{AnimationInstance}
{
}

void FL2MoverAnimaInstanceProxy::PostUpdate(UAnimInstance* AnimationInstance) const
{
	FAnimInstanceProxy::PostUpdate(AnimationInstance);

	//Epic 不允许覆盖 UAnimInstance：:P ostUpdateAnimation（）函数，因此我们必须采用这种解决方法。

	//MainAnimInstance
	auto* L2MoverMainAnimInstance{Cast<UL2MoverMainAnimInstance>(AnimationInstance)};
	if (IsValid(L2MoverMainAnimInstance))
	{
		L2MoverMainAnimInstance->NativePostUpdateAnimation();
	}

	//AnimLayer
	auto* L2MoverAnimLayer{Cast<UL2MoverAnimLayer>(AnimationInstance)};
	if (IsValid(L2MoverAnimLayer))
	{
		L2MoverAnimLayer->NativePostUpdateAnimation();
	}
}
