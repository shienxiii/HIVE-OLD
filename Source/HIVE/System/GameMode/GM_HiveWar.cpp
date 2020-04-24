// Copyright of Honeycomb Studio


#include "GM_HiveWar.h"
#include "HIVE/Gameplay/Monster/MonsterBase.h"
#include "HIVE/Gameplay/Monster/HiveBase.h"
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

	TeamSpawnMap = TMap<ETeamEnum, FTeamSpawnArea>();

	// Find all spawn points
	TArray<AActor*> spawnPoints;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AMonsterSpawnPoint::StaticClass(), spawnPoints);


	// Go through all the spawn points found and allocate them to their respective team in the TMap TeamSpawnPoint
	for (int i = 0; i < spawnPoints.Num(); i++)
	{
		AMonsterSpawnPoint* spawn = Cast<AMonsterSpawnPoint>(spawnPoints[i]);

		// Find the team in the TMap
		FTeamSpawnArea* team = TeamSpawnMap.Find(spawn->GetTeam());

		if (!team)
		{
			// If the team is not found, create it and assign the newly created team to the team pointer
			TeamSpawnMap.Add(spawn->GetTeam(), FTeamSpawnArea(spawn->GetTeam()));
			team = TeamSpawnMap.Find(spawn->GetTeam());
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



void AGM_HiveWar::PostLogin(APlayerController* InPlayerController)
{
	Super::PostLogin(InPlayerController);

	AMonsterController* controller = Cast<AMonsterController>(InPlayerController);
	check(controller);

	controller->SetupPlayerHUD();
	PlayerList.Add(controller);

	if (PlayerList.Num() < MatchPlayerCount)
	{
		// Not enough players in the game yet, show the waiting screen
		controller->LoadWaitScreen();
	}
	else if (PlayerList.Num() == MatchPlayerCount)
	{
		BeginTeamAllocation();

		for (AMonsterController* control : PlayerList)
		{
			control->LoadCharacterSelectScreen();
		}
	}
	else
	{
		check(false); // force a crash if there are more players than allowed
	}
}

void AGM_HiveWar::Logout(AController* ExitingPlayer)
{
	// Assume this will only happen while waiting
	AMonsterController* player = Cast<AMonsterController>(ExitingPlayer);

	if (player)
	{
		PlayerList.Remove(player);
	}
}

void AGM_HiveWar::BeginTeamAllocation()
{

	for (AMonsterController* player : PlayerList)
	{
		AMonsterPlayerState* state = player->GetPlayerState<AMonsterPlayerState>();
		check(state);

		ETeamEnum allocatedTeam = AllocateToTeam(state);

		if (allocatedTeam == ETeamEnum::TE_INVALID)
		{
			GEngine->AddOnScreenDebugMessage(-1, 150.0f, FColor::Red, "Invalid allocation");
		}
		else
		{
			GetHiveWarGameState()->SetToTeam(state, allocatedTeam);
		}

	}

	GetHiveWarGameState()->SetPreparationTimer();
}

ETeamEnum AGM_HiveWar::AllocateToTeam(APlayerState* InPlayerState)
{
	// Make sure the PlayerState being passed implemented ITeamInterface
	ITeamInterface* player = Cast<ITeamInterface>(InPlayerState);
	check(player);

	// Go through the spawn points that are not TE_NEUTRAL or TE_INVALID to decide which team to assign player to
	TArray<ETeamEnum> validKeys;
	TeamSpawnMap.GetKeys(validKeys);
	validKeys.Remove(ETeamEnum::TE_NEUTRAL);
	validKeys.Remove(ETeamEnum::TE_INVALID);
	validKeys.Sort();

	// This local variable value will change to reflect which team the InPlayerState will be allocated to
	ETeamEnum teamID = ETeamEnum::TE_INVALID;

	for (ETeamEnum key : validKeys)
	{
		bool spaceAvailable = (TeamSpawnMap.Find(key))->AvailableSpawnPoints() > 0;

		if (spaceAvailable && teamID == ETeamEnum::TE_INVALID)
		{
			teamID = key;
		}
		else if(spaceAvailable)
		{
			bool swapTeamID = TeamSpawnMap.Find(key)->GetMemberCount() < TeamSpawnMap.Find(teamID)->GetMemberCount();

			teamID = swapTeamID ? key : teamID;
		}
	}

	if (teamID == ETeamEnum::TE_INVALID)
	{
		// no point continuing
		return teamID;
	}

	FTeamSpawnArea* area = TeamSpawnMap.Find(teamID);
	
	if (area->AddToTeam(InPlayerState))
	{
		return teamID;
	}

	return ETeamEnum::TE_INVALID;
}


void AGM_HiveWar::SpawnMonsterForController(AMonsterController* InPlayerControl)
{
	// Needs update the most
	FActorSpawnParameters spawnParam = FActorSpawnParameters();
	spawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	AMonsterPlayerState* state = InPlayerControl->GetPlayerState<AMonsterPlayerState>();
	check(state);

	AMonsterBase* myMonster = GetWorld()->SpawnActor<AMonsterBase>((InPlayerControl->GetSelectedMonster()), state->GetDefaultSpawnPoint()->GetActorLocation(), state->GetDefaultSpawnPoint()->GetActorRotation(), spawnParam);
	InPlayerControl->Possess(myMonster);
}


void AGM_HiveWar::StartGame()
{
	if (GetLocalRole() == ENetRole::ROLE_Authority)
	{
		BeginTeamAllocation();
	}
}

void AGM_HiveWar::GameOver(AActor* InDeadHive)
{
	ITeamInterface* deadHiveInterface = Cast<ITeamInterface>(InDeadHive);
	check(deadHiveInterface);

	ETeamEnum losingTeam = deadHiveInterface->GetTeam();
	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, TEXT("GameOver"));

	// Find living hive
	TArray<AActor*> hives;
	UGameplayStatics::GetAllActorsOfClass(this, AHiveBase::StaticClass(), hives);
	check(hives.Num() == 2);

	AHiveBase* winningHive = nullptr;

	for (int i = 0; i < hives.Num(); i++)
	{
		if (hives[i] != InDeadHive)
		{
			winningHive = Cast<AHiveBase>(hives[i]);
			break;
		}
	}

	AHiveWarGameState* gameState = GetGameState<AHiveWarGameState>();
	check(gameState && winningHive);


	gameState->SetWinningTeam(winningHive->GetTeam());
	//TArray<AActor*> players;
	//UGameplayStatics::GetAllActorsOfClass(this, APlayerController::StaticClass(), players);

	//for (int i = 0; i < players.Num(); i++)
	//{
	//	APlayerController* player = Cast<APlayerController>(players[i]);
	//	player->GameHasEnded(InDeadHive);
	//}
	////InDeadHive->Destroy();
}

AHiveWarGameState* AGM_HiveWar::GetHiveWarGameState()
{
	return GetGameState<AHiveWarGameState>();
}


#pragma region TeamSpawnArea
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
	// Access the ITeamInterface of the player state
	ITeamInterface* teamInterface = Cast<ITeamInterface>(InPlayerState);

	if (!teamInterface || AvailableSpawnPoints() <= 0)
	{
		return false;
	}

	// Update the player state team interface
	teamInterface->AssignTeam(TeamID);
	teamInterface->SetDefaultSpawnPoint(SpawnPoints[Members.Num()]);

	// Add the members to the struct
	Members.Add(InPlayerState);
	
	return true;
}
#pragma endregion