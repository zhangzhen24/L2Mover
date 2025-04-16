// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "L2MoverMovementStructs.generated.h"

USTRUCT(BlueprintType)
struct FL2Mover_Animation_4Direction
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "L2Mover")
	TObjectPtr<UAnimSequence> Forward{nullptr};
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "L2Mover")
	TObjectPtr<UAnimSequence> Backward{nullptr};
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "L2Mover")
	TObjectPtr<UAnimSequence> Left{nullptr};
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "L2Mover")
	TObjectPtr<UAnimSequence> Right{nullptr};

	bool IsAnimValid() const;
	bool HasRootMotion() const;
};

USTRUCT(BlueprintType)
struct FL2Mover_Animation_StartForwardFacing
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "L2Mover")
	TObjectPtr<UAnimSequence> Forward;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "L2Mover")
	TObjectPtr<UAnimSequence> Forward_L90;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "L2Mover")
	TObjectPtr<UAnimSequence> Forward_R90;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "L2Mover")
	TObjectPtr<UAnimSequence> Forward_L180;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "L2Mover")
	TObjectPtr<UAnimSequence> Forward_R180;

	bool IsAnimValid() const;
	bool HasRootMotion() const;
};

USTRUCT(BlueprintType)
struct FL2Mover_Animation_8Direction
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "L2Mover")
	TObjectPtr<UAnimSequence> Forward;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "L2Mover")
	TObjectPtr<UAnimSequence> ForwardLeft;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "L2Mover")
	TObjectPtr<UAnimSequence> ForwardRight;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "L2Mover")
	TObjectPtr<UAnimSequence> Backward;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "L2Mover")
	TObjectPtr<UAnimSequence> BackwardLeft;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "L2Mover")
	TObjectPtr<UAnimSequence> BackwardRight;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "L2Mover")
	TObjectPtr<UAnimSequence> Left;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "L2Mover")
	TObjectPtr<UAnimSequence> Right;

	bool IsAnimValid() const;
	bool HasRootMotion() const;
};
