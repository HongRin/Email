using UnrealBuildTool;
using System.Collections.Generic;

public class EmailEditorTarget : TargetRules
{
	public EmailEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;

		ExtraModuleNames.AddRange( new string[] { "Email" } );
	}
}
