// Copyright of Honeycomb Studio

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MonsterStatHUD.generated.h"

class UTextBlock;

/**
 * 
 */
UCLASS()
class HIVE_API UMonsterStatHUD : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(meta = (BindWidget))
		UTextBlock* TeamName;

public:
	void SetTeamName(FString InTeamName);
};
