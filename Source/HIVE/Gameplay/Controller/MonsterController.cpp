// Copyright of Honeycomb Studio


#include "MonsterController.h"
#include "HIVE/Gameplay/Monster/MonsterBase.h"
#include "HIVE/System/GameMode/GM_HiveWar.h"
#include "HIVE/UI/CharacterSelect/CharacterSelectBase.h"
#include "HIVE/UI/HUD/PlayerStatHUD.h"
#include "HIVE/UI/HUD/HiveWarHUD_Base.h"
#include "HIVE/Gameplay/PlayerState/MonsterPlayerState.h"
#include "HIVE/Gameplay/GameState/HiveWarGameState.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "Net/UnrealNetwork.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/Engine.h"

void AMonsterController::BeginPlay()
{
	Super::BeginPlay();

	//SetupPlayerHUD();
}

void AMonsterController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!IsLocalPlayerController()) { return; }

	//AHiveWarGameState* gameState = Cast<AHiveWarGameState>(UGameplayStatics::GetGameState(this));
}

AMonsterController::AMonsterController(const FObjectInitializer& ObjectInitializer)
{
}

void AMonsterController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAction("Start", EInputEvent::IE_Pressed, this, &AMonsterController::StartButtonEvent);
}

void AMonsterController::SetupPlayerHUD_Implementation()
{
	if (!IsLocalPlayerController() || HUD != NULL) { return; }

	HUD = CreateWidget<UHiveWarHUD_Base>(this, HUD_BP);
	HUD->SetOwningPlayer(this);
	HUD->AddToViewport();
}


void AMonsterController::LoadWaitScreen_Implementation()
{
	HUD->SwitchActivePanel(EHUDActiveWidget::HAW_WAIT);
}

void AMonsterController::LoadCharacterSelectScreen_Implementation()
{
	HUD->SwitchActivePanel(EHUDActiveWidget::HAW_CHARACTERSELECT);
}

void AMonsterController::StartButtonEvent()
{
	HUD->SwitchActivePanel(EHUDActiveWidget::HAW_INGAMEMENU);
}

void AMonsterController::PawnRestarted(AMonsterBase* InMonster)
{
	if (!IsLocalPlayerController())
	{
		return;
	}

	HUD->SwitchActivePanel(EHUDActiveWidget::HAW_STAT);
	HUD->BindMonster(InMonster);
}


void AMonsterController::OnUnPossess()
{
	Super::OnUnPossess();

	if (IsLocalController() && HUD)
	{
		// Open the character select screen
		LoadCharacterSelectScreen();
	}
}

AMonsterPlayerState* AMonsterController::GetMonsterPlayerState()
{
	return GetPlayerState<AMonsterPlayerState>();
}

void AMonsterController::GameHasEnded(AActor* EndGameFocus, bool bIsWinner)
{
	HUD->SwitchActivePanel(EHUDActiveWidget::HAW_ENDSCREEN);
	SetViewTargetWithBlend(EndGameFocus, 0.5f);
}

void AMonsterController::SetCanExitGameEnd()
{
	if (GetLocalRole() != ENetRole::ROLE_Authority) { return; }

	bCanExitGameEnd = true;
}


#pragma region CharacterSelect
bool AMonsterController::UpdateSelectedMonster_Validate(TSubclassOf<AMonsterBase> InNewMonster)
{
	return true;
}

void AMonsterController::UpdateSelectedMonster_Implementation(TSubclassOf<AMonsterBase> InSelectedMonster)
{
	GetMonsterPlayerState()->SetSelectedMonster(InSelectedMonster);
	SpawnSelectedMonster();
}

bool AMonsterController::SpawnSelectedMonster_Validate()
{
	return true;
}

void AMonsterController::SpawnSelectedMonster_Implementation()
{
	// This function should only be run on the server
	if (GetLocalRole() < ENetRole::ROLE_Authority || GetTeam() == ETeamEnum::TE_INVALID || GetTeam() == ETeamEnum::TE_NEUTRAL || !GetSelectedMonster())
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
	if (CanSpawnMonster())
	{
		gameMode->SpawnMonsterForController(this);
		GetMonsterPlayerState()->SetSelectedMonster(NULL);
	}
}


bool AMonsterController::CanSpawnMonster()
{
	return (GetMonsterPlayerState()->GetSelectedMonster() != NULL);
}

TSubclassOf<AMonsterBase> AMonsterController::GetSelectedMonster()
{
	return GetMonsterPlayerState()->GetSelectedMonster();
}
#pragma endregion


#pragma region TeamInterface
bool AMonsterController::AssignTeam(ETeamEnum InTeam)
{
	if (InTeam == ETeamEnum::TE_NEUTRAL || InTeam == ETeamEnum::TE_INVALID) { return false; }

	// Team is assigned through the controller to the PlayerState for replication purpose
	ITeamInterface* teamInterface = Cast<ITeamInterface>(GetMonsterPlayerState());

	// return false if this is called on client or the PlayerState does not implement ITeamInterface
	if (GetLocalRole() != ENetRole::ROLE_Authority || !teamInterface)
	{
		return false;
	}

	return teamInterface->AssignTeam(InTeam);
}

ETeamEnum AMonsterController::GetTeam()
{
	// Team is stored on the PlayerState
	ITeamInterface* teamInterface = Cast<ITeamInterface>(GetMonsterPlayerState());

	if (teamInterface)
	{
		return teamInterface->GetTeam();
	}
	
	return ITeamInterface::GetTeam();
}
#pragma endregion


void AMonsterController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMonsterController, bCanExitGameEnd);

}