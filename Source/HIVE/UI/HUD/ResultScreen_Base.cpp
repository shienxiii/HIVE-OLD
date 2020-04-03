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
	InitializeInputComponent();
	UnregisterInputComponent();
}

void UResultScreen_Base::StartButtonEvent()
{
	GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Cyan, TEXT("GameEnd"));
}
