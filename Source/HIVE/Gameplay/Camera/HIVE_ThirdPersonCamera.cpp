// Copyright of Honeycomb Studio


#include "HIVE_ThirdPersonCamera.h"
#include "HIVE/Gameplay/Monster/MonsterBase.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/Engine.h"

#define TARGET OwningPlayer->GetCurrentLockOnTarget()


void UHIVE_ThirdPersonCamera::InitializeComponent()
{
	Super::InitializeComponent();

	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = true;
	SetComponentTickEnabled(true);
}

void UHIVE_ThirdPersonCamera::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!OwningPlayer || !OwningPlayer->IsLocallyControlled())
	{
		return;
	}

	if (GetComponentRotation().Roll != 0.0f)
	{
		//SetComponentRotation
	}

	if (!TARGET)
	{
		//message.Append("NULL");
	}
	else
	{
		//message.Append(TARGET->GetActorLabel());
	}
		//GEngine->AddOnScreenDebugMessage(-1, 0.1f, FColor::Cyan, message);
}

void UHIVE_ThirdPersonCamera::PlayerRotateCameraPitch(float inValue)
{
	AddWorldRotation(FRotator(PitchRate, 0.0f, 0.0f) * inValue * UGameplayStatics::GetWorldDeltaSeconds(GetWorld()));
}