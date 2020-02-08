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
	UHiveWarHUD_Base* HUD;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		TSubclassOf<UHiveWarHUD_Base> HUD_BP = NULL;

	/**
	 * The selected monster class that will be spawned when game begins or when respawn countdown is finished
	 */
	UPROPERTY(Replicated, BlueprintReadOnly)
		TSubclassOf<AMonsterBase> SelectedMonster = NULL;

	

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Runs on server side when possessing a pawn
	virtual void OnPossess(APawn* InPawn) override;
public:
	//virtual void Tick(float DeltaTime) override;
	AMonsterController(const FObjectInitializer& ObjectInitializer);
	void PawnRestarted(AMonsterBase* InMonster);


#pragma region CharacterSelect
	/**
	 * Update the selected monster to the server version of this player controller
	 */
	UFUNCTION(Reliable, Server, WithValidation)
		void UpdateSelectedMonster(TSubclassOf<AMonsterBase> InNewMonster);

	UFUNCTION(Reliable, Server, WithValidation)
		void SpawnSelectedMonster();

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
	 * Assign the team on the relevant PlayerState and return a bool indicating whether the assignment is successful or not
	 */
	virtual bool AssignTeam(ETeamEnum InTeam) override;

	/**
	 * Search the relevant PlayerState and determine the team this MonsterController belongs to
	 */
	UFUNCTION(BlueprintPure)
		virtual ETeamEnum GetTeam() override;
#pragma endregion
};
