// Copyright of Honeycomb Studio

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MonsterStatHUD.generated.h"

class AMonsterBase;
class UTextBlock;
class UProgressBar;

/**
 * 
 */
UCLASS()
class HIVE_API UMonsterStatHUD : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(BlueprintReadOnly)
		AMonsterBase* Monster;

	UPROPERTY(meta = (BindWidget))
		UProgressBar* HP;
public:
	virtual bool Initialize() override;

	void BindMonster(AMonsterBase* InMonster);
	void UnbindMonster();

	UFUNCTION(BlueprintPure)
		float HP_Ratio();
};
