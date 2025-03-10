// Copyright Epic Games, Inc. All Rights Reserved.

#include "L2MoverModule.h"

#define LOCTEXT_NAMESPACE "FL2MoverModule"

void FL2MoverModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
}

void FL2MoverModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FL2MoverModule, L2Mover)