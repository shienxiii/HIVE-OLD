// Copyright of Honeycomb Studio


#include "HiveWarHUD_Base.h"
#include "Components/WidgetSwitcher.h"
#include "HIVE/UI/CharacterSelect/CharacterSelectBase.h"
#include "HIVE/UI/HUD/MonsterStat_Base.h"
#include "HIVE/Gameplay/Controller/MonsterController.h"
#include "HIVE/Gameplay/Monster/MonsterBase.h"
#include "Engine/UserInterfaceSettings.h"
#include "Blueprint/WidgetLayoutLibrary.h"

void UHiveWarHUD_Base::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	LockOnIcon->SetVisibility(Monster && Monster->GetCurrentLockOnTarget() ? ESlateVisibility::Visible : ESlateVisibility::Hidden);

	if (!LockOnIcon->IsVisible()) { return; }

	LockOnIcon->SetRenderTranslation(GetWorldPositionToScreenPositionScaled(Monster->GetCurrentLockOnTarget()));
}

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
			Switcher->SetActiveWidget(PlayerHUD);
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

void UHiveWarHUD_Base::BindMonster(AMonsterBase* InMonster)
{
	Monster = InMonster;
	MonsterStat->BindMonster(InMonster);
}

FVector2D UHiveWarHUD_Base::GetWorldPositionToScreenPositionScaled(AActor* InActor)
{
	// Get the current viewport size
	int32 viewX, viewY;
	GetOwningPlayer()->GetViewportSize(viewX, viewY);

	// Calculate DPI of the viewport
	float dpi = GetDefault<UUserInterfaceSettings>(UUserInterfaceSettings::StaticClass())->GetDPIScaleBasedOnSize(FIntPoint(viewX, viewY));

	FVector2D screenPosition;
	GetOwningPlayer()->ProjectWorldLocationToScreen(InActor->GetActorLocation(), screenPosition, true);
	
	dpi = 1 / dpi;
	
	return screenPosition * dpi;
}
