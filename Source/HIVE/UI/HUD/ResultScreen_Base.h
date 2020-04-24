// Copyright of Honeycomb Studio

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ResultScreen_Base.generated.h"

class UButton;

/**
 * 
 */
UCLASS()
class HIVE_API UResultScreen_Base : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(meta = (BindWidget))
		UButton* ReturnBtn;

public:
	virtual void NativeOnInitialized() override;

	UFUNCTION()
		virtual void ReturnClickEvent();
};
