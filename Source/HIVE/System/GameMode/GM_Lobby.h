// Copyright of Honeycomb Studio

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "GM_Lobby.generated.h"

class UHiveGameInstance;

/**
 * Game mode to be used for the Lobby map
 */
UCLASS()
class HIVE_API AGM_Lobby : public AGameMode
{
	GENERATED_BODY()

protected:
	UHiveGameInstance* GameInstance;

public:
	AGM_Lobby();

	void BeginPlay() override;
};
