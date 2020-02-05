// Copyright of Honeycomb Studio

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MonsterHUD.generated.h"

class UTextBlock;

/**
 * 
 */
UCLASS()
class HIVE_API UMonsterHUD : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(meta = (BindWidget))
		UTextBlock* TeamName;

public:
	void SetTeamName(FString InTeamName);
};
