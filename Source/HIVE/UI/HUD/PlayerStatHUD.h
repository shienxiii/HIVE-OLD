// Copyright of Honeycomb Studio

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerStatHUD.generated.h"

class AMonsterBase;
class UMonsterStat_Base;
class UTextBlock;
class UProgressBar;

/**
 * This is the HUD viewed by the player when playing and controlling their monster
 */
UCLASS()
class HIVE_API UPlayerStatHUD : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(BlueprintReadOnly)
		AMonsterBase* Monster;

	UPROPERTY(meta = (BindWidget))
		UMonsterStat_Base* MonsterStat;
public:
	virtual bool Initialize() override;

	void BindMonster(AMonsterBase* InMonster);
	void UnbindMonster();
};
