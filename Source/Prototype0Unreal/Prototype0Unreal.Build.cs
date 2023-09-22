// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Prototype0Unreal : ModuleRules
{
	public Prototype0Unreal(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore","XAudio2_9", "CustomDoor" });

		PrivateDependencyModuleNames.AddRange(new string[] {  });

        PublicDefinitions.Add("WIN32_LEAN_AND_MEAN");

        //PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "lib", "my_lib.a"));

        // Uncomment if you are using Slate UI
        // PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

        // Uncomment if you are using online features
        // PrivateDependencyModuleNames.Add("OnlineSubsystem");

        // To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
    }
}
