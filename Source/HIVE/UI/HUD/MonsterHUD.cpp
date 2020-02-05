// Copyright of Honeycomb Studio


#include "MonsterHUD.h"
#include "Components/TextBlock.h"


void UMonsterHUD::SetTeamName(FString InTeamName)
{
	TeamName->bIsVariable = true;
	TeamName->SetText(FText::FromString(InTeamName));
}
