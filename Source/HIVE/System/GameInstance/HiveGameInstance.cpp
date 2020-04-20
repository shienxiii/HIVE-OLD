// Copyright of Honeycomb Studio


#include "HiveGameInstance.h"
#include "Engine/World.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Blueprint/UserWidget.h"
#include "HIVE/UI/MenuSystem/MainMenuBase.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/Engine.h"


UHiveGameInstance::UHiveGameInstance(const FObjectInitializer& ObjectInitializer)
{
	//static ConstructorHelpers::FClassFinder<UUserWidget> MenuBP(TEXT("/Game/Blueprint/UI/MainMenu/LAN_Menu.LAN_Menu_C"));
	static ConstructorHelpers::FClassFinder<UMainMenuBase> MenuBP(TEXT("/Game/Blueprint/UI/MainMenu/MainMenu.MainMenu_C"));

	if (MenuBP.Class != nullptr)
	{
		MenuClass = MenuBP.Class;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Fail to find BP"));
	}
}

void UHiveGameInstance::Init()
{
	Super::Init();

	OnlineInterface = NewObject<UOnlineSubsystemInterface>(this);

	if (OnlineInterface->GetSessionInterface())
	{
		OnlineInterface->GetSessionInterface()->OnCreateSessionCompleteDelegates.AddUObject(this, &UHiveGameInstance::CreateSessionComplete);
		OnlineInterface->GetSessionInterface()->OnFindSessionsCompleteDelegates.AddUObject(this, &UHiveGameInstance::FindSessionsComplete);
		OnlineInterface->GetSessionInterface()->OnJoinSessionCompleteDelegates.AddUObject(this, &UHiveGameInstance::JoinSessionComplete);
	}

	OnlineInterface->InitializePlayerOnlineIdentity();
}

void UHiveGameInstance::BeginDestroy()
{
	Super::BeginDestroy();
}

void UHiveGameInstance::LoadMenu()
{
	if (MenuClass == nullptr) return;

	if (!MainMenu)
	{
		MainMenu = CreateWidget<UMainMenuBase>(this, MenuClass);
	}
	
	MainMenu->AddToViewport();

	OnlineInterface->DestroyExistingSession();
}

void UHiveGameInstance::Host()
{
	OnlineInterface->CreateSession(0);
}

void UHiveGameInstance::FindSessions()
{
	OnlineInterface->FindSession();
}

void UHiveGameInstance::Join(uint32 InIndex)
{
	OnlineInterface->JoinSession(InIndex);
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

void UHiveGameInstance::CreateSessionComplete(FName InSessionName, bool InSuccess)
{
	if (!InSuccess)
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to create session named %s"), *InSessionName.ToString());
		return;
	}


	UE_LOG(LogTemp, Warning, TEXT("Successfully created session named %s"), *InSessionName.ToString());
	UWorld* world = GetWorld();
	if (!ensure(world != nullptr)) { return; }

	world->ServerTravel("/Game/Blueprint/Maps/ThirdPersonExampleMap?listen");
}

void UHiveGameInstance::FindSessionsComplete(bool wasSuccessful)
{
	if(wasSuccessful)
	{
		UE_LOG(LogTemp, Warning, TEXT("Session search successful"));

		if (MainMenu)
		{
			MainMenu->PopulateSessionList(OnlineInterface->GetSessionSearch()->SearchResults);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Error, MainMenu not found"));
		}
	}
}

void UHiveGameInstance::JoinSessionComplete(FName InName, const EOnJoinSessionCompleteResult::Type InJoinSessionCompleteResult)
{
	if (!OnlineInterface) { UE_LOG(LogTemp, Warning, TEXT("Failed to get OnlineInterface")); return; }

	// Get the string needed to do a client travel to join the match
	FString Address;
	if (!OnlineInterface->GetSessionInterface()->GetResolvedConnectString(InName, Address))
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to get connection string to join match"));
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("Completed Session Joining"));

	APlayerController* controller = GetFirstLocalPlayerController();
	if (!ensure(controller != nullptr)) { UE_LOG(LogTemp, Warning, TEXT("Failed to get player controller")); return; }

	controller->ClientTravel(Address, ETravelType::TRAVEL_Absolute);
}
