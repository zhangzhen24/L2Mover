// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AimOffsetBlendSpace.h"
#include "Animation/AnimSequence.h"
#include "Data/L2MoverMovementEnums.h"
#include "StructUtils/InstancedStruct.h"
#include "L2MovementFragment.generated.h"


USTRUCT(BlueprintType)
struct FL2MovementFragment_Base
{
	GENERATED_BODY()

	virtual ~FL2MovementFragment_Base();

	virtual bool IsAnimValid() const;
};

USTRUCT(BlueprintType)
struct FL2MovementFragment_Idle : public FL2MovementFragment_Base
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Data")
	TObjectPtr<UAnimSequence> Idle;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Data")
	TArray<TObjectPtr<UAnimSequence>> Idle_Breaks;

	//是否想要空闲休息
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Data")
	bool bWantsIdleBreak = true;

	virtual bool IsAnimValid() const override;
};


USTRUCT(BlueprintType)
struct FL2MovementFragment_Start : public FL2MovementFragment_Base
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Data")
	TObjectPtr<UAnimSequence> Forward;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Data")
	TObjectPtr<UAnimSequence> Backward;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Data")
	TObjectPtr<UAnimSequence> Left;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Data")
	TObjectPtr<UAnimSequence> Right;

	//步幅匹配播放速度限制
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Data")
	FVector2D PlayRateClamp = FVector2D(0.8f, 3.0f);

	virtual bool IsAnimValid() const override;
	UAnimSequence* GetSequenceByDirection(EL2MovementDirection Direction) const;
};

USTRUCT(BlueprintType)
struct FL2MovementFragment_Start_4Way : public FL2MovementFragment_Base
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Data")
	TObjectPtr<UAnimSequence> Forward;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Data")
	TObjectPtr<UAnimSequence> Forward_L90;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Data")
	TObjectPtr<UAnimSequence> Forward_R90;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Data")
	TObjectPtr<UAnimSequence> Forward_L180;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Data")
	TObjectPtr<UAnimSequence> Forward_R180;

	//步幅匹配播放速度限制
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Data")
	FVector2D PlayRateClamp = FVector2D(0.8f, 3.0f);

	virtual bool IsAnimValid() const override;
	UAnimSequence* GetSequenceByDirection(EL2MovementDirection Direction, float VelocityDirectionAngleWithOffset) const;
};


USTRUCT(BlueprintType)
struct FL2MovementFragment_Cycle : public FL2MovementFragment_Base
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Data")
	TObjectPtr<UAnimSequence> Forward;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Data")
	TObjectPtr<UAnimSequence> Backward;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Data")
	TObjectPtr<UAnimSequence> Left;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Data")
	TObjectPtr<UAnimSequence> Right;

	//步幅匹配播放速度限制
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Data")
	FVector2D PlayRateClamp = FVector2D(0.8f, 1.2f);

	virtual bool IsAnimValid() const override;
	UAnimSequence* GetSequenceByDirection(EL2MovementDirection Direction) const;
};

USTRUCT(BlueprintType)
struct FL2MovementFragment_Stop : public FL2MovementFragment_Base
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Data")
	TObjectPtr<UAnimSequence> Forward;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Data")
	TObjectPtr<UAnimSequence> Backward;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Data")
	TObjectPtr<UAnimSequence> Left;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Data")
	TObjectPtr<UAnimSequence> Right;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Data")
	FVector2D PlayRateClamp = FVector2D(0.8f, 1.2f);

	virtual bool IsAnimValid() const override;
	UAnimSequence* GetSequenceByDirection(EL2MovementDirection Direction) const;
};

USTRUCT(BlueprintType)
struct FL2MovementFragment_Pivot
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Data")
	TObjectPtr<UAnimSequence> Forward;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Data")
	TObjectPtr<UAnimSequence> Backward;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Data")
	TObjectPtr<UAnimSequence> Left;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Data")
	TObjectPtr<UAnimSequence> Right;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Data")
	FVector2D PlayRateClamp = FVector2D(0.8f, 3.0f);

	bool IsAnimValid() const { return IsValid(Forward) && IsValid(Backward) && IsValid(Left) && IsValid(Right); }
	UAnimSequence* GetSequenceByDirection(EL2MovementDirection Direction) const;
};

USTRUCT(BlueprintType)
struct FL2MovementFragment_Sprint : public FL2MovementFragment_Base
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Data")
	TObjectPtr<UAnimSequence> Sprint_Start;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Data")
	TObjectPtr<UAnimSequence> Sprint_Cycle;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Data")
	TObjectPtr<UAnimSequence> Sprint_Stop;

	//冲刺起步叠加动画
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Data")
	TObjectPtr<UAnimSequence> Sprint_Start_Acceleration_Additive;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Data")
	FVector2D PlayRateClamp = FVector2D(0.8f, 1.2f);

	virtual bool IsAnimValid() const override;
};


USTRUCT(BlueprintType)
struct FL2MovementFragment_Jump : public FL2MovementFragment_Base
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Data")
	TObjectPtr<UAnimSequence> Jump_Start;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Data")
	TObjectPtr<UAnimSequence> Jump_Apex;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Data")
	TObjectPtr<UAnimSequence> Jump_FallLand;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Data")
	TObjectPtr<UAnimSequence> Jump_RecoveryAdditive;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Data")
	TObjectPtr<UAnimSequence> Jump_StartLoop;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Data")
	TObjectPtr<UAnimSequence> Jump_FallLoop;

	//跳跃距离曲线
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Data")
	FName JumpDistanceCurveName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Data")
	float JumpRecoveryAdditiveAlpha = 0.65f;

	virtual bool IsAnimValid() const override;
};

USTRUCT(BlueprintType)
struct FL2MovementFragment_TurnInPlace : public FL2MovementFragment_Base
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Data")
	TObjectPtr<UAnimSequence> TurnInPlace_L_90;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Data")
	TObjectPtr<UAnimSequence> TurnInPlace_R_90;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Data")
	float PlayRate = 1.0;

	// 触发原地转身的最小角度
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Data")
	float MinTurnInPlaceAngle = 60.f;

	// 原地转身的延迟时间范围
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Data")
	FVector2D TurnInPlaceDelayTimeRange = FVector2D(0.1f, 0.3f);

	virtual bool IsAnimValid() const override;
};

USTRUCT(BlueprintType)
struct FL2MovementFragment_AimOffset : public FL2MovementFragment_Base
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Data")
	TObjectPtr<UAimOffsetBlendSpace> AimOffset = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Data")
	FVector2D YawRange = FVector2D(-180.f, 180.f);

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Data")
	FVector2D PitchRange = FVector2D(-90.f, 90.f);

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Data")
	float InterpSpeed = 0.0;

	virtual bool IsAnimValid() const override;
};

USTRUCT(BlueprintType)
struct FL2MovementFragment_Movement
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Data")
	TInstancedStruct<FL2MovementFragment_Idle> Idle;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Data")
	TInstancedStruct<FL2MovementFragment_Start> Start;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Data")
	TInstancedStruct<FL2MovementFragment_Start_4Way> StartForwardFacing;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Data")
	TInstancedStruct<FL2MovementFragment_Cycle> Cycle;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Data")
	TInstancedStruct<FL2MovementFragment_Stop> Stop;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Data")
	TInstancedStruct<FL2MovementFragment_Sprint> Sprint;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Data")
	TInstancedStruct<FL2MovementFragment_TurnInPlace> TurnInPlace;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Data")
	TInstancedStruct<FL2MovementFragment_Pivot> Pivot;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Data")
	TInstancedStruct<FL2MovementFragment_Jump> Jump;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Data")
	TInstancedStruct<FL2MovementFragment_AimOffset> AimOffset;
};
