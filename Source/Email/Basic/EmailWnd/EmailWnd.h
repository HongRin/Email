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

	TArray<FString> LabelList;

	TMap<FString, FString> LabelName;

protected:
	virtual void NativeConstruct() override;

private :
	// @ 가 있는 지 확인합니다.
	bool GetIsAt(FString str);

	// @ 에 따라 아이디와 도메인을 나눕니다.
	TTuple <FString, FString> GetIdAndDomainInAt(FString str);

	// " 의 개수가 짝수인지 확인합니다.
	EQuotationMarkType IsquotationMarkNumEven(FString str);

	// 인용부호 안의 문자열을 가져옵니다.
	FString GetInQuotationMark(FString str);

	// 아이디의 특수 문자를 체크합니다.
	bool CheckIDSymbol(FString str);

	// 도메인의 특수 문자를 체크합니다.
	bool CheckDomainSymbol(FString str);

	// . 이 두개 이하인지 체크합니다.
	bool CheckAperiod(FString str);

	// 앞 뒤 사이에 공백이 있는지 체크합니다.
	bool CheckBlank(FString str);

	// 이메일의 문자 개수를 확인합니다.
	bool CheckLength(FString str);

	// 앞 뒤에 - 이 있는지 확인합니다.
	bool CheckHyphen(FString str);

	// 레이블이 존재하는지 확인합니다.
	bool IsLabel(FString str);

	// 아이디를 검사합니다.
	bool IDCheck(FString checkID);

	// 도메인을 검사합니다.
	bool DomainCheck(FString checkdomain);

	// 레이블이 유효한지 확인합니다.
	bool LabelCheck(FString checkLabel);

	// 레이블 네임을 가져옵니다.
	FString GetLabel(FString label);

private :
	// 오류 텍스트를 띄웁니다.
	void OuputErrorText(EErrorType errorType);

private:
	UFUNCTION()
	void OnEmailCommitted(const FText& Text, ETextCommit::Type CommitMethod);
};
