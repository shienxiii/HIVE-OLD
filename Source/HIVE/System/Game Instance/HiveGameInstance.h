// Copyright of Honeycomb Studio

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "HiveGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class HIVE_API UHiveGameInstance : public UGameInstance
{
	GENERATED_BODY()
	

public:
	/*UHiveGameInstance(const FObjectInitializer& ObjectInitializer);
	virtual void Init() override;*/

#pragma region Multiplayer
	UFUNCTION(Exec)
		void Host();

	UFUNCTION(Exec)
		void Join(const FString& InAddress);
#pragma endregion
};
