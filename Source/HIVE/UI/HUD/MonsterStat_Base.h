// Copyright of Honeycomb Studio

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MonsterStat_Base.generated.h"

class UProgressBar;
class AMonsterBase;

/**
 * This class is used to display the status information of the current monster controlled by the player
 */
UCLASS()
class HIVE_API UMonsterStat_Base : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly)
		AMonsterBase* Monster = nullptr;

	UPROPERTY(meta = (BindWidget))
		UProgressBar* HP;

public:
	virtual bool Initialize() override;

	void BindMonster(AMonsterBase* InMonster) { Monster = InMonster; }
	void UnbindMonster() { Monster = nullptr; }

	UFUNCTION(BlueprintPure)
		float HP_Ratio();
};
