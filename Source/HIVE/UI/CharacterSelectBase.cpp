// Copyright of Honeycomb Studio


#include "CharacterSelectBase.h"
#include "CharacterPanelBase.h"

void UCharacterSelectBase::RefreshCharacterPanel()
{
	TArray<UWidget*> children = CharacterOptions->GetAllChildren();

	for (int i = 0; i < children.Num(); i++)
	{
		UCharacterPanelBase* currentChild = Cast<UCharacterPanelBase>(children[i]);

		if (currentChild)
		{
			currentChild->SyncButtonAppearance();
			UE_LOG(LogTemp, Warning, TEXT("Found Character panel"));
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Found non-child"));
		}
	}
}