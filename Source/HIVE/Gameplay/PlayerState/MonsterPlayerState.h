// Copyright of Honeycomb Studio

#pragma once

#include "CoreMinimal.h"
#include "HIVE/Interfaces/TeamInterface.h"
#include "GameFramework/PlayerState.h"
#include "MonsterPlayerState.generated.h"

class APlayerStart;

/**
 * NOTE: A PlayerState contains information about a player that is replicated and visible to all players
 */
UCLASS()
class HIVE_API AMonsterPlayerState : public APlayerState, public ITeamInterface
{
	GENERATED_BODY()

protected:
	UPROPERTY(Replicated, BlueprintReadOnly)
		ETeamEnum TeamID = ETeamEnum::TE_NEUTRAL;

	UPROPERTY(Replicated, BlueprintReadOnly)
		APlayerStart* DefaultSpawnPoint = nullptr;

	UPROPERTY(Replicated, BlueprintReadOnly)
		float SpawnCooldown = 0.0f;

	FTimerHandle SpawnTimerHandle;

public:

	virtual void SetSpawnTimer();
	virtual void AuthorizeSpawning();
	float GetSpawnTimerRemainingTime();

#pragma region Networking
	/**
	 * Needs to be implemented to initialize replicated properties
	 */
	void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;
#pragma endregion

#pragma region TeamInterface
	virtual bool AssignTeam(ETeamEnum InTeamID) override;
	virtual ETeamEnum GetTeam() override { return TeamID; }
	virtual APlayerStart* GetDefaultSpawnPoint() override { return DefaultSpawnPoint; }
	virtual bool SetDefaultSpawnPoint(APlayerStart* InSpawnPoint) override;
#pragma endregion

};
