#pragma once

#include "Email.h"
#include "Engine/LevelScriptActor.h"
#include "EmailLevel.generated.h"

UCLASS()
class EMAIL_API AEmailLevel : public ALevelScriptActor
{
	GENERATED_BODY()
	
private:
	TSubclassOf<class UEmailWnd> BP_EmailWnd;

public:
	AEmailLevel();

protected:
	virtual void BeginPlay() override;
};
