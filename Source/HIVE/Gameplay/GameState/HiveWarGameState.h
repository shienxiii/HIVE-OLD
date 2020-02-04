// Copyright of Honeycomb Studio

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "HiveWarGameState.generated.h"


UENUM()
enum class EGamePhase : uint8
{
	GP_PREGAME,
	GP_MIDGAME,
	GP_ENDGAME
};

/**
 * Provide the game state for Hive War mode
 */
UCLASS()
class HIVE_API AHiveWarGameState : public AGameState
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(Replicated)
		EGamePhase Phase = EGamePhase::GP_PREGAME;


	UPROPERTY(Replicated)
		float PreGameWaitTime = 3.0f;

public:
	AHiveWarGameState();
	virtual void Tick(float DeltaTime) override;
	virtual void PreGameTick(float DeltaTime);


#pragma region Networking
	/**
	 * Needs to be implemented to initialize replicated properties
	 */
	void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;
#pragma endregion
};
