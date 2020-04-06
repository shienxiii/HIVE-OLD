// Copyright of Honeycomb Studio


#include "HiveGameInstance.h"
#include "Engine/World.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Blueprint/UserWidget.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/Engine.h"

const static FName SESSION_NAME = TEXT("My Session");

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

void UHiveGameInstance::Init()
{
	Super::Init();

	OnlineSubsystem = IOnlineSubsystem::Get();
	check(OnlineSubsystem);

	SessionInterface = OnlineSubsystem->GetSessionInterface();
	check(SessionInterface);

	SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &UHiveGameInstance::CreateSessionComplete);
	SessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, &UHiveGameInstance::FindSessionsComplete);

	SessionSearch = MakeShareable(new FOnlineSessionSearch());
	check(SessionSearch);

}

void UHiveGameInstance::LoadMenu()
{
	if (MenuClass == nullptr) return;
	UE_LOG(LogTemp, Warning, TEXT("Generating Widget"));
	UUserWidget* menu = CreateWidget<UUserWidget>(this, MenuClass);
}

void UHiveGameInstance::Host()
{
	if (!SessionInterface.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("Invalid session interface"));
		return;
	}

	// Check if there is an existing session
	// NOTE: Find a way to destroy session on match end on final game
	auto ExistingSession = SessionInterface->GetNamedSession(SESSION_NAME);
	if (ExistingSession)
	{
		UE_LOG(LogTemp, Warning, TEXT("Destroying existing session"));
		SessionInterface->DestroySession(SESSION_NAME);
	}

	FOnlineSessionSettings sessionSettings;
	SessionInterface->CreateSession(0, SESSION_NAME, sessionSettings);
}

void UHiveGameInstance::FindSessions()
{
	if (SessionSearch.IsValid())
	{
		SessionInterface->FindSessions(0, SessionSearch.ToSharedRef());
	}
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

		TArray<FOnlineSessionSearchResult> results = SessionSearch->SearchResults;

		for (int i = 0; i < results.Num(); i++)
		{
			UE_LOG(LogTemp, Warning, TEXT("%s"), *results[i].GetSessionIdStr());
		}
	}
}
