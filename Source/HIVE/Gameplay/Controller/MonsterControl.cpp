// Copyright of Honeycomb Studio


#include "MonsterControl.h"
#include "HIVE/System/GameMode/GM_HiveWar.h"
#include "HIVE/UI/CharacterSelect/CharacterSelectBase.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "Net/UnrealNetwork.h"
#include "UObject/ConstructorHelpers.h"

void AMonsterControl::BeginPlay()
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

AMonsterControl::AMonsterControl(const FObjectInitializer& ObjectInitializer)
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

bool AMonsterControl::UpdateSelectedMonster_Validate(TSubclassOf<AMonsterBase> InNewMonster)
{
	return true;
}

void AMonsterControl::UpdateSelectedMonster_Implementation(TSubclassOf<AMonsterBase> InNewMonster)
{
	SelectedMonster = InNewMonster;
	SpawnSelectedMonster();
}

bool AMonsterControl::SpawnSelectedMonster_Validate()
{
	return true;
}

void AMonsterControl::SpawnSelectedMonster_Implementation()
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

void AMonsterControl::ToggleCharacterSelectScreen(bool ToggleOn)
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

void AMonsterControl::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMonsterControl, TeamIndex);
	DOREPLIFETIME(AMonsterControl, SelectedMonster);

}
