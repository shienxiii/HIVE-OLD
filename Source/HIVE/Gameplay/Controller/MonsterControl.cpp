// Copyright of Honeycomb Studio


#include "MonsterControl.h"

void AMonsterControl::BeginPlay()
{
	FInputModeGameAndUI gameInputMode;
	gameInputMode.SetLockMouseToViewportBehavior(EMouseLockMode::LockAlways);
	gameInputMode.SetWidgetToFocus(nullptr);

	SetInputMode(gameInputMode);
	bShowMouseCursor = false;
}