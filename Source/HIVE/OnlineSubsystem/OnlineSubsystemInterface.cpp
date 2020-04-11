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
}

UOnlineSubsystemInterface::~UOnlineSubsystemInterface()
{
	OnlineSubsystem = NULL;
	SessionInterface = NULL;
}

bool UOnlineSubsystemInterface::CreateSession(int32 HostingPlayerNum, const FOnlineSessionSettings& NewSessionSettings)
{
	if (!SessionInterface) { UE_LOG(LogTemp, Warning, TEXT("Invalid session interface")); return false; }

	// Check if there is an existing session
	// NOTE: Find a way to destroy session on match end on final game
	auto ExistingSession = SessionInterface->GetNamedSession(SESSION_NAME);
	if (ExistingSession)
	{
		UE_LOG(LogTemp, Warning, TEXT("Destroying existing session"));
		SessionInterface->DestroySession(SESSION_NAME);
	}

	SessionInterface->CreateSession(0, SESSION_NAME, NewSessionSettings);
	SessionInterface->FindSessions(0, SessionSearch.ToSharedRef());
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
