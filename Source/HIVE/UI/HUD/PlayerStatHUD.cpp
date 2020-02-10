// Copyright of Honeycomb Studio


#include "PlayerStatHUD.h"
#include "HIVE/Gameplay/Monster/MonsterBase.h"
#include "HIVE/UI/HUD/MonsterStat_Base.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"


bool UPlayerStatHUD::Initialize()
{
	Super::Initialize();

	return true;
}
void UPlayerStatHUD::BindMonster(AMonsterBase* InMonster)
{
	Monster = InMonster;
	MonsterStat->BindMonster(InMonster);
}

void UPlayerStatHUD::UnbindMonster()
{
	Monster = nullptr;
	MonsterStat->UnbindMonster();
}
