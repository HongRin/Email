#pragma once

#include "CoreMinimal.h"
#include "QuotationMarkType.generated.h"

UENUM(BlueprintType)
enum class EQuotationMarkType : uint8
{
	QT_EVEN       UMETA(DisplayName = Even),
	QT_ODD		  UMETA(DisplayName = Odd),
	QT_ZERO		  UMETA(DisplayName = Zero),
	QT_ONE		  UMETA(DisplayName = One)
};