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

void UServerListEntryBase::SetSessionInfo(FString InServerID, int32 InPing)
{
	ServerID	= InServerID;
	Ping		= InPing;

	ServerID_Txt->SetText(FText::FromString(ServerID));
	Ping_Txt->SetText(FText::FromString(FString::FromInt(Ping)));
}

void UServerListEntryBase::Setup(UMainMenuBase* InParent, uint32 InIndex)
{
	Parent = InParent;
	SessionIndex = InIndex;
	//Ping_Txt->SetText(FText::FromString(FString::FromInt(SessionIndex)));
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
	Ping_Txt->SetColorAndOpacity(InColor);
}
