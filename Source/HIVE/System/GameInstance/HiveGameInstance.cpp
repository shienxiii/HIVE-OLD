// Copyright of Honeycomb Studio


#include "HiveGameInstance.h"
#include "Engine/World.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Blueprint/UserWidget.h"
#include "UObject/ConstructorHelpers.h"

UHiveGameInstance::UHiveGameInstance(const FObjectInitializer& ObjectInitializer)
{
	//static ConstructorHelpers::FClassFinder<UUserWidget> MenuBP(TEXT("/Game/Blueprint/UI/MainMenu/LAN_Menu.LAN_Menu_C"));
	static ConstructorHelpers::FClassFinder<UUserWidget> MenuBP(TEXT("/Game/Blueprint/UI/MainMenu/MainMenu.MainMenu_C"));

	if (MenuBP.Class != nullptr)
	{
		MenuClass = MenuBP.Class;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Fail to find BP"));
	}
}

void UHiveGameInstance::LoadMenu()
{
	if (MenuClass == nullptr) return;
	UE_LOG(LogTemp, Warning, TEXT("Generating Widget"));
	UUserWidget* menu = CreateWidget<UUserWidget>(this, MenuClass);
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

void UHiveGameInstance::ExitGame()
{
	APlayerController* controller = GetFirstLocalPlayerController();
	if (!ensure(controller != nullptr)) { return; }

	//FGenericPlatformMisc::RequestExit(false);
	UKismetSystemLibrary::QuitGame(controller, controller, EQuitPreference::Quit, true);
}

void UHiveGameInstance::ReturnToLobby(APlayerController* InPlayerController)
{
	InPlayerController->ClientTravel(TEXT("/Game/Blueprint/Maps/Lobby.Lobby_C"), ETravelType::TRAVEL_Absolute);
}