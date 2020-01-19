// Copyright of Honeycomb Studio

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainMenu.generated.h"

class UHiveGameInstance;
class UButton;

/**
 * 
 */
UCLASS()
class HIVE_API UMainMenu : public UUserWidget
{
	GENERATED_BODY()

protected:
    UHiveGameInstance* GameInstance;

    // A UPROPERTY with BindWidget metadata will automatically bind the button on the editor to the variable

    UPROPERTY(meta = (BindWidget))
        UButton* HostBtn;

    UPROPERTY(meta = (BindWidget))
        UButton* JoinBtn;

public:
    virtual bool Initialize() override;
    virtual void OnLevelRemovedFromWorld(ULevel* InLevel, UWorld* InWorld) override;

    void Setup();

    UFUNCTION()
        void HostClickEvent();

};
