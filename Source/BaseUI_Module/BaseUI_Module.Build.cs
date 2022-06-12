using UnrealBuildTool;
 
public class BaseUI_Module : ModuleRules
{
	public BaseUI_Module(ReadOnlyTargetRules Target) : base(Target)
	{
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
        OptimizeCode = CodeOptimization.Never;
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "GameplayTags", "BaseFunctions", "BaseFunctions" });
		
		PrivateDependencyModuleNames.AddRange(new string[]
		{
			"UMG",
            "SlateCore",
            "Slate",
            "BaseFunctions",
            "DeveloperSettings", 
            "ImageWrapper" // 将Texture变为压缩图片格式, "LevelSnapshots" 
		});
		if (Target.bBuildEditor == true)
		{
			PrivateDependencyModuleNames.Add("UnrealEd");
		}
		PublicIncludePaths.AddRange(new string[] {"BaseUI_Module/Public"});
		PrivateIncludePaths.AddRange(new string[] {"BaseUI_Module/Private"});
	}
}