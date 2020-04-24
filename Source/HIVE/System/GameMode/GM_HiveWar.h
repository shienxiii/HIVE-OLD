// Copyright of Honeycomb Studio

#pragma once

#include "CoreMinimal.h"
#include "HIVE/Gameplay/Monster/MonsterSpawnPoint.h"
#include "HIVE/Interfaces/TeamInterface.h"
#include "GameFramework/GameMode.h"
#include "GM_HiveWar.generated.h"

class AHiveWarGameState;
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



	TArray<AMonsterSpawnPoint*> GetSpawnPoints() { return SpawnPoints; }
	TArray<APlayerState*> GetMembersList() { return Members; }

	int32 AvailableSpawnPoints(){ return SpawnPoints.Num() - Members.Num(); }
	int32 GetMemberCount() { return Members.Num(); }


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
	TMap<ETeamEnum, FTeamSpawnArea> TeamSpawnMap;
	TArray<AMonsterController*> PlayerList;

	int32 MatchPlayerCount = 2;

	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
public:
	AGM_HiveWar();
	virtual void Tick(float DeltaTime) override;


	virtual void PostLogin(APlayerController* InPlayerController) override;
	virtual void Logout(AController* ExitingPlayer) override;

	virtual void BeginTeamAllocation();

	/**
	 * Allocate the controller to a team and returns the team it is allocated to
	 *
	 * @param	InPlayerState The PlayerState to assign a team to
	 * @ret		The team InController is assigned to
	 */
	virtual ETeamEnum AllocateToTeam(APlayerState* InPlayerState);


	void SpawnMonsterForController(AMonsterController* InPlayerControl);

	UFUNCTION(Exec)
		void StartGame();

	UFUNCTION()
		void GameOver(AActor* InDeadHive);

	AHiveWarGameState* GetHiveWarGameState();
};
