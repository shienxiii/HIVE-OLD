// Copyright of Rabbit Games


#include "MonsterBase.h"
#include "Components/InputComponent.h"
#include "Engine/World.h"
#include "Net/UnrealNetwork.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
AMonsterBase::AMonsterBase(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UMonsterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	// Initialize replication
	SetReplicates(true);
	SetReplicatingMovement(true);
	bAlwaysRelevant = true;
	bOnlyRelevantToOwner = false;
	bNetLoadOnClient = true;
}

// Called when the game starts or when spawned
void AMonsterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMonsterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called by Tick() on every frame
void AMonsterBase::LockedOnTick(float deltaTime)
{
	if (currentTarget)
	{

		FRotator finalRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), currentTarget->GetActorLocation());
		finalRotation.Pitch = 0.0f;
		finalRotation.Roll = 0.0f;

		FRotator deltaRotation = UKismetMathLibrary::RInterpTo(GetActorRotation(), finalRotation, deltaTime, 45.0f);

		//FRotator::
		SetActorRotation(deltaRotation);

	}
}

// Called to bind functionality to input
void AMonsterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AMonsterBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

TArray<AActor*> AMonsterBase::GetPotentialLockOnTargets()
{
	TArray<AActor*> lockOnTargets;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AMonsterBase::StaticClass(), lockOnTargets);

	for (int32 i = lockOnTargets.Num() - 1; i >= 0; i--)
	{
		// Remove any target that is out of view or out of range
		if (!(lockOnTargets[i]->WasRecentlyRendered()) || GetDistanceTo(lockOnTargets[i]) > lockOnRange || lockOnTargets[i] == this)
		{
			lockOnTargets.RemoveAt(i);
		}
	}

	return lockOnTargets;

	return TArray<AActor*>();
}

#pragma region Networking
void AMonsterBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMonsterBase, health);
}
#pragma endregion
