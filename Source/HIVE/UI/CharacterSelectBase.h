// Copyright of Honeycomb Studio

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/GridPanel.h"
#include "CharacterSelectBase.generated.h"

/**
 * Base class for a character selection system
 */
UCLASS()
class HIVE_API UCharacterSelectBase : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(meta = (BindWidget))
		UGridPanel* CharacterOptions;
	
public:
	UFUNCTION(BlueprintCallable)
		void RefreshCharacterPanel();
};
