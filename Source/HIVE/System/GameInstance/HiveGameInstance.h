// Copyright of Honeycomb Studio

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "HiveGameInstance.generated.h"


struct FInputModeDataBase;
/**
 * 
 */
UCLASS()
class HIVE_API UHiveGameInstance : public UGameInstance
{
	GENERATED_BODY()
private:
	TSubclassOf<class UUserWidget> MenuClass;

public:
	UHiveGameInstance(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(Exec, BlueprintCallable)
		void LoadMenu();


#pragma region Multiplayer
	UFUNCTION(Exec)
		void Host();

	UFUNCTION(Exec)
		void Join(const FString& InAddress);

	UFUNCTION(Exec)
		void ExitGame();

	/**
	 * Called to return to the game lobby
	 *
	 * @param InPlayerController The PlayerController instance that represent the player to be returned to the lobby
	 */
	UFUNCTION() void ReturnToLobby(APlayerController* InPlayerController);
#pragma endregion
};
