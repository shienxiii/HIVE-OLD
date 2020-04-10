// Copyright of Honeycomb Studio


#include "HiveGameInstance.h"
#include "Engine/World.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Blueprint/UserWidget.h"
#include "HIVE/UI/MenuSystem/MainMenuBase.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/Engine.h"

const static FName SESSION_NAME = TEXT("My Session");

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

	OnlineSubsystem = IOnlineSubsystem::Get();
	check(OnlineSubsystem);
	UE_LOG(LogTemp, Warning, TEXT("Found Subsystem %s"), *OnlineSubsystem->GetSubsystemName().ToString());

	OnlineSessionInterface = OnlineSubsystem->GetSessionInterface();
	check(OnlineSessionInterface);

	OnlineSessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &UHiveGameInstance::CreateSessionComplete);
	OnlineSessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, &UHiveGameInstance::FindSessionsComplete);
	OnlineSessionInterface->OnJoinSessionCompleteDelegates.AddUObject(this, &UHiveGameInstance::JoinSessionComplete);

	SessionSearch = MakeShareable(new FOnlineSessionSearch());
	check(SessionSearch);

}

void UHiveGameInstance::LoadMenu()
{
	if (MenuClass == nullptr) return;
	MainMenu = CreateWidget<UMainMenuBase>(this, MenuClass);
}

void UHiveGameInstance::Host()
{
	if (!OnlineSessionInterface.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("Invalid session interface"));
		return;
	}

	// Check if there is an existing session
	// NOTE: Find a way to destroy session on match end on final game
	auto ExistingSession = OnlineSessionInterface->GetNamedSession(SESSION_NAME);
	if (ExistingSession)
	{
		UE_LOG(LogTemp, Warning, TEXT("Destroying existing session"));
		OnlineSessionInterface->DestroySession(SESSION_NAME);
	}

	FOnlineSessionSettings sessionSettings;
	sessionSettings.bIsLANMatch = false;
	sessionSettings.NumPublicConnections = 10;
	sessionSettings.bShouldAdvertise = true;
	sessionSettings.bUsesPresence = true; // Use presence to allow search to work over Steam

	OnlineSessionInterface->CreateSession(0, SESSION_NAME, sessionSettings);
}

void UHiveGameInstance::FindSessions()
{
	if (SessionSearch.IsValid())
	{
		//SessionSearch->bIsLanQuery = true;
		SessionSearch->MaxSearchResults = 10;
		SessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals); // Setting the search query to search presence

		OnlineSessionInterface->FindSessions(0, SessionSearch.ToSharedRef());
	}
}

void UHiveGameInstance::Join(uint32 InIndex)
{
	if (!OnlineSessionInterface.IsValid()) { return; }
	if (!SessionSearch.IsValid()) { return; }

	OnlineSessionInterface->JoinSession(0, SESSION_NAME, SessionSearch->SearchResults[InIndex]);
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

		MainMenu->PopulateSessionList(SessionSearch->SearchResults);
	}
}

void UHiveGameInstance::JoinSessionComplete(FName InName, const EOnJoinSessionCompleteResult::Type InJoinSessionCompleteResult)
{
	if (!OnlineSessionInterface) { return; }

	// Get the string needed to do a client travel to join the match
	FString Address;
	if (!OnlineSessionInterface->GetResolvedConnectString(InName, Address))
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to get connection string to join match"));
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("Completed Session Joining"));

	APlayerController* controller = GetFirstLocalPlayerController();
	if (!ensure(controller != nullptr)) { return; }

	controller->ClientTravel(Address, ETravelType::TRAVEL_Absolute);
}
