// Copyright of Honeycomb Studio

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "GM_HiveWar.generated.h"

class AMonsterControl;
class AMonsterSpawnPoint;

//USTRUCT(BlueprintType)
USTRUCT()
struct FHiveTeam
{
	GENERATED_BODY();

	uint8 TeamIndex;
	TArray<AMonsterSpawnPoint*> TeamSpawnPoints;

	FHiveTeam()
	{
		TeamIndex = 0;
		TeamSpawnPoints = TArray<AMonsterSpawnPoint*>();
	}

	FHiveTeam(uint8 InTeamIndex)
	{
		TeamIndex = InTeamIndex;
		TeamSpawnPoints = TArray<AMonsterSpawnPoint*>();
	}

	void AddSpawnPoint(AMonsterSpawnPoint* InNewSpawnPoint)
	{
		TeamSpawnPoints.Add(InNewSpawnPoint);
	}
};

/**
 * 
 */
UCLASS()
class HIVE_API AGM_HiveWar : public AGameMode
{
	GENERATED_BODY()

protected:
	TMap<uint8, TArray<AMonsterSpawnPoint*>> TeamSpawnPoints;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
public:
	AGM_HiveWar();

	void SpawnMonsterForController(AMonsterControl* InPlayerControl);

	virtual void PostLogin(APlayerController* InPlayerController) override;
};
