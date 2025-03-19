// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/L2MoverMainAnimInstance.h"

#include "Animation/L2MoverAnimInstanceProxy.h"

UL2MoverMainAnimInstance::UL2MoverMainAnimInstance(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UL2MoverMainAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
}

void UL2MoverMainAnimInstance::NativeUninitializeAnimation()
{
	Super::NativeUninitializeAnimation();
}

void UL2MoverMainAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();
}

void UL2MoverMainAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
}

void UL2MoverMainAnimInstance::NativeThreadSafeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeThreadSafeUpdateAnimation(DeltaSeconds);
}

void UL2MoverMainAnimInstance::NativePostUpdateAnimation()
{
}

FAnimInstanceProxy* UL2MoverMainAnimInstance::CreateAnimInstanceProxy()
{
	return new FL2MoverAnimaInstanceProxy{this};
}

UL2MoverComponent* UL2MoverMainAnimInstance::GetL2MoverComponent()
{
	return L2MoverComponent;
}
