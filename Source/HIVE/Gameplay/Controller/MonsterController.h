// Copyright of Honeycomb Studio

#pragma once

#include "CoreMinimal.h"
#include "HIVE/Interfaces/TeamInterface.h"
#include "GameFramework/PlayerController.h"
#include "MonsterController.generated.h"

class AMonsterBase;
class AMonsterPlayerState;
class UHiveWarHUD_Base;
class UCharacterSelectBase;
class AHiveWarGameState;


/**
 * NOTE: APlayerController only exist in server and the owning client
 * This is the APlayerController class to be used by all player during gameplay
 */
UCLASS()
class HIVE_API AMonsterController : public APlayerController, public ITeamInterface
{
	GENERATED_BODY()

protected:
	UPROPERTY()
		UHiveWarHUD_Base* HUD;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		TSubclassOf<UHiveWarHUD_Base> HUD_BP = NULL;


	// Indicator used to ensure that player data is updated on the server before allowing them to exit on game end
	UPROPERTY(Replicated, BlueprintReadOnly) bool bCanExitGameEnd = false; 


	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	//virtual void Tick(float DeltaTime) override;
	AMonsterController(const FObjectInitializer& ObjectInitializer);

	// Called to bind functionality to input
	virtual void SetupInputComponent() override;

	// Initialize the HUD for the player on the owning client
	UFUNCTION(Client, Reliable)	virtual void SetupPlayerHUD();

	// Show the waiting screen on the owning client
	UFUNCTION(Client, Reliable)	virtual void LoadWaitScreen();

	// Show the character select screen on the owning client
	UFUNCTION(Client, Reliable)	virtual void LoadCharacterSelectScreen();

	UFUNCTION()	void StartButtonEvent();

	// When the player controlled pawn restarted, call this function. Currently called by AMonsterBase::Restart, which gets called when the pawn is possessed
	void PawnRestarted(AMonsterBase* InMonster);

	virtual void OnUnPossess() override;

	AMonsterPlayerState* GetMonsterPlayerState();

#pragma region ImportantEvents

	UFUNCTION()	virtual void GameHasEnded(AActor* EndGameFocus, bool bIsWinner) override;

	UFUNCTION()	virtual void SetCanExitGameEnd();
#pragma endregion

#pragma region CharacterSelect
	// Update the selected monster to the server version of this player controller
	UFUNCTION(Reliable, Server, WithValidation)
		void UpdateSelectedMonster(TSubclassOf<AMonsterBase> InNewMonster);

	UFUNCTION(Reliable, Server, WithValidation)
		void SpawnSelectedMonster();

	bool CanSpawnMonster();

	TSubclassOf<AMonsterBase> GetSelectedMonster();
#pragma endregion



#pragma region TeamInterface
	// Assign the team on the relevant PlayerState and return a bool indicating whether the assignment is successful or not
	virtual bool AssignTeam(ETeamEnum InTeam) override;

	// Search the relevant PlayerState and determine the team this MonsterController belongs to
	UFUNCTION(BlueprintPure)
		virtual ETeamEnum GetTeam() override;
#pragma endregion


	// Needs to be implemented to initialize replicated properties
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
