// Copyright of Honeycomb Studio

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "HiveSaveGame.generated.h"


USTRUCT()
struct FPlayerProfile
{
	GENERATED_BODY()

	FName PlayerName;
	FTexture* PlayerImage;
	//UTexture PlayerImage;

	uint32 PlayerPoint;
	uint32 Gene;

public:
	FPlayerProfile()
	{
		PlayerName = TEXT("Player 1");
		PlayerImage = nullptr;

		PlayerPoint = 0;
		Gene = 0;
	}
};


/**
 * 
 */
UCLASS()
class HIVE_API UHiveSaveGame : public USaveGame
{
	GENERATED_BODY()
	
};
