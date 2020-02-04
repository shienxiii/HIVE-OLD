// Copyright of Honeycomb Studio


#include "HiveWarGameState.h"
#include "HIVE/System/GameMode/GM_HiveWar.h"
#include "Net/UnrealNetwork.h"

#include "Engine/Engine.h"

AHiveWarGameState::AHiveWarGameState()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AHiveWarGameState::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (GetLocalRole() != ENetRole::ROLE_Authority)
	{
		return;
	}

	switch (Phase)
	{
		case EGamePhase::GP_PREGAME:
			PreGameTick(DeltaTime);
			break;
		case EGamePhase::GP_MIDGAME:
			GEngine->AddOnScreenDebugMessage(-1, 150.0f, FColor::Yellow, "MidGameTick");
			SetActorTickEnabled(false);
			break;
		case EGamePhase::GP_ENDGAME:
			break;
		default:
			break;
	}
}


void AHiveWarGameState::PreGameTick(float DeltaTime)
{
	GEngine->AddOnScreenDebugMessage(-1, 150.0f, FColor::Yellow, "PreGameTick");
	if (GetLocalRole() != ENetRole::ROLE_Authority)
	{
		return;
	}

	PreGameWaitTime -= DeltaTime;

	if (PreGameWaitTime <= 0.0f)
	{
		AGM_HiveWar* gameMode = Cast<AGM_HiveWar>(AuthorityGameMode);
		check(gameMode);

		gameMode->BeginTeamAllocation();
		Phase = EGamePhase::GP_MIDGAME;
	}
}

void AHiveWarGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AHiveWarGameState, Phase);
	DOREPLIFETIME(AHiveWarGameState, PreGameWaitTime);
}
