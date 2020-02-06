// Copyright of Honeycomb Studio


#include "HiveWarHUD_Base.h"
#include "Components/WidgetSwitcher.h"
#include "HIVE/UI/CharacterSelect/CharacterSelectBase.h"
#include "HIVE/UI/HUD/MonsterHUD.h"
#include "HIVE/Gameplay/Controller/MonsterController.h"

void UHiveWarHUD_Base::SynchronizeProperties()
{
	Super::SynchronizeProperties();
	OwningPlayer = Cast<AMonsterController>(GetOwningPlayer());
}

bool UHiveWarHUD_Base::SwitchActivePanel(EHUDActiveWidget InNewActiveWidget)
{
	switch (InNewActiveWidget)
	{
		case EHUDActiveWidget::HAW_STAT:
			Switcher->SetActiveWidget(Stat);
			OwningPlayer->SetInputMode(GameInput);
			OwningPlayer->bShowMouseCursor = false;
			break;
		case EHUDActiveWidget::HAW_CHARACTERSELECT:
			Switcher->SetActiveWidget(CharacterSelector);

			UIInput.SetLockMouseToViewportBehavior(EMouseLockMode::LockInFullscreen);
			UIInput.SetWidgetToFocus(CharacterSelector->TakeWidget());

			OwningPlayer->SetInputMode(UIInput);
			OwningPlayer->bShowMouseCursor = true;
			break;
		default:
			break;
	}

	return true;
}
