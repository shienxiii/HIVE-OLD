// Copyright of Honeycomb Studio

#pragma once

#include "CoreMinimal.h"
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

public:
	AHiveWarGameState();

	void UpdateGameStartTime(float InUpdatedTime) { SpawnCountdown = InUpdatedTime; }
	float GetSpawnCountdown() { return SpawnCountdown; }

#pragma region Networking
	/**
	 * Needs to be implemented to initialize replicated properties
	 */
	void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;
#pragma endregion
};
