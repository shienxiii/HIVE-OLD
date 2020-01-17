// Copyright of Honeycomb Studio


#include "MainMenu.h"
#include "Components/Button.h"
#include "HIVE/System/Game Instance/HiveGameInstance.h"

bool UMainMenu::Initialize()
{
	Super::Initialize();

	GameInstance = Cast<UHiveGameInstance>(GetGameInstance());
	check(GameInstance);

	HostBtn->OnClicked.AddDynamic(this, &UMainMenu::HostClickEvent);

	return true;
}

void UMainMenu::HostClickEvent()
{
	GameInstance->Host();
}
