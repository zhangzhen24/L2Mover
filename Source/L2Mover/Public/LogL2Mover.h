// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


L2MOVER_API DECLARE_LOG_CATEGORY_EXTERN(LogL2Mover, Log, All);

#define L2MOVER_LOG(Verbosity, Format, ...) UE_LOG(LogL2Mover, Verbosity, Format, ##__VA_ARGS__)
#define L2MOVER_CLOG(Condition, Verbosity, Format, ...) UE_CLOG((Condition), LogL2Mover, Verbosity, Format, ##__VA_ARGS__)
