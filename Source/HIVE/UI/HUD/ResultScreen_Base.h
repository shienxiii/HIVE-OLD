// Copyright of Honeycomb Studio

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ResultScreen_Base.generated.h"

/**
 * 
 */
UCLASS()
class HIVE_API UResultScreen_Base : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void InitializeInputComponent() override;

public:
	virtual void NativeOnInitialized() override;
	virtual FReply NativeOnFocusReceived(const FGeometry& InGeometry, const FFocusEvent& InFocusEvent) override;
	virtual void NativeOnFocusLost(const FFocusEvent& InFocusEvent) override;
	virtual void RegisterInput() { RegisterInputComponent(); }
	virtual void StartButtonEvent();
};
