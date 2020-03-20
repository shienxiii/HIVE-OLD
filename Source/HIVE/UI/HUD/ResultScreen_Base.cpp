// Copyright of Honeycomb Studio


#include "ResultScreen_Base.h"
#include "HIVE/System/GameInstance/HiveGameInstance.h"

void UResultScreen_Base::InitializeInputComponent()
{
	Super::InitializeInputComponent();
	InputComponent->BindAction("Confirm", EInputEvent::IE_Pressed, this, &UResultScreen_Base::ReturnToLobby);
}

void UResultScreen_Base::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	InitializeInputComponent();
	bIsFocusable = true;
}

void UResultScreen_Base::ReturnToLobby()
{
	UHiveGameInstance* GM = GetGameInstance<UHiveGameInstance>();
	check(GM);
	GM->ReturnToLobby(GetOwningPlayer());
}
