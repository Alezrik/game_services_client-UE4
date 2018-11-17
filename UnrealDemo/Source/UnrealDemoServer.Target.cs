using UnrealBuildTool;
using System.Collections.Generic;

[SupportedPlatforms(UnrealPlatformClass.Server)]
public class UnrealDemoServerTarget : TargetRules   // Change this line as shown previously
{
       public UnrealDemoServerTarget(TargetInfo Target) : base(Target)  // Change this line as shown previously
       {
        Type = TargetType.Server;
        ExtraModuleNames.Add("UnrealDemo");    // Change this line as shown previously
       }
}