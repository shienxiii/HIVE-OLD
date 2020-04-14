// Copyright of Honeycomb Studio


#include "MainMenuBase.h"
#include "Components/WidgetSwitcher.h"
#include "Components/ComboBoxString.h"
#include "Components/PanelWidget.h"
#include "Components/Button.h"
#include "Components/ScrollBox.h"
#include "ServerListEntryBase.h"
#include "UObject/ConstructorHelpers.h"
#include "HIVE/System/GameInstance/HiveGameInstance.h"

UMainMenuBase::UMainMenuBase(const FObjectInitializer& ObjectInitializer)
	: UUserWidget(ObjectInitializer)
{
	static ConstructorHelpers::FClassFinder<UUserWidget> ServerBP(TEXT("/Game/Blueprint/UI/MainMenu/ServerListEntry.ServerListEntry_C"));

	if (ServerBP.Class != nullptr)
	{
		ServerEntryClass = ServerBP.Class;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Fail to find BP"));
	}
}

void UMainMenuBase::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	bIsFocusable = true;

	GameInstance = Cast<UHiveGameInstance>(GetGameInstance());
	if (!GameInstance) { return; }
	
	// Set click events for the buttons
	if (LobbyBtn) { LobbyBtn->OnClicked.AddDynamic(this, &UMainMenuBase::LobbyClickEvent); }

	if (QuitBtn) { QuitBtn->OnClicked.AddDynamic(this, &UMainMenuBase::QuitClickEvent); }

	if (HostBtn) { HostBtn->OnClicked.AddDynamic(this, &UMainMenuBase::HostClickEvent); }

	if (JoinBtn) { JoinBtn->OnClicked.AddDynamic(this, &UMainMenuBase::JoinClickEvent); }

	if (FindSessionsBtn) { FindSessionsBtn->OnClicked.AddDynamic(this, &UMainMenuBase::FindSessionsClickEvent); }

	if (BackBtn) { BackBtn->OnClicked.AddDynamic(this, &UMainMenuBase::BackClickEvent); }


	Setup();
}

void UMainMenuBase::OnLevelRemovedFromWorld(ULevel* InLevel, UWorld* InWorld)
{
	Super::OnLevelRemovedFromWorld(InLevel, InWorld);

	FInputModeGameOnly gameInputMode;
	APlayerController* controller = GetOwningPlayer();
	controller->SetInputMode(gameInputMode);
	controller->bShowMouseCursor = false;
}

void UMainMenuBase::Setup()
{
	UWorld* world = GetWorld();
	if (!world) { return; }

	APlayerController* controller = world->GetFirstPlayerController();
	if (!controller) { return; }

	FInputModeUIOnly uiInputMode;
	uiInputMode.SetWidgetToFocus(TakeWidget());
	uiInputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);

	controller->SetInputMode(uiInputMode);
	controller->bShowMouseCursor = true;
}

void UMainMenuBase::LobbyClickEvent()
{
	MenuSwitcher->SetActiveWidget(LobbyMenu);
}

void UMainMenuBase::QuitClickEvent()
{
	GameInstance->ExitGame();
}

void UMainMenuBase::HostClickEvent()
{
	GameInstance->Host();
}

void UMainMenuBase::JoinClickEvent()
{
	if (!GameInstance) { UE_LOG(LogTemp, Warning, TEXT("Invalid GameInstance class")); return; }

	if (!SessionIndex.IsSet()) { UE_LOG(LogTemp, Warning, TEXT("No Session Selected")); return; }

	// SessionIndex value corresponds to the indexes of the sessions TArray in the GameInstance
	GameInstance->Join(SessionIndex.GetValue());
}

void UMainMenuBase::FindSessionsClickEvent()
{
	// Clear the list before populating it
	SessionList->ClearChildren();
	SessionIndex.Reset();
	GameInstance->FindSessions();
}

void UMainMenuBase::BackClickEvent()
{
	MenuSwitcher->SetActiveWidget(MainMenu);
}

void UMainMenuBase::PopulateSessionList(TArray<FOnlineSessionSearchResult> InSearchResults)
{
	uint32 i = 0;

	for (FOnlineSessionSearchResult& SearchResult : InSearchResults)
	{
		UE_LOG(LogTemp, Warning, TEXT("Parsing"));
		UServerListEntryBase* newEntry = CreateWidget<UServerListEntryBase>(this, ServerEntryClass);
		newEntry->SetSessionInfo(SearchResult, this, i);
		//newEntry->SetSessionInfo(SearchResult.GetSessionIdStr(), SearchResult.PingInMs);
		//newEntry->Setup(this, i);
		SessionList->AddChild(newEntry);

		i++;
	}

	/*int y = i + 3;
	for (int x = i; x < y; x++)
	{
		UE_LOG(LogTemp, Warning, TEXT("Populating fake server for UI testing"));
		UServerListEntryBase* newEntry = CreateWidget<UServerListEntryBase>(this, ServerEntryClass);

		FString fakeName = FString("Do not select ");
		fakeName.Append(FString::FromInt(x));
		newEntry->SetSessionInfo(FString(fakeName), x);
		SessionList->AddChild(newEntry);
	}*/

}

void UMainMenuBase::SetSessionIndex(uint32 InIndex)
{
	if (SessionIndex.IsSet())
	{
		UServerListEntryBase* prevSelection = Cast<UServerListEntryBase>
														(SessionList->GetChildAt(SessionIndex.GetValue())
																										);

		if (prevSelection)
		{
			prevSelection->UnSelectSession();
		}
	}
	SessionIndex = InIndex;
}