// Copyright of Honeycomb Studio

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "GM_HiveWar.generated.h"

class AMonsterControl;

/**
 * 
 */
UCLASS()
class HIVE_API AGM_HiveWar : public AGameMode
{
	GENERATED_BODY()
	
public:
	AGM_HiveWar();

	void SpawnMonsterForController(AMonsterControl* InPlayerControl);
};
