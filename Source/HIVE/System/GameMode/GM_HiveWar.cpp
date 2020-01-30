// Copyright of Honeycomb Studio


#include "GM_HiveWar.h"
#include "HIVE/Gameplay/Monster/MonsterBase.h"
#include "HIVE/Gameplay/Monster/MonsterSpawnPoint.h"
#include "HIVE/Gameplay/Controller/MonsterControl.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "GameFramework/PlayerStart.h"
#include "Engine/Engine.h"

void AGM_HiveWar::BeginPlay()
{
	Super::BeginPlay();

	TeamSpawnPoints = TMap<uint8, TArray<AMonsterSpawnPoint*>>();

	// Find all spawn points
	TArray<AActor*> spawnPoints;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AMonsterSpawnPoint::StaticClass(), spawnPoints);

	// Allocate all spawn points to team
	for (int i = 0; i < spawnPoints.Num(); i++)
	{
		AMonsterSpawnPoint* spawn = Cast<AMonsterSpawnPoint>(spawnPoints[i]);
		check(spawn);

		// Get a reference to the Map that contains the team index
		TArray<AMonsterSpawnPoint*>* teamFound = TeamSpawnPoints.Find(spawn->GetTeamIndex());

		if (teamFound)
		{
			teamFound->Add(spawn);
		}
		else
		{
			TeamSpawnPoints.Add(spawn->GetTeamIndex(), TArray<AMonsterSpawnPoint*>());

			TeamSpawnPoints.Find(spawn->GetTeamIndex())
				->Add(spawn);
		}
	}

	// Test Run
	TArray<uint8> keys;
	TeamSpawnPoints.GetKeys(keys);
	for (int i = 0; i < keys.Num(); i++)
	{
		TArray<AMonsterSpawnPoint*>* spwn = TeamSpawnPoints.Find(keys[i]);

		for (int j = 0; j < spwn->Num(); j++)
		{
			FString name = FString("Team Index ");
			name.Append(FString::FromInt((*spwn)[j]->GetTeamIndex()));
			name.Append(" Label ");
			name.Append((*spwn)[j]->GetName());
			GEngine->AddOnScreenDebugMessage(-1, 120.0f, FColor::Cyan, name);
		}
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

void AGM_HiveWar::SpawnMonsterForController(AMonsterControl* InPlayerControl)
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

	AMonsterControl* inControl = Cast<AMonsterControl>(InPlayerController);

	if (!inControl) { return; }

	inControl->AssignTeam(0);
}
