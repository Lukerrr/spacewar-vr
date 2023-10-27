// Copyright (C) 2022 GB ltd. All rights reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class SpaceWarVREditorTarget : TargetRules
{
	public SpaceWarVREditorTarget( TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V2;
		ExtraModuleNames.AddRange( new string[] { "SpaceWarVR" } );
	}
}
