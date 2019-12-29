// Copyright of Honeycomb Studio


#include "HiveGameInstance.h"
#include "Engine/World.h"
#include "Engine.h"

void UHiveGameInstance::Host()
{
	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Cyan, "Hosting");

	UWorld* world = GetWorld();
	if (!ensure(world != nullptr)) { return; }

	world->ServerTravel("/Game/ThirdPersonBP/Maps/ThirdPersonExampleMap?listen");
}

void UHiveGameInstance::Join(const FString& InAddress)
{
	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Cyan, FString::Printf(TEXT("Joining %s"), *InAddress));

	APlayerController* controller = GetFirstLocalPlayerController();
	if (!ensure(controller != nullptr)) { return; }

	controller->ClientTravel(*InAddress, ETravelType::TRAVEL_Absolute);
}
