// Copyright of Honeycomb Studio

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MonsterControl.generated.h"

class AMonsterBase;
/**
 * 
 */
UCLASS()
class HIVE_API AMonsterControl : public APlayerController
{
	GENERATED_BODY()

protected:
	UPROPERTY(Replicated, BlueprintReadOnly)
		TSubclassOf<AMonsterBase> SelectedMonster = NULL;

	bool SpawnCountdown = false;
	float SpawnTimer = 0.0f;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
#pragma region CharacterSelect
	UFUNCTION(Reliable, Server, WithValidation)
		void UpdateSelectedMonster(TSubclassOf<AMonsterBase> InNewMonster);
#pragma endregion


#pragma region Networking
	/**
	 * Needs to be implemented to initialize replicated properties
	 */
	void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;
#pragma endregion
};
