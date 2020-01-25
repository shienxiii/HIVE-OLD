// Copyright of Honeycomb Studio


#include "MonsterControl.h"
#include "Net/UnrealNetwork.h"

void AMonsterControl::BeginPlay()
{
	Super::BeginPlay();
}

bool AMonsterControl::UpdateSelectedMonster_Validate(TSubclassOf<AMonsterBase> InNewMonster)
{
	return true;
}

void AMonsterControl::UpdateSelectedMonster_Implementation(TSubclassOf<AMonsterBase> InNewMonster)
{
	SelectedMonster = InNewMonster;
}

void AMonsterControl::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMonsterControl, SelectedMonster);
}
