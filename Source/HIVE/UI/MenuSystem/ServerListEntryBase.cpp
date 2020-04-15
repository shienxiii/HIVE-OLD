// Copyright of Honeycomb Studio


#include "ServerListEntryBase.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "MainMenuBase.h"

void UServerListEntryBase::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (!SessionBtn) { return; }

	DefaultTxtColor = ServerID_Txt->ColorAndOpacity;

	
	SessionBtn->OnHovered.AddDynamic(this, &UServerListEntryBase::EntryHovered);
	SessionBtn->OnUnhovered.AddDynamic(this, &UServerListEntryBase::EntryUnHovered);
	SessionBtn->OnClicked.AddDynamic(this, &UServerListEntryBase::SessionSelectedEvent);
}


void UServerListEntryBase::SetSessionInfo(FOnlineSessionSearchResult& SearchResult, UMainMenuBase* InParent, uint32 InIndex)
{
	SearchResult.GetSessionIdStr();

	Parent = InParent;
	SessionIndex = InIndex;

	ServerID = SearchResult.GetSessionIdStr();
	Ping = SearchResult.PingInMs;

	ServerID_Txt->SetText(FText::FromString(ServerID));
	Ping_Txt->SetText(FText::FromString(FString::FromInt(Ping)));
	Username_Txt->SetText(FText::FromString(SearchResult.Session.OwningUserName));

	CurrentPlayers = SearchResult.Session.NumOpenPrivateConnections;
	MaxPlayers = SearchResult.Session.SessionSettings.NumPublicConnections;

	PlayerCount_Txt->SetText(FText::FromString(FString::Printf(TEXT("%d/%d"), CurrentPlayers, MaxPlayers)));
}


bool UServerListEntryBase::CurrentlySelected()
{
	// check if the SessionIndex TOptional is set
	if (Parent->GetSessionIndex().IsSet() && Parent->GetSessionIndex().GetValue() == SessionIndex)
	{
		return true;
	}

	return false;
}

void UServerListEntryBase::SessionSelectedEvent()
{
	Parent->SetSessionIndex(SessionIndex);

	ChangeTextBlockColors(SelectedTxtColor);
}

void UServerListEntryBase::EntryHovered()
{
	if (CurrentlySelected()) { return; }

	ChangeTextBlockColors(HoverTxtColor);
}

void UServerListEntryBase::EntryUnHovered()
{
	if (CurrentlySelected()) { return; }

	ChangeTextBlockColors(DefaultTxtColor);
}

void UServerListEntryBase::UnSelectSession()
{
	if (!CurrentlySelected()) { return; }


	ChangeTextBlockColors(DefaultTxtColor);
}

void UServerListEntryBase::ChangeTextBlockColors(FSlateColor InColor)
{
	ServerID_Txt->SetColorAndOpacity(InColor);
	Username_Txt->SetColorAndOpacity(InColor);
	Ping_Txt->SetColorAndOpacity(InColor);
	PlayerCount_Txt->SetColorAndOpacity(InColor);;
}
