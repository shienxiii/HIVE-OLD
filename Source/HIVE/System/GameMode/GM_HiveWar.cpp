// Copyright of Honeycomb Studio


#include "GM_HiveWar.h"
#include "HIVE/Gameplay/Monster/MonsterBase.h"
#include "HIVE/Gameplay/Controller/MonsterController.h"
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
	TArray<ETeamEnum> allKeys;
	TeamSpawnPoints.GetKeys(allKeys);

	for (int i = 0; i < allKeys.Num(); i++)
	{
		FTeamSpawnArea* area = TeamSpawnPoints.Find(allKeys[i]);

		FString keyName = UEnum::GetValueAsString(allKeys[i]);
		FString printMessage = keyName;
		printMessage.Append(" have ");
		printMessage.Append(FString::FromInt(area->GetSpawnPoints().Num()));
		printMessage.Append(" spawn points");
		GEngine->AddOnScreenDebugMessage(-1, 150.0f, FColor::Green, printMessage);
	}

}

AGM_HiveWar::AGM_HiveWar()
{
	static ConstructorHelpers::FClassFinder<APawn> tempMonsterBP(TEXT("/Game/Blueprint/TestCharacter/RedGuy.RedGuy_C"));

	if (tempMonsterBP.Class != NULL)
	{
		DefaultPawnClass = tempMonsterBP.Class;
		PlayerControllerClass = AMonsterController::StaticClass();
	}
}

void AGM_HiveWar::SpawnMonsterForController(AMonsterController* InPlayerControl)
{


	FActorSpawnParameters spawnParam = FActorSpawnParameters();
	spawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	//spawnParam.IsRemoteOwned = true;
	TArray<AActor*> start;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStart::StaticClass(), start);
	
	AMonsterBase* myMonster = GetWorld()->SpawnActor<AMonsterBase>((InPlayerControl->GetSelectedMonster()), start[0]->GetActorLocation() , FRotator(), spawnParam);
	InPlayerControl->Possess(myMonster);
}

void AGM_HiveWar::PostLogin(APlayerController* InPlayerController)
{
	// NOTE: When this code runs, player is not yet controlling a monster but just recently joined the game
	Super::PostLogin(InPlayerController);

	// Make sure the player controller being passed implemented ITeamInterface
	ITeamInterface* controller = Cast<ITeamInterface>(InPlayerController);
	check(controller);

	// Go through the spawn points that are not TE_NEUTRAL to decide which team to assign player to
	TArray<ETeamEnum> validKeys;
	TeamSpawnPoints.GetKeys(validKeys);
	validKeys.Remove(ETeamEnum::TE_NEUTRAL);
	validKeys.Sort();

	ETeamEnum teamID = ETeamEnum::TE_NEUTRAL;

	for (int i = 0; i < validKeys.Num(); i++)
	{
		FString keyName = UEnum::GetValueAsString(validKeys[i]);
		FString printMessage = "Checking ";
		printMessage.Append(keyName);
		GEngine->AddOnScreenDebugMessage(-1, 150.0f, FColor::Yellow, printMessage);

		if (teamID == ETeamEnum::TE_NEUTRAL)
		{
			teamID = validKeys[i];
		}
		else
		{
			bool changeToCurrentKey = (TeamSpawnPoints.Find(validKeys[i])->GetMembersList().Num()) < (TeamSpawnPoints.Find(teamID)->GetMembersList().Num());

			teamID = changeToCurrentKey ? validKeys[i] : teamID;
		}
	}

	// teamID is now the ETeamEnum to assign to current controller
	controller->AssignTeam(teamID);

	// might as will assign the default spawn point as well
}


#pragma region TeamSpawnPoint
int8 FTeamSpawnArea::GetFreeSpawnPoint(AController* InController)
{
	return int8();
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

void FTeamSpawnArea::AssignSpawnPointToPlayer(int8 InSpawnPointIndex, AController* InController)
{
}
#pragma endregion