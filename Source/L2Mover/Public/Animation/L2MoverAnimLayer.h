// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Animation/AnimInstance.h"
#include "L2MoverAnimLayer.generated.h"

class APawn;
class UL2MoverComponent;
class UL2MoverMainAnimInstance;

/**
 * 
 */
UCLASS()
class L2MOVER_API UL2MoverAnimLayer : public UAnimInstance
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "L2Mover", Transient)
	TObjectPtr<APawn> OwnerPawn;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "L2Mover", Transient)
	TObjectPtr<UL2MoverComponent> L2MoverComponent;

private:
	UPROPERTY(VisibleAnywhere, Category = "L2Mover", Transient)
	TWeakObjectPtr<UL2MoverMainAnimInstance> Parent;

public:
	UL2MoverAnimLayer(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

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

	UFUNCTION(BlueprintPure, Category = "L2Mover", meta = (BlueprintThreadSafe, ReturnDisplayName = "Parent"))
	UL2MoverMainAnimInstance* GetParent() const;

	UFUNCTION(BlueprintPure, Category = "L2Mover", meta = (BlueprintThreadSafe))
	APawn* GetPawn();

	UFUNCTION(BlueprintPure, Category = "L2Mover", meta = (BlueprintThreadSafe))
	UL2MoverComponent* GetL2MoverComponent();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "L2Mover")
	void OnLinked();
	virtual void OnLinked_Implementation();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "L2Mover")
	void OnUnlinked();
	virtual void OnUnlinked_Implementation();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "L2Mover")
	void ApplySetting();
	virtual void ApplySetting_Implementation();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "L2Mover")
	void ResetSetting();
	virtual void ResetSetting_Implementation();
};
