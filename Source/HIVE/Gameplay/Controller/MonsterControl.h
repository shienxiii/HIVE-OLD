// Copyright of Honeycomb Studio

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MonsterControl.generated.h"

/**
 * 
 */
UCLASS()
class HIVE_API AMonsterControl : public APlayerController
{
	GENERATED_BODY()

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
