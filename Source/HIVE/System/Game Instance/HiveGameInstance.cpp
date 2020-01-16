// Copyright of Honeycomb Studio


#include "HiveGameInstance.h"
#include "Engine/World.h"
#include "Blueprint/UserWidget.h"
#include "UObject/ConstructorHelpers.h"

UHiveGameInstance::UHiveGameInstance(const FObjectInitializer& ObjectInitializer)
{
	static ConstructorHelpers::FClassFinder<UUserWidget> MenuBP(TEXT("/Game/Blueprint/UI/MainMenu/LAN_Menu.LAN_Menu_C"));

	if (MenuBP.Class != nullptr)
	{
		MenuClass = MenuBP.Class;
	}
}

void UHiveGameInstance::LoadMenu()
{
	if (MenuClass == nullptr) return;

	UUserWidget* menu = CreateWidget<UUserWidget>(this, MenuClass);
	menu->AddToViewport();

	APlayerController* controller = GetFirstLocalPlayerController();
	if (!ensure(controller != nullptr)) { return; }

	FInputModeUIOnly uiInputMode;
	uiInputMode.SetWidgetToFocus(menu->TakeWidget());
	uiInputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);

	SetupPlayerController(uiInputMode, true, controller);

}

void UHiveGameInstance::SetupPlayerController(const FInputModeDataBase& InInputMode, bool bShowMouseCursor, APlayerController* InPlayerController)
{
	if (!InPlayerController)
	{
		InPlayerController = GetFirstLocalPlayerController();

	}

	InPlayerController->SetInputMode(InInputMode);
	InPlayerController->bShowMouseCursor = bShowMouseCursor;
}

void UHiveGameInstance::Host()
{
	UWorld* world = GetWorld();
	if (!ensure(world != nullptr)) { return; }

	world->ServerTravel("/Game/Blueprint/Maps/ThirdPersonExampleMap?listen");
}

void UHiveGameInstance::Join(const FString& InAddress)
{
	APlayerController* controller = GetFirstLocalPlayerController();
	if (!ensure(controller != nullptr)) { return; }

	controller->ClientTravel(*InAddress, ETravelType::TRAVEL_Absolute);
}
