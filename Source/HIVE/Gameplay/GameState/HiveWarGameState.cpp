// Copyright of Honeycomb Studio


#include "HiveWarGameState.h"
#include "HIVE/System/GameMode/GM_HiveWar.h"
#include "Net/UnrealNetwork.h"

AHiveWarGameState::AHiveWarGameState()
{
	//PrimaryActorTick.bCanEverTick = true;
}


void AHiveWarGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	//DOREPLIFETIME(AHiveWarGameState, Phase);
	//DOREPLIFETIME(AHiveWarGameState, PreGameWaitTime);
}
