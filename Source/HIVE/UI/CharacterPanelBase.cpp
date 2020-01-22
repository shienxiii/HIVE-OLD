// Copyright of Honeycomb Studio


#include "CharacterPanelBase.h"

UCharacterPanelBase::UCharacterPanelBase(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)

{
	WidgetStyle.Normal.SetResourceObject(PlayerImage);
	WidgetStyle.Normal.SetImageSize(FVector2D(150.0f, 150.0f));
	WidgetStyle.Normal.DrawAs = ESlateBrushDrawType::Image;

	WidgetStyle.Hovered.SetResourceObject(PlayerImage);
	WidgetStyle.Hovered.SetImageSize(FVector2D(150.0f, 150.0f));
	WidgetStyle.Hovered.DrawAs = ESlateBrushDrawType::Image;

	WidgetStyle.Pressed.SetResourceObject(PlayerImage);
	WidgetStyle.Pressed.SetImageSize(FVector2D(150.0f, 150.0f));
	WidgetStyle.Pressed.DrawAs = ESlateBrushDrawType::Image;
}

void UCharacterPanelBase::SyncButtonAppearance()
{
	WidgetStyle.Normal.SetResourceObject(PlayerImage);
	WidgetStyle.Normal.SetImageSize(FVector2D(150.0f, 150.0f));
	WidgetStyle.Normal.DrawAs = ESlateBrushDrawType::Image;

	WidgetStyle.Hovered.SetResourceObject(PlayerImage);
	WidgetStyle.Hovered.SetImageSize(FVector2D(150.0f, 150.0f));
	WidgetStyle.Hovered.DrawAs = ESlateBrushDrawType::Image;

	WidgetStyle.Pressed.SetResourceObject(PlayerImage);
	WidgetStyle.Pressed.SetImageSize(FVector2D(150.0f, 150.0f));
	WidgetStyle.Pressed.DrawAs = ESlateBrushDrawType::Image;
}