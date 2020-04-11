// Copyright of Honeycomb Studio

#pragma once

#include "CoreMinimal.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"
#include "Interfaces/OnlineSessionInterface.h"
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

public:
	//FOnlineSessionSettings DefaultSessionSetting;

	UOnlineSubsystemInterface();
	~UOnlineSubsystemInterface();
	IOnlineSessionPtr GetSessionInterface() { return SessionInterface; }
	TSharedPtr<FOnlineSessionSearch> GetSessionSearch() { return SessionSearch; }


	bool CreateSession(int32 HostingPlayerNum, const FOnlineSessionSettings& NewSessionSettings);
	bool FindSession();
	bool JoinSession(int32 SessionIndex);
	// CMD: Log LogOnline VeryVerbose
};
