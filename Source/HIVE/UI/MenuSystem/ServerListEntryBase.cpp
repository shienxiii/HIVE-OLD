// Copyright of Honeycomb Studio


#include "ServerListEntryBase.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "MainMenuBase.h"

void UServerListEntryBase::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (SessionBtn) { SessionBtn->OnClicked.AddDynamic(this, &UServerListEntryBase::SessionSelectedEvent); }
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
	Ping_Txt->SetText(FText::FromString(FString::FromInt(SessionIndex)));
}

void UServerListEntryBase::SessionSelectedEvent()
{
	Parent->SetSessionIndex(SessionIndex);
}
