#pragma once

#include "Email.h"
#include "Blueprint/UserWidget.h"
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

private:
	UFUNCTION()
	void OnEmailCommitted(const FText& Text, ETextCommit::Type CommitMethod);
};
