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