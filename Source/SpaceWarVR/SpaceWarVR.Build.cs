// Copyright (C) 2022 GB ltd. All rights reserved.

using UnrealBuildTool;

public class SpaceWarVR : ModuleRules
{
	public SpaceWarVR(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PrivateIncludePaths.Add(ModuleDirectory);

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore" });
		PublicDependencyModuleNames.AddRange(new string[] { "RoflanEngine" });

		PrivateDependencyModuleNames.AddRange(new string[] {  });
	}
}
