// Copyright of Honeycomb Studio


#include "MonsterPlayerState.h"
#include "GameFramework/PlayerStart.h"
#include "Net/UnrealNetwork.h"
#include "HIVE/Interfaces/TeamInterface.h"

void AMonsterPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMonsterPlayerState, TeamID);
	DOREPLIFETIME(AMonsterPlayerState, DefaultSpawnPoint);
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
