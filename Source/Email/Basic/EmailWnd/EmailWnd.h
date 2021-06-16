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
	// @ �� �ִ� �� Ȯ���մϴ�.
	bool GetIsAt(FString str);

	// @ �� ���� ���̵�� �������� �����ϴ�.
	TTuple <FString, FString> GetIdAndDomainInAt(FString str);

	// " �� ������ ¦������ Ȯ���մϴ�.
	EQuotationMarkType IsquotationMarkNumEven(FString str);

	// �ο��ȣ ���� ���ڿ��� �����ɴϴ�.
	FString GetInQuotationMark(FString str);

	// ���̵��� Ư�� ���ڸ� üũ�մϴ�.
	bool CheckIDSymbol(FString str);

	// �������� Ư�� ���ڸ� üũ�մϴ�.
	bool CheckDomainSymbol(FString str);

	// . �� �ΰ� �������� üũ�մϴ�.
	bool CheckAperiod(FString str);

	// �� �� ���̿� ������ �ִ��� üũ�մϴ�.
	bool CheckBlank(FString str);

	// �̸����� ���� ������ Ȯ���մϴ�.
	bool CheckLength(FString str);

	// �� �ڿ� - �� �ִ��� Ȯ���մϴ�.
	bool CheckHyphen(FString str);

	// ���̺��� �����ϴ��� Ȯ���մϴ�.
	bool IsLabel(FString str);

	// ���̵� �˻��մϴ�.
	bool IDCheck(FString checkID);

	// �������� �˻��մϴ�.
	bool DomainCheck(FString checkdomain);

	// ���̺��� ��ȿ���� Ȯ���մϴ�.
	bool LabelCheck(FString checkLabel);

	// ���̺� ������ �����ɴϴ�.
	FString GetLabel(FString label);

private :
	// ���� �ؽ�Ʈ�� ���ϴ�.
	void OuputErrorText(EErrorType errorType);

private:
	UFUNCTION()
	void OnEmailCommitted(const FText& Text, ETextCommit::Type CommitMethod);
};
