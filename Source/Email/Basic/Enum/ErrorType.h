#pragma once

#include "CoreMinimal.h"
#include "ErrorType.generated.h"

UENUM(BlueprintType)
enum class EErrorType : uint8
{
	ET_ATZERO			UMETA(DisplayName = AtZero),
	ET_SPECIALSYMBOL	UMETA(DisplayName = SpecialSymbol),
	ET_IDAPERIOD		UMETA(DisplayName = IdAperiod),
	ET_DOMAINAPERIOD	UMETA(DisplayName = DomainAperiod),
	ET_IDBLANK			UMETA(DisplayName = IdBlank),
	ET_IDLENGTH			UMETA(DisplayName = IdLength),
	ET_DOMAINLENGTH		UMETA(DisplayName = DomainLength)
};
