// Copyright of Honeycomb Studio


#include "CharacterSlotBase.h"
#include "Materials/MaterialInterface.h"
#include "Materials/Material.h"
#include "Materials/MaterialInstance.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "CharacterSelectBase.h"

UCharacterSlotBase::UCharacterSlotBase(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)

{
	this->OnClicked.AddDynamic(this, &UCharacterSlotBase::CharacterSelectedEvent);
}

void UCharacterSlotBase::SyncButton(UCharacterSelectBase* InCharacterSelector, UMaterialInterface* NormalMat, UMaterialInterface* HoverMat, UMaterialInterface* ClickMat)
{
	CharacterSelector = InCharacterSelector;

	UMaterialInstanceDynamic* normal = UMaterialInstanceDynamic::Create(NormalMat, this);
	normal->SetTextureParameterValue("UI_Image", PlayerImage);

	WidgetStyle.Normal.SetResourceObject(normal);
	WidgetStyle.Normal.SetImageSize(FVector2D(150.0f, 150.0f));
	WidgetStyle.Normal.DrawAs = ESlateBrushDrawType::Image;

	UMaterialInstanceDynamic* hover = UMaterialInstanceDynamic::Create(HoverMat, this);
	hover->SetTextureParameterValue("UI_Image", PlayerImage);

	WidgetStyle.Hovered.SetResourceObject(hover);
	WidgetStyle.Hovered.SetImageSize(FVector2D(150.0f, 150.0f));
	WidgetStyle.Hovered.DrawAs = ESlateBrushDrawType::Image;

	UMaterialInstanceDynamic* click = UMaterialInstanceDynamic::Create(ClickMat, this);
	click->SetTextureParameterValue("UI_Image", PlayerImage);

	WidgetStyle.Pressed.SetResourceObject(click);
	WidgetStyle.Pressed.SetImageSize(FVector2D(150.0f, 150.0f));
	WidgetStyle.Pressed.DrawAs = ESlateBrushDrawType::Image;
}

void UCharacterSlotBase::CharacterSelectedEvent()
{
	if (!CharacterSelector)
	{
		UE_LOG(LogTemp, Warning, TEXT("Cannot find CharacterSelector"));
		return;
	}

	CharacterSelector->CharacterSelectEvent(Monster);
}
