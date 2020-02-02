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
	int8 GetFreeSpawnPoint(AController* InController);
	TArray<AMonsterSpawnPoint*> GetSpawnPoints() { return SpawnPoints; }
	TArray<AController*> GetMembersList() { return Members; }

	bool AddSpawnPoint(AMonsterSpawnPoint* InNewSpawnPoint);
	bool AddToTeam(AController* InController) { Members.Add(InController); }
	void AssignSpawnPointToPlayer(int8 InSpawnPointIndex, AController* InController);

};

/**
 * 
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
};
