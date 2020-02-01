// Copyright of Honeycomb Studio

#pragma once

#include "CoreMinimal.h"
#include "HIVE/Interfaces/TeamInterface.h"
#include "GameFramework/PlayerStart.h"
#include "MonsterSpawnPoint.generated.h"

/**
 * This spawn point is contains information of the team that are allowed to spawn at
 * The TeamID variable is used to determine which team should spawn on each instance of this spawn point
 */
UCLASS()
class HIVE_API AMonsterSpawnPoint : public APlayerStart, public ITeamInterface
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		ETeamEnum TeamID = ETeamEnum::TE_NEUTRAL;
	
	APlayerController* LinkedPlayer = nullptr;

public:
	ETeamEnum GetTeam() { return TeamID; }
	void LinkPlayer(APlayerController* InPlayerController) { LinkedPlayer = InPlayerController; }
};
