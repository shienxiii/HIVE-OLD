// Copyright of Honeycomb Studio


#include "ResultScreen_Base.h"
#include "HIVE/System/GameInstance/HiveGameInstance.h"

void UResultScreen_Base::InitializeInputComponent()
{
	Super::InitializeInputComponent();
}

void UResultScreen_Base::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	InitializeInputComponent();
}
