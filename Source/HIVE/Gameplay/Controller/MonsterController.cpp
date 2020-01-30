// Copyright of Honeycomb Studio


#include "MonsterController.h"
#include "HIVE/System/GameMode/GM_HiveWar.h"
#include "HIVE/UI/CharacterSelect/CharacterSelectBase.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "Net/UnrealNetwork.h"
#include "UObject/ConstructorHelpers.h"

void AMonsterController::BeginPlay()
{
	Super::BeginPlay();

	if (GetLocalRole() != ENetRole::ROLE_AutonomousProxy)
	{
		return;
	}

	CharacterSelect = CreateWidget<UCharacterSelectBase>(this, CharacterSelectBP);

	CharSelInputMode.SetLockMouseToViewportBehavior(EMouseLockMode::LockInFullscreen);
	CharSelInputMode.SetWidgetToFocus(CharacterSelect->TakeWidget());

	ToggleCharacterSelectScreen(true);
}

AMonsterController::AMonsterController(const FObjectInitializer& ObjectInitializer)
{
	if (CharacterSelectBP != NULL && CharacterSelectBP != UCharacterSelectBase::StaticClass())
	{
		return;
	}

	static ConstructorHelpers::FClassFinder<UUserWidget> charSelect(TEXT("/Game/Blueprint/UI/Gameplay/CharacterSelect.CharacterSelect_C"));

	if (charSelect.Class != nullptr)
	{
		CharacterSelectBP = charSelect.Class;
	}
}

bool AMonsterController::UpdateSelectedMonster_Validate(TSubclassOf<AMonsterBase> InNewMonster)
{
	return true;
}

void AMonsterController::UpdateSelectedMonster_Implementation(TSubclassOf<AMonsterBase> InNewMonster)
{
	SelectedMonster = InNewMonster;
	SpawnSelectedMonster();
}

bool AMonsterController::SpawnSelectedMonster_Validate()
{
	return true;
}

void AMonsterController::SpawnSelectedMonster_Implementation()
{
	// This function should only be run on the server
	if (GetLocalRole() < ENetRole::ROLE_Authority)
	{
		return;
	}

	// Get the GameMode and cast it to a compatible class
	AGM_HiveWar* gameMode = Cast<AGM_HiveWar>(UGameplayStatics::GetGameMode(GetWorld()));

	// Make sure the current GameMode is compatible
	if (!gameMode)
	{
		return;
	}

	// Request game mode to spawn the monster for this controller
	gameMode->SpawnMonsterForController(this);
}

void AMonsterController::ToggleCharacterSelectScreen(bool ToggleOn)
{
	if (GetLocalRole() == ENetRole::ROLE_AutonomousProxy)
	{
		if (ToggleOn)
		{
			SetInputMode(CharSelInputMode);
			bShowMouseCursor = true;
			CharacterSelect->AddToViewport();
		}
		else
		{
			SetInputMode(GameInputMode);
			bShowMouseCursor = false;
			CharacterSelect->RemoveFromViewport();
		}
	}
}

void AMonsterController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMonsterController, TeamIndex);
	DOREPLIFETIME(AMonsterController, SelectedMonster);

}
