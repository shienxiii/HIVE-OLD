// Copyright of Honeycomb Studio

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerStart.h"
#include "MonsterSpawnPoint.generated.h"

/**
 * This spawn point is contains information of the team that are allowed to spawn at
 * The TeamIndex variable is used to determine which team should spawn on each instance of this spawn point
 */
UCLASS()
class HIVE_API AMonsterSpawnPoint : public APlayerStart
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		uint8 TeamIndex = 0;
	
	APlayerController* LinkedPlayer = nullptr;

public:
	uint8 GetTeamIndex() { return TeamIndex; }
	void LinkPlayer(APlayerController* InPlayerController) { LinkedPlayer = InPlayerController; }
};
