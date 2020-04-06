// Copyright of Honeycomb Studio

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainMenuBase.generated.h"

class UHiveGameInstance;
class UWidgetSwitcher;
class UComboBoxString;
class UButton;

/**
 * 
 */
UCLASS()
class HIVE_API UMainMenuBase : public UUserWidget
{
	GENERATED_BODY()

protected:
    UHiveGameInstance* GameInstance;

    // A UPROPERTY with BindWidget metadata will automatically bind the button on the editor to the variable

    UPROPERTY(meta = (BindWidget))
        UWidgetSwitcher* MenuSwitcher;

    UPROPERTY(meta = (BindWidget))
        UWidget* MainMenu;

    UPROPERTY(meta = (BindWidget))
        UWidget* LobbyMenu;

    UPROPERTY(meta = (BindWidget))
        UButton* LobbyBtn;

    UPROPERTY(meta = (BindWidget))
        UButton* QuitBtn;

    UPROPERTY(meta = (BindWidget))
        UButton* HostBtn;

    UPROPERTY(meta = (BindWidget))
        UButton* JoinBtn;

    UPROPERTY(meta = (BindWidget))
        UButton* FindSessionsBtn;

    UPROPERTY(meta = (BindWidget))
        UComboBoxString* TargetIP;

    UPROPERTY(meta = (BindWidget))
        UButton* BackBtn;

public:
    virtual void NativeOnInitialized() override;

    // Automatically called when the level in which this widget belonged to is removed from the world
    virtual void OnLevelRemovedFromWorld(ULevel* InLevel, UWorld* InWorld) override;

    void Setup();

    UFUNCTION()
        void LobbyClickEvent();

    UFUNCTION()
        void QuitClickEvent();

    UFUNCTION()
        void HostClickEvent();

    UFUNCTION()
        void JoinClickEvent();

    UFUNCTION()
        void FindSessionsClickEvent();

    UFUNCTION()
        void BackClickEvent();
};
