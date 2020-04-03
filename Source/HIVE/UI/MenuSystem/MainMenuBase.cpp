// Copyright of Honeycomb Studio


#include "MainMenuBase.h"
#include "Components/WidgetSwitcher.h"
#include "Components/ComboBoxString.h"
#include "Components/PanelWidget.h"
#include "Components/Button.h"
#include "HIVE/System/GameInstance/HiveGameInstance.h"

void UMainMenuBase::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	bIsFocusable = true;

	GameInstance = Cast<UHiveGameInstance>(GetGameInstance());
	if (!GameInstance) { return; }
	
	if (LobbyBtn) { LobbyBtn->OnClicked.AddDynamic(this, &UMainMenuBase::LobbyClickEvent); }

	if (QuitBtn) { QuitBtn->OnClicked.AddDynamic(this, &UMainMenuBase::QuitClickEvent); }

	if (HostBtn) { HostBtn->OnClicked.AddDynamic(this, &UMainMenuBase::HostClickEvent); }

	if (JoinBtn) { JoinBtn->OnClicked.AddDynamic(this, &UMainMenuBase::JoinClickEvent); }

	if (BackBtn) { BackBtn->OnClicked.AddDynamic(this, &UMainMenuBase::BackClickEvent); }


	Setup();
}

void UMainMenuBase::OnLevelRemovedFromWorld(ULevel* InLevel, UWorld* InWorld)
{
	Super::OnLevelRemovedFromWorld(InLevel, InWorld);

	FInputModeGameOnly gameInputMode;
	APlayerController* controller = GetOwningPlayer();
	controller->SetInputMode(gameInputMode);
	controller->bShowMouseCursor = false;
}

void UMainMenuBase::Setup()
{
	UWorld* world = GetWorld();
	if (!world) { return; }

	APlayerController* controller = world->GetFirstPlayerController();
	if (!controller) { return; }

	AddToViewport();


	FInputModeUIOnly uiInputMode;
	uiInputMode.SetWidgetToFocus(TakeWidget());
	uiInputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);

	controller->SetInputMode(uiInputMode);
	controller->bShowMouseCursor = true;
}

void UMainMenuBase::LobbyClickEvent()
{
	MenuSwitcher->SetActiveWidget(LobbyMenu);
}

void UMainMenuBase::QuitClickEvent()
{
	GameInstance->ExitGame();
}

void UMainMenuBase::HostClickEvent()
{
	GameInstance->Host();
}

void UMainMenuBase::JoinClickEvent()
{
	/*if (!TargetIP->Text.IsEmpty())
	{
		GameInstance->Join(TargetIP->Text.ToString());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("NO IP detected"));
	}*/

	GameInstance->Join(TargetIP->GetSelectedOption());
}

void UMainMenuBase::BackClickEvent()
{
	MenuSwitcher->SetActiveWidget(MainMenu);
}
