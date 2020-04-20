// Copyright of Honeycomb Studio


#include "OnlineSubsystemInterface.h"

const static FName SESSION_NAME = TEXT("My Session");

UOnlineSubsystemInterface::UOnlineSubsystemInterface()
{
	OnlineSubsystem = IOnlineSubsystem::Get();
	
	check(OnlineSubsystem);
	UE_LOG(LogTemp, Warning, TEXT("Found Online Subsystem %s"), *OnlineSubsystem->GetSubsystemName().ToString());

	SessionInterface = OnlineSubsystem->GetSessionInterface();
	check(SessionInterface);

	SessionSearch = MakeShareable(new FOnlineSessionSearch());
	check(SessionSearch);

	// Delegates are initialized in UHiveGameInstance::Init
}

UOnlineSubsystemInterface::~UOnlineSubsystemInterface()
{
	OnlineSubsystem = NULL;
	SessionInterface = NULL;
}

void UOnlineSubsystemInterface::InitializePlayerOnlineIdentity()
{
	if (OnlineSubsystem->GetSubsystemName() == FName("NULL"))
	{
		UE_LOG(LogTemp, Warning, TEXT("Using NULL subsystem"));
		return;
	}


	//OnlineUser = OnlineSubsystem->GetUserInterface();
	OnlineIdentity = OnlineSubsystem->GetIdentityInterface();
	check(OnlineIdentity);

	TSharedPtr<const FUniqueNetId> id = OnlineIdentity->GetUniquePlayerId(0);
	OnlineIdentity->GetPlayerNickname(*id.Get());
	UE_LOG(LogTemp, Warning, TEXT("UniqueNetID %s"), *id.Get()->ToString());
	UE_LOG(LogTemp, Warning, TEXT("Name %s"), *OnlineIdentity->GetPlayerNickname(*id.Get()));

	TSharedPtr<FUserOnlineAccount> p = OnlineIdentity->GetUserAccount(*id.Get());
	if (p.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("Found Online Account"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("No Online Account Found"));
	}
}

bool UOnlineSubsystemInterface::CreateSession(int32 HostingPlayerNum)
{
	if (!SessionInterface) { UE_LOG(LogTemp, Warning, TEXT("Invalid session interface")); return false; }

	DestroyExistingSession();

	FOnlineSessionSettings sessionSettings;
	sessionSettings.bIsLANMatch = (OnlineSubsystem->GetSubsystemName() == "NULL");
	sessionSettings.NumPublicConnections = 10;
	sessionSettings.bShouldAdvertise = true;
	sessionSettings.bUsesPresence = true; // Enable on both server and search for steam to use lobbies
	sessionSettings.Set(TEXT("Name"), FString("RandomName"), EOnlineDataAdvertisementType::ViaOnlineServiceAndPing); // Used to create a custom session setting

	SessionInterface->CreateSession(0, SESSION_NAME, sessionSettings);

	return true;
}

bool UOnlineSubsystemInterface::FindSession()
{
	if (!SessionSearch) { UE_LOG(LogTemp, Warning, TEXT("Invalid session search")); return false; }

	//SessionSearch->bIsLanQuery = true;
	SessionSearch->MaxSearchResults = 10;
	SessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals); // Setting the search query to search presence

	SessionInterface->FindSessions(0, SessionSearch.ToSharedRef());

	return true;
}

bool UOnlineSubsystemInterface::JoinSession(int32 SessionIndex)
{
	if (!SessionInterface.IsValid()) { return false; }
	if (!SessionSearch.IsValid()) { return false; }

	SessionInterface->JoinSession(0, SESSION_NAME, SessionSearch->SearchResults[SessionIndex]);

	return true;
}

void UOnlineSubsystemInterface::DestroyExistingSession()
{
	// Check if there is an existing session
	// NOTE: Find a way to destroy session on match end on final game
	auto ExistingSession = SessionInterface->GetNamedSession(SESSION_NAME);
	if (ExistingSession)
	{
		UE_LOG(LogTemp, Warning, TEXT("Destroying existing session"));
		SessionInterface->DestroySession(SESSION_NAME);
	}
}
