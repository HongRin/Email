#include "EmailWnd.h"
#include "Components/EditableTextBox.h"
#include "Components/TextBlock.h"

void UEmailWnd::NativeConstruct()
{
	Super::NativeConstruct();

	EditableTextBox_Input->OnTextCommitted.AddDynamic(this, &UEmailWnd::OnEmailCommitted);

	GetWorld()->GetFirstPlayerController()->SetInputMode(FInputModeUIOnly());
	GetWorld()->GetFirstPlayerController()->bShowMouseCursor = true;
}


int32 UEmailWnd::GetAtNum(FString str)
{
	int32 atCount = 0;

	for (int i = 0; i < str.Len(); ++i)
	{
		if (str[i] == TCHAR(TEXT('@')))
			atCount++;
	}

	return atCount;
}

TTuple<FString, FString> UEmailWnd::GetIdAndDomainInAt(FString str)
{
	FString id;

	FString domain;

	// 마지막 @ 기준으로 문자열을 나눔

	FString read = TEXT("@");

	int32 index = str.Find(read, ESearchCase::IgnoreCase ,ESearchDir::FromEnd, str.Len());

	UE_LOG(LogTemp, Warning, TEXT("%d"), index);

	for (int i = 0; i < index; ++i)
	{
		id.AppendChar(str[i]);
	}

	for (int i = (index + 1); i < str.Len(); ++i)
	{
		domain.AppendChar(str[i]);
	}

	return MakeTuple(id, domain);
}

void UEmailWnd::OuputErrorText(EErrorType errorType)
{
	switch (errorType)
	{
	case EErrorType::ET_ATZERO :
		Text_Output->SetText(FText::FromString(TEXT("@ 를 찾을 수 없습니다.")));
		break;
	case EErrorType::ET_SPECIALSYMBOL:
		Text_Output->SetText(FText::FromString(TEXT("인용 부호밖에 특수 문자가 있습니다.")));
		break;
	}
}



void UEmailWnd::OnEmailCommitted(const FText& Text, ETextCommit::Type CommitMethod)
{
	// 엔터키를 눌렀다면
	if (CommitMethod == ETextCommit::OnEnter)
	{
		// 입력한 email 을 FString 으로 담을 변수
		FString emailStr = Text.ToString();

		FString id;

		FString domain;

		if (GetAtNum(emailStr) != 0)
		{
			auto tupleData = GetIdAndDomainInAt(emailStr);
			id = tupleData.Get<0>();
			domain = tupleData.Get<1>();

			Text_Output->SetText(FText::FromString(TEXT("ID[") + id + TEXT("]::Domain[") + domain + TEXT("]")));
		}
		else OuputErrorText(EErrorType::ET_ATZERO);
	}
}