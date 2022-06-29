using UnrealBuildTool;

public class BaseUI_K2Nodes : ModuleRules
{
    public BaseUI_K2Nodes(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		OptimizeCode = CodeOptimization.Never;

        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "Core", "UnrealEd",
            }
        );

        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                "CoreUObject",
                "Engine",
                "Slate",
                "SlateCore",
                "BaseUI_Module",
                "KismetCompiler",
                "UnrealEd",
                "BlueprintGraph",
                "GraphEditor",
            }
        );
    }
}