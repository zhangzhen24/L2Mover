// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/L2MoverLinkedAnimInstance.h"

#include "Animation/L2MoverAnimInstanceProxy.h"
#include "GameFramework/Character.h"

UL2MoverLinkedAnimInstance::UL2MoverLinkedAnimInstance()
{
}

void UL2MoverLinkedAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	Parent = Cast<UL2MoverAnimInstance>(GetSkelMeshComponent()->GetAnimInstance());
	Character = Cast<ACharacter>(GetOwningActor());

#if WITH_EDITOR
	const auto* World{GetWorld()};

	if (IsValid(World) && !World->IsGameWorld())
	{
		// Use default objects for editor preview.

		if (!Parent.IsValid())
		{
			Parent = GetMutableDefault<UL2MoverAnimInstance>();
		}

		if (!IsValid(Character))
		{
			Character = GetMutableDefault<ACharacter>();
		}
	}
#endif
}

void UL2MoverLinkedAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();
}

UL2MoverAnimInstance* UL2MoverLinkedAnimInstance::GetParent()
{
	return Parent.Get();
}

ACharacter* UL2MoverLinkedAnimInstance::GetCharacter()
{
	return Character;
}

FAnimInstanceProxy* UL2MoverLinkedAnimInstance::CreateAnimInstanceProxy()
{
	return new FL2MoverAnimaInstanceProxy{this};
}
