// Copyright of Honeycomb Studio

#pragma once

#include "CoreMinimal.h"
#include "HIVE/Gameplay/Monster/MonsterSpawnPoint.h"
#include "HIVE/Interfaces/TeamInterface.h"
#include "GameFramework/GameMode.h"
#include "GM_HiveWar.generated.h"


class AMonsterController;


UENUM()
enum class EGamePhase : uint8
{
	GP_PREGAME,
	GP_MIDGAME,
	GP_ENDGAME
};

USTRUCT()
struct FTeamSpawnArea
{
	GENERATED_BODY()

private:

	UPROPERTY()
		ETeamEnum TeamID;

	UPROPERTY()
		TArray<AMonsterSpawnPoint*> SpawnPoints;

	UPROPERTY()
		TArray<APlayerState*> Members;

public:
	FTeamSpawnArea()
	{
		TeamID		= ETeamEnum::TE_NEUTRAL;
		SpawnPoints	= TArray<AMonsterSpawnPoint*>();
		Members		= TArray<APlayerState*>();
	}

	FTeamSpawnArea(ETeamEnum InTeamID)
	{
		TeamID		= InTeamID;
		SpawnPoints	= TArray<AMonsterSpawnPoint*>();
		Members		= TArray<APlayerState*>();
	}


	int32 AvailableSpawnPoints();
	TArray<AMonsterSpawnPoint*> GetSpawnPoints() { return SpawnPoints; }
	TArray<APlayerState*> GetMembersList() { return Members; }

	bool AddSpawnPoint(AMonsterSpawnPoint* InNewSpawnPoint);
	bool AddToTeam(APlayerState* InPlayerState);

	void SortSpawnPoints() { SpawnPoints.Sort(); }

};


/**
 * GameMode for any arena stage
 */
UCLASS()
class HIVE_API AGM_HiveWar : public AGameMode
{
	GENERATED_BODY()

protected:
	TMap<ETeamEnum, FTeamSpawnArea> TeamSpawnPoints;

	// Boolean to decide if the game is in a state where they can spawn the player character
	bool bCanSpawnPlayerCharacter = false;

	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
public:
	AGM_HiveWar();
	virtual void Tick(float DeltaTime) override;

	void SpawnMonsterForController(AMonsterController* InPlayerControl);

	virtual void PostLogin(APlayerController* InPlayerController) override;

	virtual void BeginTeamAllocation();

	/**
	 * Allocate the controller to a team and returns the team it is allocated to
	 *
	 * @param	InPlayerState The PlayerState to assign a team to
	 * @ret		The team InController is assigned to
	 */
	virtual ETeamEnum AllocateToTeam(APlayerState* InPlayerState);

	UFUNCTION(Exec)
		void StartGame();

	UFUNCTION()
		void GameOver(AActor* InDeadHive);
};
