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

void UEmailWnd::OnEmailCommitted(const FText& Text, ETextCommit::Type CommitMethod)
{
	// 엔터키를 눌렀다면
	if (CommitMethod == ETextCommit::OnEnter)
	{
		// 입력한 email 을 FString 으로 담을 변수
		FString emailStr = Text.ToString();

		// email을 @ 기준으로 나눠 저장할 배열
		TArray<FString> emailArg;

		// @ 기준으로 문자열을 나눔
		emailStr.ParseIntoArray(emailArg, TEXT("@"));

		// 출력
		Text_Output->SetText(FText::FromString(
			TEXT("LocaleName[") + emailArg[0] + TEXT("]::UserName[") + emailArg[1] + TEXT("]")));
	}
}
