// Copyright of Honeycomb Studio


#include "GM_HiveWar.h"
#include "HIVE/Gameplay/Monster/MonsterBase.h"
#include "HIVE/Gameplay/Monster/MonsterSpawnPoint.h"
#include "HIVE/Gameplay/Controller/MonsterController.h"
#include "HIVE/Gameplay/PlayerState/MonsterPlayerState.h"
#include "HIVE/Gameplay/GameState/HiveWarGameState.h"
#include "HIVE/Interfaces/TeamInterface.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "GameFramework/PlayerStart.h"
#include "Engine/Engine.h"
#include "Kismet/KismetStringLibrary.h"


void AGM_HiveWar::BeginPlay()
{
	Super::BeginPlay();

	TeamSpawnPoints = TMap<ETeamEnum, FTeamSpawnArea>();

	// Find all spawn points
	TArray<AActor*> spawnPoints;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AMonsterSpawnPoint::StaticClass(), spawnPoints);


	// Go through all the spawn points found and allocate them to their respective team in the TMap TeamSpawnPoint
	for (int i = 0; i < spawnPoints.Num(); i++)
	{
		AMonsterSpawnPoint* spawn = Cast<AMonsterSpawnPoint>(spawnPoints[i]);

		// Find the team in the TMap
		FTeamSpawnArea* team = TeamSpawnPoints.Find(spawn->GetTeam());

		if (!team)
		{
			// If the team is not found, create it and assign the newly created team to the team pointer
			TeamSpawnPoints.Add(spawn->GetTeam(), FTeamSpawnArea(spawn->GetTeam()));
			team = TeamSpawnPoints.Find(spawn->GetTeam());
		}

		// Add the spawn point to the relevant team
		team->AddSpawnPoint(spawn);
	}
}

AGM_HiveWar::AGM_HiveWar()
{
	// Set all default default classes to the class that is required for this game mode to run all its tasks
	DefaultPawnClass = NULL;
	PlayerControllerClass = AMonsterController::StaticClass();
	PlayerStateClass = AMonsterPlayerState::StaticClass();
	GameStateClass = AHiveWarGameState::StaticClass();
}

void AGM_HiveWar::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AGM_HiveWar::SpawnMonsterForController(AMonsterController* InPlayerControl)
{
	// Needs update the most
	FActorSpawnParameters spawnParam = FActorSpawnParameters();
	spawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	AMonsterPlayerState* state = InPlayerControl->GetPlayerState<AMonsterPlayerState>();

	if (!state)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Failed to get state"));
		return;
	}

	if (!state->GetDefaultSpawnPoint())
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Failed to get spawn point"));
		return;
	}


	AMonsterBase* myMonster = GetWorld()->SpawnActor<AMonsterBase>((InPlayerControl->GetSelectedMonster()), state->GetDefaultSpawnPoint()->GetActorLocation(), state->GetDefaultSpawnPoint()->GetActorRotation(), spawnParam);
	InPlayerControl->Possess(myMonster);
}

void AGM_HiveWar::PostLogin(APlayerController* InPlayerController)
{
	Super::PostLogin(InPlayerController);
}

void AGM_HiveWar::BeginTeamAllocation()
{
	bCanSpawnPlayerCharacter = true;
	TArray<APlayerState*> currentPlayers = GetGameState<AHiveWarGameState>()->PlayerArray;

	for (int i = 0; i < currentPlayers.Num(); i++)
	{
		if (AllocateToTeam(currentPlayers[i]) != ETeamEnum::TE_INVALID)
		{
			GEngine->AddOnScreenDebugMessage(-1, 150.0f, FColor::Green, "Successful allocation");
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 150.0f, FColor::Green, "Invalid allocation");
		}
	}
}

ETeamEnum AGM_HiveWar::AllocateToTeam(APlayerState* InPlayerState)
{
	// Make sure the PlayerState being passed implemented ITeamInterface
	ITeamInterface* player = Cast<ITeamInterface>(InPlayerState);
	check(player);

	// Go through the spawn points that are not TE_NEUTRAL or TE_INVALID to decide which team to assign player to
	TArray<ETeamEnum> validKeys;
	TeamSpawnPoints.GetKeys(validKeys);
	validKeys.Remove(ETeamEnum::TE_NEUTRAL);
	validKeys.Remove(ETeamEnum::TE_INVALID);
	validKeys.Sort();

	// This local variable value will change to reflect which team the InController will be allocated to
	ETeamEnum teamID = ETeamEnum::TE_INVALID;

	// Go through all key that are not TE_NEUTRAL or TE_INVALID
	for (int i = 0; i < validKeys.Num(); i++)
	{
		// Check if there are space available for the controller to join the team
		bool spaceAvailable = (TeamSpawnPoints.Find(validKeys[i])->AvailableSpawnPoints()) > 0;

		// Make sure spaceAvailable is the condition of all checks that changes the teamID local variable to the current key
		if (spaceAvailable && teamID == ETeamEnum::TE_INVALID)
		{
			teamID = validKeys[i];
		}
		else if(spaceAvailable)
		{
			bool changeToCurrentKey = (TeamSpawnPoints.Find(validKeys[i])->GetMembersList().Num()) < (TeamSpawnPoints.Find(teamID)->GetMembersList().Num());
			

			teamID = changeToCurrentKey ? validKeys[i] : teamID;
		}
	}


	if (teamID == ETeamEnum::TE_INVALID)
	{
		// no point continuing
		return teamID;
	}

	FTeamSpawnArea* area = TeamSpawnPoints.Find(teamID);
	
	if (area->AddToTeam(InPlayerState))
	{
		return teamID;
	}

	return ETeamEnum::TE_INVALID;
}

void AGM_HiveWar::StartGame()
{
	if (GetLocalRole() == ENetRole::ROLE_Authority)
	{
		BeginTeamAllocation();
		Phase = EGamePhase::GP_MIDGAME;
	}
}


#pragma region TeamSpawnArea
int32 FTeamSpawnArea::AvailableSpawnPoints()
{
	return SpawnPoints.Num() - Members.Num();
}

bool FTeamSpawnArea::AddSpawnPoint(AMonsterSpawnPoint* InNewSpawnPoint)
{
	if (SpawnPoints.Find(InNewSpawnPoint) != INDEX_NONE)
	{
		// if trying to add existing spawn point, do nothing and return false
		return false;
	}

	SpawnPoints.Add(InNewSpawnPoint);

	return true;
}

bool FTeamSpawnArea::AddToTeam(APlayerState* InPlayerState)
{
	ITeamInterface* state = Cast<ITeamInterface>(InPlayerState);

	if (!state || AvailableSpawnPoints() <= 0)
	{
		return false;
	}

	state->AssignTeam(TeamID);
	Members.Add(InPlayerState);
	
	return AssignSpawnPointToPlayer(InPlayerState);
}

bool FTeamSpawnArea::AssignSpawnPointToPlayer(APlayerState* InPlayerState)
{
	ITeamInterface* state = Cast<ITeamInterface>(InPlayerState);

	// Find free spawn point
	for (int i = 0; i < SpawnPoints.Num(); i++)
	{
		if (!(SpawnPoints[i]->GetLinkedPlayer()))
		{
			SpawnPoints[i]->LinkPlayer(InPlayerState);
			state->SetDefaultSpawnPoint(SpawnPoints[i]);
			return true;
		}
	}

	return false;
}
#pragma endregion