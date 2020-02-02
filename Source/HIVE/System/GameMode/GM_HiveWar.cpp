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
	Super::PostLogin(InPlayerController);

	// Separate player to team
	AMonsterController* inControl = Cast<AMonsterController>(InPlayerController);

	if (!inControl) { return; }

	//inControl->AssignTeam(3);
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