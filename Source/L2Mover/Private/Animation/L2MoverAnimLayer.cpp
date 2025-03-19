// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/L2MoverAnimLayer.h"

#include "L2MoverComponent.h"
#include "Animation/L2MoverAnimInstanceProxy.h"
#include "Animation/L2MoverMainAnimInstance.h"
#include "GameFramework/Pawn.h"

UL2MoverAnimLayer::UL2MoverAnimLayer(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UL2MoverAnimLayer::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	Parent = Cast<UL2MoverMainAnimInstance>(GetSkelMeshComponent()->GetAnimInstance());

	OwnerPawn = Cast<APawn>(GetOwningActor());

#if WITH_EDITOR
	const auto* World{GetWorld()};

	if (IsValid(World) && !World->IsGameWorld())
	{
		// Use default objects for editor preview.

		if (!Parent.IsValid())
		{
			Parent = GetMutableDefault<UL2MoverMainAnimInstance>();
		}

		if (!IsValid(OwnerPawn))
		{
			OwnerPawn = GetMutableDefault<APawn>();
		}
	}
#endif
}

void UL2MoverAnimLayer::NativeUninitializeAnimation()
{
	Super::NativeUninitializeAnimation();
}

void UL2MoverAnimLayer::NativeBeginPlay()
{
	Super::NativeBeginPlay();

	ensure(OwnerPawn);

	L2MoverComponent = OwnerPawn->FindComponentByClass<UL2MoverComponent>();
}

void UL2MoverAnimLayer::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
}

void UL2MoverAnimLayer::NativeThreadSafeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeThreadSafeUpdateAnimation(DeltaSeconds);
}

void UL2MoverAnimLayer::NativePostUpdateAnimation()
{
}

FAnimInstanceProxy* UL2MoverAnimLayer::CreateAnimInstanceProxy()
{
	return new FL2MoverAnimaInstanceProxy{this};
}

UL2MoverMainAnimInstance* UL2MoverAnimLayer::GetParent() const
{
	return Parent.Get();
}

APawn* UL2MoverAnimLayer::GetPawn()
{
	return OwnerPawn;
}

UL2MoverComponent* UL2MoverAnimLayer::GetL2MoverComponent()
{
	return L2MoverComponent;
}

void UL2MoverAnimLayer::OnLinked_Implementation()
{
	//TODO: 链接动画层时处理一些事情
}

void UL2MoverAnimLayer::OnUnlinked_Implementation()
{
	//TODO: 从动画层取消链接时处理一些事情
}

void UL2MoverAnimLayer::ApplySetting_Implementation()
{
}

void UL2MoverAnimLayer::ResetSetting_Implementation()
{
}
