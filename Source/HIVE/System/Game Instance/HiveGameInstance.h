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

	/**
	 * Setup the player controller input mode
	 *
	 * @param InInputMode The new input mode to set this controller to
	 * @param bShowMouseCursor Set whether or not to show the mouse cursor on the viewport. Default is false
	 * @param InPlayerController The player controller to set the new input mode to. Default will set to the player controller 0
	 */
	void SetupPlayerController(const FInputModeDataBase& InInputMode, bool bShowMouseCursor = false, APlayerController* InPlayerController = nullptr);


#pragma region Multiplayer
	UFUNCTION(Exec)
		void Host();

	UFUNCTION(Exec)
		void Join(const FString& InAddress);
#pragma endregion
};
