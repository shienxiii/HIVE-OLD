// Copyright of Honeycomb Studio

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "HiveGameInstance.generated.h"


class UMainMenuBase;
/**
 * 
 */
UCLASS()
class HIVE_API UHiveGameInstance : public UGameInstance
{
	GENERATED_BODY()
private:
	// Online subsystem properties
	IOnlineSubsystem* OnlineSubsystem;
	IOnlineSessionPtr OnlineSessionInterface;
	TSharedPtr<FOnlineSessionSearch> SessionSearch;


	TSubclassOf<class UMainMenuBase> MenuClass;
	UMainMenuBase* MainMenu;

public:
	UHiveGameInstance(const FObjectInitializer& ObjectInitializer);
	virtual void Init() override;


	UFUNCTION(Exec, BlueprintCallable)
		void LoadMenu();


#pragma region Multiplayer
	UFUNCTION(Exec)
		void Host();

	UFUNCTION()
		void FindSessions();

	UFUNCTION(Exec)
		void Join(uint32 InIndex);

	UFUNCTION(Exec)
		void ExitGame();

	/**
	 * Called to return to the game lobby
	 *
	 * @param InPlayerController The PlayerController instance that represent the player to be returned to the lobby
	 */
	UFUNCTION() void ReturnToLobby(APlayerController* InPlayerController);

	/**
	 * To be binded to the online subsystem delegate that runs when an online session is created
	 */
	UFUNCTION() void CreateSessionComplete(FName InSessionName, bool Success);

	/**
	 * To be binded to the online subsystem delegate that runs when an online session is created
	 */
	UFUNCTION() void FindSessionsComplete(bool wasSuccessful);

	//UFUNCTION() commented out because it causes error with EJoinSessionCompleteResult parameter
	void JoinSessionComplete(FName InName, const EOnJoinSessionCompleteResult::Type InJoinSessionCompleteResult);
#pragma endregion
};
