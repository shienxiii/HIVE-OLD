// Copyright of Honeycomb Studio


#include "InGameMenuBase.h"
#include "Components/Button.h"
#include "Engine/Engine.h"


bool UInGameMenuBase::Initialize()
{
	Super::Initialize();

	if (CancelButton) { CancelButton->OnClicked.AddDynamic(this, &UInGameMenuBase::CancelPressedEvent); }

	if (QuitButton) { QuitButton->OnClicked.AddDynamic(this, &UInGameMenuBase::QuitPressedEvent); }

	return true;
}

void UInGameMenuBase::CancelPressedEvent()
{
	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Cyan, TEXT("Cancel Event"));
}

void UInGameMenuBase::QuitPressedEvent()
{
	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, TEXT("Quit Event"));
}
