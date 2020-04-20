// Copyright of Honeycomb Studio

#pragma once

#include "CoreMinimal.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "Interfaces/OnlineUserInterface.h"
#include "Interfaces/OnlineIdentityInterface.h"
#include "UObject/NoExportTypes.h"
#include "OnlineSubsystemInterface.generated.h"

/**
 * Custom made online subsystem interface to make it easier to link to an online subsystem
 */
UCLASS()
class HIVE_API UOnlineSubsystemInterface : public UObject
{
	GENERATED_BODY()

protected:
		IOnlineSubsystem* OnlineSubsystem;
		IOnlineSessionPtr SessionInterface;
		TSharedPtr<FOnlineSessionSearch> SessionSearch;
		IOnlineUserPtr OnlineUser;
		IOnlineIdentityPtr OnlineIdentity;

public:
	//FOnlineSessionSettings DefaultSessionSetting;

	UOnlineSubsystemInterface();
	~UOnlineSubsystemInterface();
	void InitializePlayerOnlineIdentity();


	IOnlineSessionPtr GetSessionInterface() { return SessionInterface; }
	TSharedPtr<FOnlineSessionSearch> GetSessionSearch() { return SessionSearch; }


	bool CreateSession(int32 HostingPlayerNum);
	bool FindSession();
	bool JoinSession(int32 SessionIndex);
	void DestroyExistingSession();
	// CMD: Log LogOnline VeryVerbose
};
