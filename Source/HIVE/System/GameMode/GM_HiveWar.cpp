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

	// Get all the keys
	/*TArray<ETeamEnum> allKeys;
	TeamSpawnPoints.GetKeys(allKeys);

	for (int i = 0; i < allKeys.Num(); i++)
	{
		FTeamSpawnArea* area = TeamSpawnPoints.Find(allKeys[i]);
		area->SortSpawnPoints();
		FString keyName = UEnum::GetValueAsString(allKeys[i]);
		FString printMessage = keyName;
		printMessage.Append(" have ");
		printMessage.Append(FString::FromInt(area->GetSpawnPoints().Num()));
		printMessage.Append(" spawn points");
		GEngine->AddOnScreenDebugMessage(-1, 150.0f, FColor::Green, printMessage);
	}*/

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

	//switch (Phase)
	//{
	//case EGamePhase::GP_PREGAME:
	//	PreGameTick(DeltaTime);
	//	break;
	//case EGamePhase::GP_MIDGAME:
	//	/*GEngine->AddOnScreenDebugMessage(-1, 150.0f, FColor::Yellow, "GM MidGameTick");
	//	SetActorTickEnabled(false);*/
	//	break;
	//case EGamePhase::GP_ENDGAME:
	//	break;
	//default:
	//	break;
	//}
}

void AGM_HiveWar::PreGameTick(float DeltaTime)
{
	/*GEngine->AddOnScreenDebugMessage(-1, 150.0f, FColor::Yellow, "GM PreGameTick");*/
	if (GetLocalRole() != ENetRole::ROLE_Authority)
	{
		return;
	}

	PreGameWaitTime -= DeltaTime;

	// DELETE ON FINAL GAME
	TArray<AActor*> Players;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AMonsterController::StaticClass(), Players);
	for (int i = 0; i < Players.Num(); i++)
	{
		Cast<AMonsterController>(Players[i])->UpdateCountdownTimer(PreGameWaitTime);
	}



	// Begin allocating once pre-game phase ended
	if (PreGameWaitTime <= 0.0f)
	{
		BeginTeamAllocation();
		Phase = EGamePhase::GP_MIDGAME;
	}
}

void AGM_HiveWar::SpawnMonsterForController(AMonsterController* InPlayerControl)
{
	FActorSpawnParameters spawnParam = FActorSpawnParameters();
	spawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	TArray<AActor*> start;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStart::StaticClass(), start);
	
	AMonsterBase* myMonster = GetWorld()->SpawnActor<AMonsterBase>((InPlayerControl->GetSelectedMonster()), start[0]->GetActorLocation() , start[0]->GetActorRotation(), spawnParam);
	InPlayerControl->Possess(myMonster);
}

void AGM_HiveWar::PostLogin(APlayerController* InPlayerController)
{
	Super::PostLogin(InPlayerController);
}

void AGM_HiveWar::BeginTeamAllocation()
{
	//GEngine->AddOnScreenDebugMessage(-1, 150.0f, FColor::Yellow, "BeginTeamAllocation");
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
		/*if (spaceAvailable)
		{
			FString printMessage = "Space available on ";
			printMessage.Append(UEnum::GetValueAsString(validKeys[i]));
			GEngine->AddOnScreenDebugMessage(-1, 150.0f, FColor::Yellow, printMessage);
		}*/

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
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 150.0f, FColor::Yellow, TEXT("Not Server"));
	}
}


#pragma region TeamSpawnPoint
int32 FTeamSpawnArea::GetFreeSpawnPoint(AController* InController)
{
	return int32();
}

int32 FTeamSpawnArea::AvailableSpawnPoints()
{
	/*FString printMessage = UEnum::GetValueAsString(TeamID);
	printMessage.Append(" has ");
	printMessage.Append(FString::FromInt(SpawnPoints.Num() - Members.Num()));
	GEngine->AddOnScreenDebugMessage(-1, 150.0f, FColor::Yellow, printMessage);*/
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
	ITeamInterface* controller = Cast<ITeamInterface>(InPlayerState);

	if (!controller || AvailableSpawnPoints() <= 0)
	{
		return false;
	}

	controller->AssignTeam(TeamID);
	Members.Add(InPlayerState);
	return true;
}

void FTeamSpawnArea::AssignSpawnPointToPlayer(int8 InSpawnPointIndex, AController* InController)
{
}
#pragma endregion