// Copyright of Honeycomb Studio

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MonsterControl.generated.h"

class AMonsterBase;


/**
 * This is the APlayerController class to be used by all player during gameplay
 */
UCLASS()
class HIVE_API AMonsterControl : public APlayerController
{
	GENERATED_BODY()

protected:
	/**
	 * The selected monster class that will be spawned when game begins or when respawn countdown is finished
	 */
	UPROPERTY(Replicated, BlueprintReadOnly)
		TSubclassOf<AMonsterBase> SelectedMonster = NULL;

	bool SpawnCountdown = false;
	float SpawnTimer = 0.0f;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
#pragma region CharacterSelect
	/**
	 * Update the selected monster to the server version of this player controller
	 */
	UFUNCTION(Reliable, Server, WithValidation)
		void UpdateSelectedMonster(TSubclassOf<AMonsterBase> InNewMonster);

	UFUNCTION(Reliable, Server, WithValidation)
		void SpawnSelectedMonster();

	//UFUNCTION(Reliable, Server, WithValidation)
		void SpawnCompleteTest();

	TSubclassOf<AMonsterBase> GetSelectedMonster() { return SelectedMonster; }
#pragma endregion


#pragma region Networking
	/**
	 * Needs to be implemented to initialize replicated properties
	 */
	void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;
#pragma endregion
};
