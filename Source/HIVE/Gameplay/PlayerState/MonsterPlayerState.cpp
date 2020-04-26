// Copyright of Honeycomb Studio


#include "MonsterPlayerState.h"
#include "GameFramework/PlayerStart.h"
#include "Net/UnrealNetwork.h"
#include "HIVE/Interfaces/TeamInterface.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Engine/Engine.h"


void AMonsterPlayerState::SetSpawnTimer()
{
	GetWorldTimerManager().SetTimer(SpawnTimerHandle, this, &AMonsterPlayerState::AuthorizeSpawning, 3.0f);
}

void AMonsterPlayerState::AuthorizeSpawning()
{
	UE_LOG(LogTemp, Warning, TEXT("Spawning Authorized"));
}

float AMonsterPlayerState::GetSpawnTimerRemainingTime()
{
	if (!SpawnTimerHandle.IsValid()) { return -1.0f; }

	return UKismetSystemLibrary::K2_GetTimerRemainingTimeHandle(this, SpawnTimerHandle);
}

void AMonsterPlayerState::SetSelectedMonster(TSubclassOf<AMonsterBase>  InSelectedMonster)
{
	SelectedMonster = InSelectedMonster;
}

void AMonsterPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMonsterPlayerState, TeamID);
	DOREPLIFETIME(AMonsterPlayerState, DefaultSpawnPoint);
	DOREPLIFETIME(AMonsterPlayerState, SelectedMonster);
}

bool AMonsterPlayerState::AssignTeam(ETeamEnum InTeamID)
{
	TeamID = InTeamID;
	return true;
}

bool AMonsterPlayerState::SetDefaultSpawnPoint(APlayerStart* InSpawnPoint)
{
	ITeamInterface* spawnInfo = Cast<ITeamInterface>(InSpawnPoint);
	if (!spawnInfo) { return false; }

	if (spawnInfo->GetTeam() != TeamID) { return false; }

	DefaultSpawnPoint = InSpawnPoint;

	return true;
}
