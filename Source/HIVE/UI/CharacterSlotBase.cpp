// Copyright of Honeycomb Studio


#include "CharacterSlotBase.h"
#include "Materials/MaterialInterface.h"
#include "Materials/Material.h"
#include "Materials/MaterialInstance.h"
#include "Materials/MaterialInstanceDynamic.h"

UCharacterSlotBase::UCharacterSlotBase(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)

{
	/*WidgetStyle.Normal.SetResourceObject(PlayerImage);
	WidgetStyle.Normal.SetImageSize(FVector2D(150.0f, 150.0f));
	WidgetStyle.Normal.DrawAs = ESlateBrushDrawType::Image;

	WidgetStyle.Hovered.SetResourceObject(PlayerImage);
	WidgetStyle.Hovered.SetImageSize(FVector2D(150.0f, 150.0f));
	WidgetStyle.Hovered.DrawAs = ESlateBrushDrawType::Image;

	WidgetStyle.Pressed.SetResourceObject(PlayerImage);
	WidgetStyle.Pressed.SetImageSize(FVector2D(150.0f, 150.0f));
	WidgetStyle.Pressed.DrawAs = ESlateBrushDrawType::Image;*/
}

void UCharacterSlotBase::SyncButtonAppearance(UMaterialInterface* NormalMat, UMaterialInterface* HoverMat, UMaterialInterface* ClickMat)
{
	UMaterialInstanceDynamic* i = UMaterialInstanceDynamic::Create(NormalMat, this);
	i->SetTextureParameterValue("UI_Image", PlayerImage);

	WidgetStyle.Normal.SetResourceObject(i);
	WidgetStyle.Normal.SetImageSize(FVector2D(150.0f, 150.0f));
	WidgetStyle.Normal.DrawAs = ESlateBrushDrawType::Image;

	WidgetStyle.Hovered.SetResourceObject(HoverMat);
	WidgetStyle.Hovered.SetImageSize(FVector2D(150.0f, 150.0f));
	WidgetStyle.Hovered.DrawAs = ESlateBrushDrawType::Image;

	WidgetStyle.Pressed.SetResourceObject(ClickMat);
	WidgetStyle.Pressed.SetImageSize(FVector2D(150.0f, 150.0f));
	WidgetStyle.Pressed.DrawAs = ESlateBrushDrawType::Image;
}