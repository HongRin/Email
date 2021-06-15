#pragma once

#include "Email.h"
#include "Blueprint/UserWidget.h"
#include "Templates/Tuple.h"

#include "../Enum/ErrorType.h"
#include "../Enum/QuotationMarkType.h"

#include "EmailWnd.generated.h"

UCLASS()
class EMAIL_API UEmailWnd : public UUserWidget
{
	GENERATED_BODY()
	
private :
	UPROPERTY(meta = (BindWidget))
	class UEditableTextBox* EditableTextBox_Input;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Text_Output;

protected:
	virtual void NativeConstruct() override;

private :
	bool GetIsAt(FString str);

	TTuple <FString, FString> GetIdAndDomainInAt(FString str);

	EQuotationMarkType IsquotationMarkNumEven(FString str);

	FString GetInQuotationMark(FString str);

	bool CheckSymbol(FString str);

	bool IDCheck(FString checkID);
	bool DomainCheck(FString checkdomain);


private :
	void OuputErrorText(EErrorType errorType);

private:
	UFUNCTION()
	void OnEmailCommitted(const FText& Text, ETextCommit::Type CommitMethod);
};
