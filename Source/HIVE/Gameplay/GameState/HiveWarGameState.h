// Copyright of Honeycomb Studio

#pragma once

#include "CoreMinimal.h"
#include "HIVE/Interfaces/TeamInterface.h"
#include "GameFramework/GameState.h"
#include "HiveWarGameState.generated.h"




/**
 * Provide the game state for Hive War mode
 */
UCLASS()
class HIVE_API AHiveWarGameState : public AGameState
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(Replicated)
		float SpawnCountdown;

	UPROPERTY(ReplicatedUsing = WinningTeamRepEvent)
		ETeamEnum WinningTeam = ETeamEnum::TE_INVALID;

public:
	AHiveWarGameState();

	void UpdateGameStartTime(float InUpdatedTime) { SpawnCountdown = InUpdatedTime; }
	float GetSpawnCountdown() { return SpawnCountdown; }
	void SetWinningTeam(ETeamEnum InWinningTeam) { WinningTeam = InWinningTeam; }

	UFUNCTION()
		void WinningTeamRepEvent();

#pragma region Networking
	/**
	 * Needs to be implemented to initialize replicated properties
	 */
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
#pragma endregion
};
