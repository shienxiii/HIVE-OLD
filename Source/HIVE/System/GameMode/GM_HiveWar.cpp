// Copyright of Honeycomb Studio


#include "GM_HiveWar.h"
#include "HIVE/Gameplay/Monster/MonsterBase.h"
#include "UObject/ConstructorHelpers.h"

AGM_HiveWar::AGM_HiveWar()
{
	static ConstructorHelpers::FClassFinder<APawn> tempMonsterBP(TEXT("/Game/Blueprint/TestCharacter/RedGuy.RedGuy_C"));

	if (tempMonsterBP.Class != NULL)
	{
		DefaultPawnClass = tempMonsterBP.Class;
	}
}