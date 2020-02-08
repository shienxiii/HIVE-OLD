// Copyright of Honeycomb Studio


#include "MonsterStatHUD.h"
#include "Components/TextBlock.h"


void UMonsterStatHUD::SetTeamName(FString InTeamName)
{
	TeamName->bIsVariable = true;
	TeamName->SetText(FText::FromString(InTeamName));
}
