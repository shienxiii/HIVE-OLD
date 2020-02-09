// Copyright of Honeycomb Studio


#include "MonsterStatHUD.h"
#include "HIVE/Gameplay/Monster/MonsterBase.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"


#include "Engine/Engine.h"
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
	/*HP->PercentDelegate.Unbind();*/
}

float UMonsterStatHUD::HP_Ratio()
{
	//GEngine->AddOnScreenDebugMessage(-1, 150.0f, FColor::Green, "Running HP Ratio");
	return 1.0f;
}
