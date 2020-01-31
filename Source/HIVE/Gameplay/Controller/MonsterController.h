// Copyright of Honeycomb Studio

#pragma once

#include "CoreMinimal.h"
#include "HIVE/Interfaces/TeamInterface.h"
#include "GameFramework/PlayerController.h"
#include "MonsterController.generated.h"

class AMonsterBase;
class AMonsterPlayerState;
class UCharacterSelectBase;

/**
 * This is the APlayerController class to be used by all player during gameplay
 */
UCLASS()
class HIVE_API AMonsterController : public APlayerController, public ITeamInterface
{
	GENERATED_BODY()

protected:
#pragma region CharacterSelect
	FInputModeGameOnly	GameInputMode;
	FInputModeUIOnly	CharSelInputMode;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		TSubclassOf<UCharacterSelectBase> CharacterSelectBP = NULL;

	UCharacterSelectBase* CharacterSelect = nullptr;

	/**
	 * The selected monster class that will be spawned when game begins or when respawn countdown is finished
	 */
	UPROPERTY(Replicated, BlueprintReadOnly)
		TSubclassOf<AMonsterBase> SelectedMonster = NULL;
#pragma endregion

	bool SpawnCountdown = false;
	float SpawnTimer = 0.0f;


	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
public:
	AMonsterController(const FObjectInitializer& ObjectInitializer);

#pragma region CharacterSelect
	/**
	 * Update the selected monster to the server version of this player controller
	 */
	UFUNCTION(Reliable, Server, WithValidation)
		void UpdateSelectedMonster(TSubclassOf<AMonsterBase> InNewMonster);

	UFUNCTION(Reliable, Server, WithValidation)
		void SpawnSelectedMonster();

		void ToggleCharacterSelectScreen(bool ToggleOn);

	TSubclassOf<AMonsterBase> GetSelectedMonster() { return SelectedMonster; }
#pragma endregion


#pragma region Networking
	/**
	 * Needs to be implemented to initialize replicated properties
	 */
	void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;
#pragma endregion

#pragma region TeamInterface
	/**
	 * Assign the team on the relevant PlayerState
	 */
	virtual void AssignTeam(uint8 InTeam);

	/**
	 * Search the relevant PlayerState and determine the team this MonsterController belongs to
	 */
	UFUNCTION(BlueprintPure)
		virtual uint8 GetTeam() override;
#pragma endregion
};
