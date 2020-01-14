// Copyright of Honeycomb Studio


#include "GM_Lobby.h"
#include "HIVE/Gameplay/Monster/MonsterBase.h"
#include "HIVE/System/Game Instance/HiveGameInstance.h"

AGM_Lobby::AGM_Lobby()
{
	DefaultPawnClass = nullptr;
}

void AGM_Lobby::BeginPlay()
{
	GameInstance = Cast<UHiveGameInstance>(GetGameInstance());
	check(GameInstance != nullptr);

	GameInstance->LoadMenu();

}
