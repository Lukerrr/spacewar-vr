// Copyright (C) 2022 GB ltd. All rights reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class SpaceWarVRTarget : TargetRules
{
	public SpaceWarVRTarget( TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V2;
		ExtraModuleNames.AddRange( new string[] { "SpaceWarVR" } );
	}
}
