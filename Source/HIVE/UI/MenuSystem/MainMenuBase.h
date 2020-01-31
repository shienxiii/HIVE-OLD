// Copyright of Honeycomb Studio

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/WidgetSwitcher.h"
#include "Components/EditableTextBox.h"
#include "MainMenuBase.generated.h"

class UHiveGameInstance;
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
        UButton* HostBtn;

    UPROPERTY(meta = (BindWidget))
        UButton* JoinBtn;

    UPROPERTY(meta = (BindWidget))
        UEditableTextBox* TargetIP;

    UPROPERTY(meta = (BindWidget))
        UButton* JoinServerBtn;

    UPROPERTY(meta = (BindWidget))
        UButton* BackBtn;

public:
    virtual bool Initialize() override;

    // Automatically called when the level in which this widget belonged to is removed from the world
    virtual void OnLevelRemovedFromWorld(ULevel* InLevel, UWorld* InWorld) override;

    void Setup();

    UFUNCTION()
        void HostClickEvent();

    UFUNCTION()
        void JoinClickEvent();

    UFUNCTION()
        void JoinServerClickEvent();

    UFUNCTION()
        void BackClickEvent();
};
