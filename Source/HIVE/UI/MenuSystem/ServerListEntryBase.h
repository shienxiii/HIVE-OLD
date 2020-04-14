// Copyright of Honeycomb Studio

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ServerListEntryBase.generated.h"

class UButton;
class UTextBlock;
class UMainMenuBase;

/**
 * 
 */
UCLASS()
class HIVE_API UServerListEntryBase : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	UPROPERTY()
		FString ServerID = TEXT("<SERVER_ID>");

	UPROPERTY()
		int32 Ping = 99;

	UPROPERTY(meta = (BindWidget))
		UTextBlock* ServerID_Txt;

	UPROPERTY(meta = (BindWidget))
		UTextBlock* Ping_Txt;

	UPROPERTY(meta = (BindWidget))
		UButton* SessionBtn;

	UPROPERTY()
		UMainMenuBase* Parent;

	uint32 SessionIndex;


	FSlateColor DefaultTxtColor;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Setup)
		FSlateColor HoverTxtColor;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Setup)
		FSlateColor SelectedTxtColor;

public:
	virtual void NativeOnInitialized() override;

	UFUNCTION()
		void SetSessionInfo(FString InServerID, int32 InPing);
	
	UFUNCTION()
		void Setup(UMainMenuBase* InParent, uint32 InIndex);

	// Return if the session displayed by this entry is selected
	bool CurrentlySelected();

	/**
	 * Click event for this entry
	 */
	UFUNCTION() void SessionSelectedEvent();

	/**
	 * Hover event for this entry
	 */
	UFUNCTION() void EntryHovered();

	/**
	 * Unhover event for this entry
	 */
	UFUNCTION() void EntryUnHovered();

	/**
	 * To be run when another entry is selected
	 */
	UFUNCTION() void UnSelectSession();

	void ChangeTextBlockColors(FSlateColor InColor);
};
