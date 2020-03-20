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
};
