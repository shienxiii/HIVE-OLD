// Copyright of Honeycomb Studio

#pragma once

#include "CoreMinimal.h"
#include "HIVE/Gameplay/Monster/MonsterSpawnPoint.h"
#include "HIVE/Interfaces/TeamInterface.h"
#include "GameFramework/GameMode.h"
#include "GM_HiveWar.generated.h"

class AMonsterController;

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
		TArray<AController*> Members;

public:
	FTeamSpawnArea()
	{
		TeamID		= ETeamEnum::TE_NEUTRAL;
		SpawnPoints	= TArray<AMonsterSpawnPoint*>();
		Members		= TArray<AController*>();
	}

	FTeamSpawnArea(ETeamEnum InTeamID)
	{
		TeamID		= InTeamID;
		SpawnPoints	= TArray<AMonsterSpawnPoint*>();
		Members		= TArray<AController*>();
	}

	// Return the index of the next free spawn point
	int32 GetFreeSpawnPoint(AController* InController);
	int32 AvailableSpawnPoints();
	TArray<AMonsterSpawnPoint*> GetSpawnPoints() { return SpawnPoints; }
	TArray<AController*> GetMembersList() { return Members; }

	bool AddSpawnPoint(AMonsterSpawnPoint* InNewSpawnPoint);
	bool AddToTeam(AController* InController);
	void AssignSpawnPointToPlayer(int8 InSpawnPointIndex, AController* InController);

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

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
public:
	AGM_HiveWar();

	void SpawnMonsterForController(AMonsterController* InPlayerControl);

	virtual void PostLogin(APlayerController* InPlayerController) override;

	/**
	 * Allocate the controller to a team and returns the team it is allocated to
	 *
	 * @param	InController The controller to assign a team to
	 * @ret		The team InController is assigned to
	 */
	virtual ETeamEnum AllocateToTeam(AController* InController);

};
