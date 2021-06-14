#pragma once

#include "CoreMinimal.h"
#include "ErrorType.generated.h"

UENUM(BlueprintType)
enum class EErrorType : uint8
{
	ET_ATZERO			UMETA(DisplayName = AtZero),
	ET_SPECIALSYMBOL	UMETA(DisplayName = SpecialSymbol)
};
