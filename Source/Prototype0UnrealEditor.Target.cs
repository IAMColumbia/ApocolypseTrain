// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class Prototype0UnrealEditorTarget : TargetRules
{
	public Prototype0UnrealEditorTarget( TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V2;
		ExtraModuleNames.AddRange( new string[] { "Prototype0Unreal" } );
        ExtraModuleNames.AddRange(new string[] { "CustomDoor" });
    }
}
