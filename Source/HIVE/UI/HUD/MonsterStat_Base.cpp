// Copyright of Honeycomb Studio


#include "MonsterStat_Base.h"
#include "HIVE/Gameplay/Monster/MonsterBase.h"
#include "Components/ProgressBar.h"

bool UMonsterStat_Base::Initialize()
{
	Super::Initialize();
	if (HP)
	{
		HP->PercentDelegate.BindDynamic(this, &UMonsterStat_Base::HP_Ratio);
	}
	return true;
}

float UMonsterStat_Base::HP_Ratio()
{
	if (!Monster)
	{
		return 0.0f;
	}

	return Monster->GetHealthPercentRatio();
}
