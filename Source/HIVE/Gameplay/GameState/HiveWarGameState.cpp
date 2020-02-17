// Copyright of Honeycomb Studio


#include "HiveWarGameState.h"
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
