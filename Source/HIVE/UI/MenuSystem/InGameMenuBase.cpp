// Copyright of Honeycomb Studio


#include "InGameMenuBase.h"
#include "Kismet/GameplayStatics.h"
#include "HIVE/System/GameInstance/HiveGameInstance.h"
#include "Components/Button.h"
#include "Engine/Engine.h"


void UInGameMenuBase::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	//if (ReturnButton) { ReturnButton->OnClicked.AddDynamic(this, &UInGameMenuBase::ReturnPressedEvent); }
	if (QuitButton) { QuitButton->OnClicked.AddDynamic(this, &UInGameMenuBase::QuitPressedEvent); }

	bIsFocusable = true;
}

void UInGameMenuBase::QuitPressedEvent()
{
	UHiveGameInstance* GM = GetGameInstance<UHiveGameInstance>();
	check(GM);
	GM->ReturnToLobby(GetOwningPlayer());
}
