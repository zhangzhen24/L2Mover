// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "L2MoverMovementStructs.h"
#include "Animation/AimOffsetBlendSpace.h"
#include "Animation/AnimSequence.h"
#include "Types/L2MoverMovementEnums.h"
#include "L2MoverAnimationData.generated.h"


USTRUCT(BlueprintType)
struct L2MOVER_API FL2Mover_AnimData
{
	GENERATED_BODY()
	virtual ~FL2Mover_AnimData() = default;
	virtual bool IsAnimValid() const;
};

USTRUCT(BlueprintType)
struct FL2Mover_AnimData_Idle : public FL2Mover_AnimData
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "L2Mover")
	TObjectPtr<UAnimSequence> Idle;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "L2Mover")
	TArray<TObjectPtr<UAnimSequence>> Idle_Breaks;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "L2Mover")
	bool bWantsIdleBreak = true;

	virtual bool IsAnimValid() const override;
};

USTRUCT(BlueprintType)
struct FL2Mover_AnimData_Start_LookingDirection : public FL2Mover_AnimData
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "L2Mover")
	FL2Mover_Animation_StartForwardFacing Animations;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "L2Mover")
	FVector2D PlayRateClamp = FVector2D(0.8f, 3.0f);

	virtual bool IsAnimValid() const override;

	UAnimSequence* GetSequenceByDirection(EL2MovementDirection Direction, float VelocityDirectionAngleWithOffset) const;
};

USTRUCT(BlueprintType)
struct FL2Mover_AnimData_Start_VelocityDirection : public FL2Mover_AnimData
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "L2Mover")
	FL2Mover_Animation_4Direction Animations;

	//步幅匹配播放速度限制
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "L2Mover")
	FVector2D PlayRateClamp = FVector2D(0.8f, 3.0f);

	virtual bool IsAnimValid() const override;

	UAnimSequence* GetSequenceByDirection(EL2MovementDirection Direction) const;
};

USTRUCT(BlueprintType)
struct FL2Mover_AnimData_Cycle : public FL2Mover_AnimData
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "L2Mover")
	FL2Mover_Animation_4Direction Animations;

	//步幅匹配播放速度限制
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "L2Mover")
	FVector2D PlayRateClamp = FVector2D(0.8f, 1.2f);

	virtual bool IsAnimValid() const override;

	UAnimSequence* GetSequenceByDirection(EL2MovementDirection Direction) const;
};

USTRUCT(BlueprintType)
struct FL2Mover_AnimData_Stop : public FL2Mover_AnimData
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "L2Mover")
	FL2Mover_Animation_4Direction Animations;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "L2Mover")
	FVector2D PlayRateClamp = FVector2D(0.8f, 1.2f);

	virtual bool IsAnimValid() const override;

	UAnimSequence* GetSequenceByDirection(EL2MovementDirection Direction) const;
};

USTRUCT(BlueprintType)
struct FL2Mover_AnimData_Pivot : public FL2Mover_AnimData
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "L2Mover")
	FL2Mover_Animation_4Direction Animations;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "L2Mover")
	FVector2D PlayRateClamp = FVector2D(0.8f, 3.0f);

	virtual bool IsAnimValid() const override;

	UAnimSequence* GetSequenceByDirection(EL2MovementDirection Direction) const;
};

USTRUCT(BlueprintType)
struct FL2Mover_AnimData_Sprint : public FL2Mover_AnimData
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "L2Mover")
	TObjectPtr<UAnimSequence> Sprint_Start;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "L2Mover")
	TObjectPtr<UAnimSequence> Sprint_Cycle;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "L2Mover")
	TObjectPtr<UAnimSequence> Sprint_Stop;

	//冲刺起步叠加动画
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "L2Mover")
	TObjectPtr<UAnimSequence> Sprint_Start_Acceleration_Additive;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "L2Mover")
	FVector2D PlayRateClamp = FVector2D(0.8f, 1.2f);

	virtual bool IsAnimValid() const override;
};

USTRUCT(BlueprintType)
struct FL2Mover_AnimData_Jump : public FL2Mover_AnimData
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "L2Mover")
	TObjectPtr<UAnimSequence> Jump_Start;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "L2Mover")
	TObjectPtr<UAnimSequence> Jump_Apex;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "L2Mover")
	TObjectPtr<UAnimSequence> Jump_FallLand;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "L2Mover")
	TObjectPtr<UAnimSequence> Jump_RecoveryAdditive;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "L2Mover")
	TObjectPtr<UAnimSequence> Jump_StartLoop;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "L2Mover")
	TObjectPtr<UAnimSequence> Jump_FallLoop;

	//跳跃距离曲线
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "L2Mover")
	FName JumpDistanceCurveName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "L2Mover")
	float JumpRecoveryAdditiveAlpha = 0.65f;

	virtual bool IsAnimValid() const override;
};

USTRUCT(BlueprintType)
struct FL2Mover_AnimData_TurnInPlace : public FL2Mover_AnimData
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "L2Mover")
	TObjectPtr<UAnimSequence> TurnInPlace_L_90;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "L2Mover")
	TObjectPtr<UAnimSequence> TurnInPlace_R_90;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "L2Mover")
	float PlayRate = 1.0;

	// 触发原地转身的最小角度
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "L2Mover")
	float MinTurnInPlaceAngle = 60.f;

	// 原地转身的延迟时间范围
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "L2Mover")
	FVector2D TurnInPlaceDelayTimeRange = FVector2D(0.1f, 0.3f);

	virtual bool IsAnimValid() const override;
};

USTRUCT(BlueprintType)
struct FL2Mover_AnimData_AimOffset : public FL2Mover_AnimData
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "L2Mover")
	TObjectPtr<UAimOffsetBlendSpace> AimOffset = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "L2Mover")
	FVector2D YawRange = FVector2D(-180.f, 180.f);

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "L2Mover")
	FVector2D PitchRange = FVector2D(-90.f, 90.f);

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "L2Mover")
	float InterpSpeed = 0.0;

	virtual bool IsAnimValid() const override;
};
