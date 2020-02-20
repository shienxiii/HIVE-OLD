// Copyright of Honeycomb Studio


#include "InGameMenuBase.h"
#include "Components/Button.h"
#include "Engine/Engine.h"


void UInGameMenuBase::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	//if (CancelButton) { CancelButton->OnClicked.AddDynamic(this, &UInGameMenuBase::CancelPressedEvent); }

	if (QuitButton) { QuitButton->OnClicked.AddDynamic(this, &UInGameMenuBase::QuitPressedEvent); }
}

void UInGameMenuBase::ReturnPressedEvent()
{
	//GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Cyan, TEXT("Cancel Event"));
}

void UInGameMenuBase::QuitPressedEvent()
{
	GetOwningPlayer()->ClientTravel(TEXT("/Game/Blueprint/Maps/Lobby.Lobby_C"), ETravelType::TRAVEL_Absolute);
}
