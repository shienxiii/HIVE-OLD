// Copyright of Honeycomb Studio


#include "MainMenuBase.h"
#include "Components/Button.h"
#include "HIVE/System/Game Instance/HiveGameInstance.h"

bool UMainMenuBase::Initialize()
{
	Super::Initialize();

	bIsFocusable = true;

	GameInstance = Cast<UHiveGameInstance>(GetGameInstance());
	if (!GameInstance) { return false; }

	HostBtn->OnClicked.AddDynamic(this, &UMainMenuBase::HostClickEvent);
	JoinBtn->OnClicked.AddDynamic(this, &UMainMenuBase::JoinClickEvent);
	JoinServerBtn->OnClicked.AddDynamic(this, &UMainMenuBase::JoinServerClickEvent);
	BackBtn->OnClicked.AddDynamic(this, &UMainMenuBase::BackClickEvent);
	Setup();

	return true;
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

void UMainMenuBase::HostClickEvent()
{
	GameInstance->Host();
}

void UMainMenuBase::JoinClickEvent()
{
	if (MenuSwitcher->GetChildrenCount() >= 2)
	{
		MenuSwitcher->SetActiveWidgetIndex(1);
	}
}

void UMainMenuBase::JoinServerClickEvent()
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

void UMainMenuBase::BackClickEvent()
{
	MenuSwitcher->SetActiveWidgetIndex(0);
}
