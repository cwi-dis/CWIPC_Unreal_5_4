

using System.IO;
using UnrealBuildTool;

public class CWIPointCloudSuite : ModuleRules
{
    public CWIPointCloudSuite(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
        string api_location;
        string[] dirs = Directory.GetDirectories("C:/Program Files", "cwipc*");
        if (dirs.Length == 1)
        {
            api_location = dirs[0];
        }
        else
        {
            throw new System.Exception("Cannot find cwipc install directory");
        }

        PublicIncludePaths.AddRange(
            new string[] {
                Path.Combine(api_location, "include"),
                Path.Combine(ModuleDirectory, "Public"),
            });

        PrivateIncludePaths.AddRange(
            new string[] {
                Path.Combine(ModuleDirectory, "Private"),
            });

        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "Niagara" });

        PublicAdditionalLibraries.AddRange(
            new string[] {
                Path.Combine(api_location, "lib", "cwipc_util.lib"),
                Path.Combine(api_location, "lib", "cwipc_codec.lib"),
                Path.Combine(api_location, "lib", "cwipc_kinect.lib"),
                Path.Combine(api_location, "lib", "cwipc_realsense2.lib"),
            });
    }
}