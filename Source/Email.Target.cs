using UnrealBuildTool;
using System.Collections.Generic;

public class EmailTarget : TargetRules
{
	public EmailTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;

		ExtraModuleNames.AddRange( new string[] { "Email" } );
	}
}
