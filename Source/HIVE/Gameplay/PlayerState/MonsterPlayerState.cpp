// Copyright of Honeycomb Studio


#include "MonsterPlayerState.h"
#include "Net/UnrealNetwork.h"

void AMonsterPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMonsterPlayerState, TeamID);
}

bool AMonsterPlayerState::AssignTeam(ETeamEnum InTeamID)
{
	TeamID = InTeamID;
	return true;
}
