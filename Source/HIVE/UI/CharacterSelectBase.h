// Copyright of Honeycomb Studio

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CharacterSelectBase.generated.h"

class UPanelWidget;
class UMaterialInterface;
class UMaterialInstanceDynamic;
class AMonsterControl;

/**
 * Base class for a character selection system.
 * This class processes the widgets and actions on the assumption that the character panel is arranged using VerticalBox and HorizontalBox
 *
 * How to setup this character selector(recommended):
 * 1. A PanelWidget, preferably a VerticalBox or HorizontalBox, named CharacterOptions. This is mandatory as this is the point of entry for the character selector.
 * 2. Create another VerticalBox or HorizontalBox to be placed under the CharacterOptions
 * 3. Create a CharacterPanelBase widget for the character button and fill in the information required under the Character tab
 *
 * Any widget under CharacterOptions that is not a VerticalBox, HorizontalBox or CharacterPanelBase will be ignored(as of 22/01/2020)
 */
UCLASS()
class HIVE_API UCharacterSelectBase : public UUserWidget
{
	GENERATED_BODY()
		
private:
	/**
	 * A recursive function to be used only by RefreshCharacterPanel() to refresh all the UCharacterPanelBase under CharacterOptions
	 */
	void RecursiveRefreshCharacterPanel(UWidget* InWidget);
	bool IsAcceptablePanel(UWidget* InWidget);

protected:
	AMonsterControl* OwningPlayer = nullptr;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Character Select", meta = (AllowPrivateAccess = "true", DisplayThumbnail = "true"))
		UMaterialInterface* NormalMat;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Character Select")
		UMaterialInterface* HoverMat;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Character Select")
		UMaterialInterface* ClickMat;


	UPROPERTY(meta = (BindWidget))
		UPanelWidget* CharacterOptions;
	
public:
	/**
	 * Synchronise the property of this UUserWidget on both editor and game
	 */
	virtual void SynchronizeProperties() override;

	/**
	 * Refreshes all the UCharacterPanelBase under CharacterOptions
	 */
	UFUNCTION(BlueprintCallable)
		void RefreshCharacterPanel();

	void CharacterSelectEvent(TSubclassOf<class AMonsterBase> InSelectedMonster);
};
