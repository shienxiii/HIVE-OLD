// Copyright of Honeycomb Studio


#include "MainMenu.h"
#include "Components/Button.h"
#include "HIVE/System/Game Instance/HiveGameInstance.h"

bool UMainMenu::Initialize()
{
	Super::Initialize();

	bIsFocusable = true;

	GameInstance = Cast<UHiveGameInstance>(GetGameInstance());
	if (!GameInstance) { return false; }

	HostBtn->OnClicked.AddDynamic(this, &UMainMenu::HostClickEvent);
	JoinBtn->OnClicked.AddDynamic(this, &UMainMenu::JoinClickEvent);
	JoinServerBtn->OnClicked.AddDynamic(this, &UMainMenu::JoinServerClickEvent);
	BackBtn->OnClicked.AddDynamic(this, &UMainMenu::BackClickEvent);
	Setup();

	return true;
}

void UMainMenu::OnLevelRemovedFromWorld(ULevel* InLevel, UWorld* InWorld)
{
	Super::OnLevelRemovedFromWorld(InLevel, InWorld);

	FInputModeGameOnly gameInputMode;
	APlayerController* controller = GetOwningPlayer();
	controller->SetInputMode(gameInputMode);
	controller->bShowMouseCursor = false;
}

void UMainMenu::Setup()
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

void UMainMenu::HostClickEvent()
{
	GameInstance->Host();
}

void UMainMenu::JoinClickEvent()
{
	if (MenuSwitcher->GetChildrenCount() >= 2)
	{
		MenuSwitcher->SetActiveWidgetIndex(1);
	}
}

void UMainMenu::JoinServerClickEvent()
{
	if (!TargetIP->Text.IsEmpty())
	{
		GameInstance->Join(TargetIP->Text.ToString());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("NO IP detected"));
	}
}

void UMainMenu::BackClickEvent()
{
	MenuSwitcher->SetActiveWidgetIndex(0);
}
