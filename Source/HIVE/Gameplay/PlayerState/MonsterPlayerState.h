// Copyright of Honeycomb Studio

#pragma once

#include "CoreMinimal.h"
#include "HIVE/Interfaces/TeamInterface.h"
#include "GameFramework/PlayerState.h"
#include "MonsterPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class HIVE_API AMonsterPlayerState : public APlayerState, public ITeamInterface
{
	GENERATED_BODY()

protected:
	UPROPERTY(Replicated, BlueprintReadOnly)
		ETeamEnum TeamID = ETeamEnum::TE_NEUTRAL;

public:
	
#pragma region Networking
	/**
	 * Needs to be implemented to initialize replicated properties
	 */
	void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;
#pragma endregion

#pragma region TeamInterface
	virtual bool AssignTeam(ETeamEnum InTeamID) override;

	virtual ETeamEnum GetTeam() override { return TeamID; }
#pragma endregion

};