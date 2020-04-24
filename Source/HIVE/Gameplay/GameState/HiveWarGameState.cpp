// Copyright of Honeycomb Studio


#include "HiveWarGameState.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "TimerManager.h"
#include "Engine/Engine.h"
#include "Net/UnrealNetwork.h"

AHiveWarGameState::AHiveWarGameState()
{
	//PrimaryActorTick.bCanEverTick = true;
}

AHiveWarGameState::~AHiveWarGameState()
{
	GreenTeam.Empty();
	RedTeam.Empty();
}

void AHiveWarGameState::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
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

bool AHiveWarGameState::SetToTeam(AMonsterPlayerState* InPlayerState, ETeamEnum InTeam)
{
	if (InTeam == ETeamEnum::TE_GREEN)
	{
		GreenTeam.Add(InPlayerState);
	}
	else if (InTeam == ETeamEnum::TE_RED)
	{
		RedTeam.Add(InPlayerState);
	}
	else
	{
		return false;
	}

	return true;
}

float AHiveWarGameState::GetRemainingPreparationTime()
{
	if (!PreparationTimerHandle.IsValid()) { return -1.0f; }

	return UKismetSystemLibrary::K2_GetTimerRemainingTimeHandle(this, PreparationTimerHandle);
}

void AHiveWarGameState::SetPreparationTimer(float InTime)
{
	GetWorldTimerManager().SetTimer(PreparationTimerHandle, this, &AHiveWarGameState::PrintTeam, InTime);
}

void AHiveWarGameState::PrintTeam()
{
	for (AMonsterPlayerState* green : GreenTeam)
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Green, TEXT("Green Team Member"));
	}

	for (AMonsterPlayerState* red : RedTeam)
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, TEXT("Red Team Member"));
	}
}


void AHiveWarGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AHiveWarGameState, WinningTeam);
	DOREPLIFETIME(AHiveWarGameState, GreenTeam);
	DOREPLIFETIME(AHiveWarGameState, RedTeam);
	DOREPLIFETIME(AHiveWarGameState, PreparationTimerHandle);
}
