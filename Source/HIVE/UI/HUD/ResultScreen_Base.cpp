// Copyright of Honeycomb Studio


#include "ResultScreen_Base.h"
#include "Components/Button.h"
#include "HIVE/System/GameInstance/HiveGameInstance.h"
#include "Engine/Engine.h"

void UResultScreen_Base::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	if (ReturnBtn) { ReturnBtn->OnClicked.AddDynamic(this, &UResultScreen_Base::ReturnClickEvent); }
}

void UResultScreen_Base::ReturnClickEvent()
{
	UHiveGameInstance* GM = GetGameInstance<UHiveGameInstance>();
	check(GM);
	GM->ReturnToLobby(GetOwningPlayer());
}
