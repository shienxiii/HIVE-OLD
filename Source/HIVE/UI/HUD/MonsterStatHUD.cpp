// Copyright of Honeycomb Studio


#include "MonsterStatHUD.h"
#include "HIVE/Gameplay/Monster/MonsterBase.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"


bool UMonsterStatHUD::Initialize()
{
	Super::Initialize();

	HP->PercentDelegate.BindDynamic(this, &UMonsterStatHUD::HP_Ratio);
	return true;
}
void UMonsterStatHUD::BindMonster(AMonsterBase* InMonster)
{
	Monster = InMonster;
}

void UMonsterStatHUD::UnbindMonster()
{
	Monster = nullptr;
}

float UMonsterStatHUD::HP_Ratio()
{
	if (!Monster)
	{
		return 0.0f;
	}

	return Monster->GetHealthPercentRatio();
}
