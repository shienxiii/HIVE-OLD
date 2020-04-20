// Copyright of Honeycomb Studio


#include "CharacterSelectBase.h"
#include "Components/PanelWidget.h"
#include "Components/VerticalBox.h"
#include "Components/HorizontalBox.h"
#include "Components/TextBlock.h"
#include "CharacterSlotBase.h"
#include "HIVE/Gameplay/Monster/MonsterBase.h"
#include "HIVE/Gameplay/Controller/MonsterController.h"

void UCharacterSelectBase::RecursiveRefreshCharacterPanel(UWidget* InWidget)
{
	if (IsAcceptablePanel(InWidget))
	{
		TArray<UWidget*> panel = (Cast<UPanelWidget>(InWidget))->GetAllChildren();

		for (int i = 0; i < panel.Num(); i++)
		{
			if (panel[i]->IsA<UVerticalBox>() || panel[i]->IsA<UHorizontalBox>() || panel[i]->IsA<UCharacterSlotBase>())
			{
				RecursiveRefreshCharacterPanel(panel[i]);
			}
		}

		return;
	}

	if (InWidget->IsA<UCharacterSlotBase>())
	{
		Cast<UCharacterSlotBase>(InWidget)->SyncButton(this, NormalMat, HoverMat, ClickMat);
	}
}

bool UCharacterSelectBase::IsAcceptablePanel(UWidget* InWidget)
{
	if (InWidget->IsA<UVerticalBox>() || InWidget->IsA<UHorizontalBox>())
	{
		return true;
	}

	return false;
}

void UCharacterSelectBase::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	bIsFocusable = true;
}


void UCharacterSelectBase::SynchronizeProperties()
{
	Super::SynchronizeProperties();
	OwningPlayer = Cast<AMonsterController>(GetOwningPlayer());
	RefreshCharacterPanel();
}

void UCharacterSelectBase::RefreshCharacterPanel()
{
	// Get the child of CharacterOptions
	if (!CharacterOptions)
	{
		return;
	}

	TArray<UWidget*> topPanel = CharacterOptions->GetAllChildren();

	for (int i = 0; i < topPanel.Num(); i++)
	{
		// Run each child of CharacterOptions through the RecursiveRefreshCharacterPanel if they meet the required condition
		if (topPanel[i]->IsA<UVerticalBox>() || topPanel[i]->IsA<UHorizontalBox>() || topPanel[i]->IsA<UCharacterSlotBase>())
		{
			RecursiveRefreshCharacterPanel(topPanel[i]);
		}
	}
}

void UCharacterSelectBase::CharacterSelectEvent(TSubclassOf<class AMonsterBase> InSelectedMonster)
{
	if (!OwningPlayer)
	{
		UE_LOG(LogTemp, Warning, TEXT("No valid owning player"));
		return;
	}

	OwningPlayer->UpdateSelectedMonster(InSelectedMonster);
}
