// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class Prototype0UnrealTarget : TargetRules
{
	public Prototype0UnrealTarget( TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V2;
		ExtraModuleNames.AddRange( new string[] { "Prototype0Unreal" } );
        ExtraModuleNames.AddRange(new string[] { "CustomDoor" });
    }
}
