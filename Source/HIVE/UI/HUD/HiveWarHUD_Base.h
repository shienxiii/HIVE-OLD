// Copyright of Honeycomb Studio

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HiveWarHUD_Base.generated.h"


UENUM()
enum class EHUDActiveWidget : uint8
{
	HAW_STAT,
	HAW_CHARACTERSELECT,
	HAW_INGAMEMENU,
	HAW_ENDSCREEN,
	HAW_WAIT
};

class UWidgetSwitcher;
class UCharacterSelectBase;
class UMonsterStat_Base;
class UPlayerStatHUD;
class UResultScreen_Base;
class UInGameMenuBase;
class AMonsterController;
class AMonsterBase;

/**
 * This class is used as the center HUD for the players during HiveWar to switch between active HUD
 */
UCLASS()
class HIVE_API UHiveWarHUD_Base : public UUserWidget
{
	GENERATED_BODY()

protected:
	AMonsterController* OwningPlayer = nullptr;

	UPROPERTY(BlueprintReadOnly)
		AMonsterBase* OwningMonster = nullptr;

	FInputModeGameOnly GameInput;
	FInputModeUIOnly UIInput;
	FInputModeGameAndUI GameAndUIInput;


	EHUDActiveWidget ActiveWidget = EHUDActiveWidget::HAW_CHARACTERSELECT;

	UPROPERTY(meta = (BindWidget)) UWidgetSwitcher* Switcher;

	UPROPERTY(meta = (BindWidget)) UCharacterSelectBase* CharacterSelector;

	UPROPERTY(meta = (BindWidget)) UMonsterStat_Base* MonsterStat;

	UPROPERTY(meta = (BindWidget)) UInGameMenuBase* InGameMenu;

	UPROPERTY(meta = (BindWidget)) UResultScreen_Base* ResultScreen;

	UPROPERTY(meta = (BindWidget)) UUserWidget* WaitScreen;

	// NOTE: This is the overlay for the HUD that displays the monster stat and lock on icon
	UPROPERTY(meta = (BindWidget)) UWidget* PlayerHUD;

	UPROPERTY(meta = (BindWidget)) UWidget* LockOnIcon;

	virtual void InitializeInputComponent() override;

public:
	virtual void NativeOnInitialized() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	/**
	 * Override synchronise the property of this UUserWidget on both editor and game
	 */
	virtual void SynchronizeProperties() override;

	UFUNCTION()
		virtual void OpenInGameMenu();

	bool SwitchActivePanel(EHUDActiveWidget InNewActiveWidget);


	/**
	 * Binds the monster currently controlled by the player allow the UI to reflect the monster's stat
	 *
	 * @param InMonster The monster currently controlled by the owning player
	 */
	void BindMonster(AMonsterBase* InMonster);

	/**
	 * Used as an intermediary to tell the Switcher to switch back to the MonsterStat HUD
	 */
	UFUNCTION() void ReturnToGame();

	/**
	 * Calculates the location of the actor in UMG canvas space
	 *
	 * @param InActor The actor in which to calculate the UMG position of
	 *
	 * @return FVector2D position on UMG space
	 */
	UFUNCTION(BlueprintPure) FVector2D GetWorldPositionToScreenPositionUMGScaled(AActor* InActor);
};
