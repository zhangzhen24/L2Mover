// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class L2Mover : ModuleRules
{
	public L2Mover(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicIncludePaths.Add("L2Mover");

		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"Core", 
			}
		);
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"CoreUObject",
				"Engine",

				"ModularGameplay",
				"AnimGraphRuntime",
				"AnimationLocomotionLibraryRuntime",
				"GameplayTags",
			}
		);
	}
}