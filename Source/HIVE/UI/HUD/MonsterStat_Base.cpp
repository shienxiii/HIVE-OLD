// Copyright of Honeycomb Studio


#include "MonsterStat_Base.h"
#include "HIVE/Gameplay/Monster/MonsterBase.h"
#include "Components/ProgressBar.h"

void UMonsterStat_Base::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (HP)
	{
		HP->PercentDelegate.BindDynamic(this, &UMonsterStat_Base::HP_Ratio);
	}
}

float UMonsterStat_Base::HP_Ratio()
{
	if (!OwningMonster)
	{
		return 0.0f;
	}

	return OwningMonster->GetHealthPercentRatio();
}
