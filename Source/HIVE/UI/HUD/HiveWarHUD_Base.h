// Copyright of Honeycomb Studio

#pragma once

#include "CoreMinimal.h"

#include "Blueprint/UserWidget.h"
#include "HiveWarHUD_Base.generated.h"


UENUM()
enum class EHUDActiveWidget : uint8
{
	HAW_STAT,
	HAW_CHARACTERSELECT
};

class UWidgetSwitcher;
class UCharacterSelectBase;
class UMonsterHUD;
class AMonsterController;

/**
 * This class is used as the center HUD for the players during HiveWar to switch between active HUD
 */
UCLASS()
class HIVE_API UHiveWarHUD_Base : public UUserWidget
{
	GENERATED_BODY()

protected:
	AMonsterController* OwningPlayer = nullptr;
	FInputModeGameOnly GameInput;
	FInputModeUIOnly UIInput;
	FInputModeGameAndUI GameAndUIInput;


	EHUDActiveWidget ActiveWidget = EHUDActiveWidget::HAW_CHARACTERSELECT;

	UPROPERTY(meta = (BindWidget))
		UWidgetSwitcher* Switcher;

	UPROPERTY(meta = (BindWidget))
		UCharacterSelectBase* CharacterSelector;

	UPROPERTY(meta = (BindWidget))
		UUserWidget* Stat;


public:
	/**
	 * Override synchronise the property of this UUserWidget on both editor and game
	 */
	virtual void SynchronizeProperties() override;

	bool SwitchActivePanel(EHUDActiveWidget InNewActiveWidget);
};