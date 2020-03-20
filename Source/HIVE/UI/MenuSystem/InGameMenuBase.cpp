// Copyright of Honeycomb Studio


#include "InGameMenuBase.h"
#include "Kismet/GameplayStatics.h"
#include "HIVE/System/GameInstance/HiveGameInstance.h"
#include "Components/Button.h"
#include "Engine/Engine.h"


void UInGameMenuBase::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	//if (CancelButton) { CancelButton->OnClicked.AddDynamic(this, &UInGameMenuBase::CancelPressedEvent); }
	if (QuitButton) { QuitButton->OnClicked.AddDynamic(this, &UInGameMenuBase::QuitPressedEvent); }

	bIsFocusable = true;
}

void UInGameMenuBase::ReturnPressedEvent()
{
	//GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Cyan, TEXT("Cancel Event"));
}

void UInGameMenuBase::QuitPressedEvent()
{
	UHiveGameInstance* GM = GetGameInstance<UHiveGameInstance>();
	check(GM);
	GM->ReturnToLobby(GetOwningPlayer());
}
