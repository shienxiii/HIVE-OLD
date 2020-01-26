// Copyright of Honeycomb Studio


#include "MonsterControl.h"
#include "Net/UnrealNetwork.h"
#include "HIVE/System/GameMode/GM_HiveWar.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"

void AMonsterControl::BeginPlay()
{
	Super::BeginPlay();
}

bool AMonsterControl::UpdateSelectedMonster_Validate(TSubclassOf<AMonsterBase> InNewMonster)
{
	return true;
}

void AMonsterControl::UpdateSelectedMonster_Implementation(TSubclassOf<AMonsterBase> InNewMonster)
{
	SelectedMonster = InNewMonster;
	SpawnSelectedMonster();
}

bool AMonsterControl::SpawnSelectedMonster_Validate()
{
	return true;
}

void AMonsterControl::SpawnSelectedMonster_Implementation()
{
	// This function should only be run on the server
	if (GetLocalRole() < ENetRole::ROLE_Authority)
	{
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("Spawning"));
	AGM_HiveWar* gameMode = Cast<AGM_HiveWar>(UGameplayStatics::GetGameMode(GetWorld()));

	if (!gameMode)
	{
		UE_LOG(LogTemp, Warning, TEXT("No suitable game mode found"));
		return;
	}

	gameMode->SpawnMonsterForController(this);
	UE_LOG(LogTemp, Warning, TEXT("Spawn Complete"));
}

void AMonsterControl::SpawnCompleteTest()
{
	FInputModeGameOnly gameInput;
	SetInputMode(gameInput);
}

void AMonsterControl::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMonsterControl, SelectedMonster);
}
