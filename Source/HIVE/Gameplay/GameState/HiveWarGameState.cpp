// Copyright of Honeycomb Studio


#include "HiveWarGameState.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/Engine.h"
#include "Net/UnrealNetwork.h"

AHiveWarGameState::AHiveWarGameState()
{
	//PrimaryActorTick.bCanEverTick = true;
}

void AHiveWarGameState::SetWinningTeam(ETeamEnum InWinningTeam)
{
	WinningTeam = InWinningTeam;

	TArray<AActor*> players;
	UGameplayStatics::GetAllActorsOfClass(this, APlayerController::StaticClass(), players);

	for (int i = 0; i < players.Num(); i++)
	{
		APlayerController* player = Cast<APlayerController>(players[i]);

		if (player->IsLocalPlayerController()) { player->GameHasEnded(); }
		else { GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, TEXT("Skipping")); }
	}
}

void AHiveWarGameState::WinningTeamRepEvent()
{
	// Don't need to run this on the server
	if (GetLocalRole() == ENetRole::ROLE_Authority) { return; }

	TArray<AActor*> players;
	UGameplayStatics::GetAllActorsOfClass(this, APlayerController::StaticClass(), players);

	//TArray<AActor*> players;
	//UGameplayStatics::GetAllActorsOfClass(this, APlayerController::StaticClass(), players);

	for (int i = 0; i < players.Num(); i++)
	{
		APlayerController* player = Cast<APlayerController>(players[i]);
		player->GameHasEnded();
	}

	GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Yellow, TEXT("GameState WinningTeam Replicated"));
}


void AHiveWarGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AHiveWarGameState, WinningTeam);
	//DOREPLIFETIME(AHiveWarGameState, PreGameWaitTime);
}
