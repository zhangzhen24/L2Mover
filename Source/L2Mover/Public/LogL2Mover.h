// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


L2MOVER_API DECLARE_LOG_CATEGORY_EXTERN(LogL2Mover, Log, All);

#define LOG_L2MOVER(Verbosity, Message) \
UE_LOG(LogL2Mover, Verbosity, TEXT("%s: %s"), *(GetClass()->GetName() + "::" + __FUNCTION__), TEXT(Message));

#define LOG_L2MOVER_ARGS(Verbosity, Format, ...) \
UE_LOG(LogL2Mover, Verbosity, TEXT("%s: %s"), *(GetClass()->GetName() + "::" + __FUNCTION__), *FString::Printf(TEXT(Format), ##__VA_ARGS__));

#define LOG_L2MOVER_STATIC_LOG(Verbosity, Message) \
UE_LOG(LogL2Mover, Verbosity, TEXT("%s: %s"), *(StaticClass()->GetName() + "::" + __FUNCTION__), TEXT(Message));

#define LOG_L2MOVER_STATIC_LOG_ARGS(Verbosity, Format, ...) \
UE_LOG(LogL2Mover, Verbosity, TEXT("%s: %s"), *(StaticClass()->GetName() + "::" + __FUNCTION__), *FString::Printf(TEXT(Format), ##__VA_ARGS__));

#define LOG_L2MOVER_NO_UCLASS(Verbosity, Message) \
UE_LOG(LogL2Mover, Verbosity, TEXT("[%hs]: %s"), __FUNCTION__, TEXT(Message));

#define LOG_L2MOVER_NO_UCLASS_ARGS(Verbosity, Format, ...) \
UE_LOG(LogL2Mover, Verbosity, TEXT("[%hs]: %s"), __FUNCTION__, *FString::Printf(TEXT(Format), ##__VA_ARGS__));