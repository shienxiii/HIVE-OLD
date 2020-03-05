// Copyright of Honeycomb Studio


#include "HIVE_ThirdPersonCamera.h"
#include "HIVE/Gameplay/Monster/MonsterBase.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
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

	FRotator targetRotation = OwningPlayer->GetControlRotation();

	if (OwningPlayer->GetCurrentLockOnTarget())
	{
		GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Red, targetRotation.ToString());

		// Find the point between the OwningPlayer and the lock on target
		FVector lookPosition = (OwningPlayer->GetActorLocation() + TARGET->GetActorLocation()) / 2.0f;


		FVector cameraLocation = this->GetComponentLocation();


		targetRotation = UKismetMathLibrary::FindLookAtRotation(cameraLocation, lookPosition);
		targetRotation.Roll = 0.0f;

		GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Green, targetRotation.ToString());
	}

	this->SetWorldRotation(FQuat(targetRotation));
	

}

void UHIVE_ThirdPersonCamera::PlayerRotateCameraPitch(float inValue)
{
	AddWorldRotation(FRotator(PitchRate, 0.0f, 0.0f) * inValue * UGameplayStatics::GetWorldDeltaSeconds(GetWorld()));
}
