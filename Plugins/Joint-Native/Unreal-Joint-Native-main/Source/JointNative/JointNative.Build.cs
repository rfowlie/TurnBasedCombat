//Copyright 2022~2024 DevGrain. All Rights Reserved.

#undef UCONFIG_NO_TRANSLITERATION

using UnrealBuildTool;

public class JointNative : ModuleRules
{
	public JointNative(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

		//bEnforceIWYU = true;


		PublicIncludePaths.AddRange(
			new string[] {
				// ... add public include paths required here ...

			}
			);
				
		
		PrivateIncludePaths.AddRange(
			new string[] {
				// ... add other private include paths required here ...
				"JointNative",
				"JointNative/Public",
				"JointNative/Public/Node",
			}
			);
			
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				"Slate",
				"SlateCore",
				"UMG",
				"ICU",
				"GameplayAbilities",
				"Joint", 
				"LevelSequence",
				"MovieScene"
				// ... add other public dependencies that you statically link with here ...
			}
			);
			
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"CoreUObject",
				"Engine",
				"Slate",
				"SlateCore",
				"UMG",
				"ICU",
				"DeveloperSettings",
				"Joint",
				
				// Gameplay abilities with
				"GameplayTags",
				"GameplayTasks",
				"GameplayAbilities",
			}
			);
		
		
		DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
				// ... add any modules that your module loads dynamically here ...
			}
			);

		if (Target.bCompileICU)
		{
			AddEngineThirdPartyPrivateStaticDependencies(Target, "ICU");
		}

		//PublicDefinitions.Remove("UE_ENABLE_ICU");
		//PublicDefinitions.Add("UE_ENABLE_ICU=" + (Target.bCompileICU ? "1" : "0")); // Enable/disable (=1/=0) ICU usage in the codebase. NOTE: This flag is for use while integrating ICU and will be removed afterward.

		//PublicDefinitions.Remove("UCONFIG_NO_TRANSLITERATION");
		//PublicDefinitions.Add("UCONFIG_NO_TRANSLITERATION=" + (Target.bCompileICU ? "0" : "1")); // Enable/disable (=0 /=1) ICU usage in the codebase. NOTE: This flag is for use while integrating ICU and will be removed afterward.

	}
}
