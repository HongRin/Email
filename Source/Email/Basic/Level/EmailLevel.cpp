#include "EmailLevel.h"
#include "../EmailWnd/EmailWnd.h"

AEmailLevel::AEmailLevel()
{
	static ConstructorHelpers::FClassFinder<UEmailWnd> BP_EMAIL_WND(
		TEXT("WidgetBlueprint'/Game/BP_EmailWnd.BP_EmailWnd_C'"));
	if (BP_EMAIL_WND.Succeeded()) BP_EmailWnd = BP_EMAIL_WND.Class;
	else UE_LOG(LogTemp, Error, TEXT("AEmailLevel.cpp::%d::LINE:: BP_EMAIL_WND is not loaded !"));
}

void AEmailLevel::BeginPlay()
{
	Super::BeginPlay();

	CreateWidget<UUserWidget>(GetWorld(), BP_EmailWnd)->AddToViewport();
}
