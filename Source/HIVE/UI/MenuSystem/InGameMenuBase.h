// Copyright of Honeycomb Studio

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InGameMenuBase.generated.h"

class UButton;

/**
 * This is the base class for the in-game menu which will pop up to the player when start or esc is pressed
 */
UCLASS()
class HIVE_API UInGameMenuBase : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(meta = (BindWidget))
		UButton* CancelButton;

	UPROPERTY(meta = (BindWidget))
		UButton* QuitButton;


public:
	virtual void NativeOnInitialized() override;

	UFUNCTION()
		void CancelPressedEvent();

	UFUNCTION()
		void QuitPressedEvent();
};
