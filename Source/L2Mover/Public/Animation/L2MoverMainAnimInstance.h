// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "L2MoverMainAnimInstance.generated.h"

class UL2MoverComponent;

/**
 * 主动画实例
 */
UCLASS()
class L2MOVER_API UL2MoverMainAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UL2MoverMainAnimInstance(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	// -- Begin AnimInstance Interface
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUninitializeAnimation() override;
	virtual void NativeBeginPlay() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	virtual void NativeThreadSafeUpdateAnimation(float DeltaSeconds) override;
	// -- End AnimInstance Interface

	// -- Post Update Animation
	virtual void NativePostUpdateAnimation();
	virtual FAnimInstanceProxy* CreateAnimInstanceProxy() override;
	// -- Post Update Animation

	UFUNCTION(BlueprintPure, Category = "L2Mover", meta = (BlueprintThreadSafe))
	UL2MoverComponent* GetL2MoverComponent();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "L2Mover", Transient)
	TObjectPtr<UL2MoverComponent> L2MoverComponent;
};
