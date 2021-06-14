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

}
