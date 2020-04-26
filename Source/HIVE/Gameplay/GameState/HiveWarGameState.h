// Copyright of Honeycomb Studio

#pragma once

#include "CoreMinimal.h"
#include "HIVE/Interfaces/TeamInterface.h"
#include "GameFramework/GameState.h"
#include "HiveWarGameState.generated.h"

class AMonsterPlayerState;


/**
 * Provide the game state for Hive War mode
 */
UCLASS()
class HIVE_API AHiveWarGameState : public AGameState
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(Replicated)
		TArray<AMonsterPlayerState*> GreenTeam;

	UPROPERTY(Replicated)
		TArray<AMonsterPlayerState*> RedTeam;

	UPROPERTY(ReplicatedUsing = WinningTeamRepEvent)
		ETeamEnum WinningTeam = ETeamEnum::TE_INVALID;

public:
	AHiveWarGameState();
	~AHiveWarGameState();

	virtual void Tick(float DeltaTime) override;
	void SetWinningTeam(ETeamEnum InWinningTeam);

	UFUNCTION()
		void WinningTeamRepEvent();

	TArray<AMonsterPlayerState*> GetGreenTeam() { return GreenTeam; }
	TArray<AMonsterPlayerState*> GetRedTeam() { return RedTeam; }
	bool SetToTeam(AMonsterPlayerState* InPlayerState, ETeamEnum InTeam);

	void PrintTeam();

#pragma region Networking
	/**
	 * Needs to be implemented to initialize replicated properties
	 */
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
#pragma endregion
};
