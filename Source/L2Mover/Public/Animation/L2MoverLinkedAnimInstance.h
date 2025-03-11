// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "L2MoverAnimInstance.h"
#include "L2MoverLinkedAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class L2MOVER_API UL2MoverLinkedAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleAnywhere, Category = "State", Transient)
	TWeakObjectPtr<UL2MoverAnimInstance> Parent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient)
	TObjectPtr<ACharacter> Character;

public:
	UL2MoverLinkedAnimInstance();

	virtual void NativeInitializeAnimation() override;

	virtual void NativeBeginPlay() override;

protected:
	virtual FAnimInstanceProxy* CreateAnimInstanceProxy() override;
};
