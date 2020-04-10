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

public:
	virtual void NativeOnInitialized() override;

	UFUNCTION()
		void SetSessionInfo(FString InServerID, int32 InPing);
	
	UFUNCTION()
		void Setup(UMainMenuBase* InParent, uint32 InIndex);

	UFUNCTION()
		void SessionSelectedEvent();

};
