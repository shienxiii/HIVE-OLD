// Copyright of Honeycomb Studio


#include "ResultScreen_Base.h"
#include "HIVE/System/GameInstance/HiveGameInstance.h"
#include "Engine/Engine.h"

void UResultScreen_Base::InitializeInputComponent()
{
	Super::InitializeInputComponent();

	InputComponent->BindAction("Jump", EInputEvent::IE_Pressed, this, &UResultScreen_Base::StartButtonEvent);
}

void UResultScreen_Base::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	bIsFocusable = true;


	// Initialize the InputComponent
	InitializeInputComponent();
	UnregisterInputComponent();
}

FReply UResultScreen_Base::NativeOnFocusReceived(const FGeometry& InGeometry, const FFocusEvent& InFocusEvent)
{
	FReply returnVal = Super::NativeOnFocusReceived(InGeometry, InFocusEvent);
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Cyan, TEXT("Focus Receive"));
	RegisterInputComponent();
	if (InputComponent->HasBindings())
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Cyan, TEXT("Has Binding"));
	}

	SetInputActionPriority(0);

	return returnVal;
}

void UResultScreen_Base::NativeOnFocusLost(const FFocusEvent& InFocusEvent)
{
	Super::NativeOnFocusLost(InFocusEvent);
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Cyan, TEXT("Lost Focus"));
	
	RegisterInputComponent();
}

void UResultScreen_Base::StartButtonEvent()
{
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Cyan, TEXT("Result Screen End"));
	UHiveGameInstance* GM = GetGameInstance<UHiveGameInstance>();
	check(GM);
	GM->ReturnToLobby(GetOwningPlayer());
}
