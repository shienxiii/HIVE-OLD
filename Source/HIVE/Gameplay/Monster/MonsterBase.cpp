// Copyright of Rabbit Games


#include "MonsterBase.h"
#include "Components/InputComponent.h"
#include "Engine/World.h"
#include "Net/UnrealNetwork.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Engine.h"

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
	LockedOnTick(DeltaTime);
}

// Called by Tick() on every frame
void AMonsterBase::LockedOnTick(float DeltaTime)
{
	// Check if there is a target currently being locked on
	if (!currentTarget)
	{
		// to be removed after refining lock on
		if (!(GetCharacterMovement()->bOrientRotationToMovement))
		{
			GetCharacterMovement()->bOrientRotationToMovement = true;
		}

		return;
	}
	else
	{
		GetCharacterMovement()->bOrientRotationToMovement = true;
	}

	// Calculate the desired final rotation
	FRotator finalRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), currentTarget->GetActorLocation());
	finalRotation.Pitch = 0.0f;
	finalRotation.Roll = 0.0f;

	// Calculate the rotation delta
	FRotator deltaRotation = UKismetMathLibrary::RInterpTo_Constant(GetActorRotation(), finalRotation, DeltaTime, GetCharacterMovement()->RotationRate.Yaw * 5.0f);

	SetActorRotation(deltaRotation);
}

// Called to bind functionality to input
void AMonsterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	InputComponent->BindAxis("Forward", this, &AMonsterBase::MoveForward);
	InputComponent->BindAxis("Right", this, &AMonsterBase::MoveRight);

	//InputComponent->BindAction("Block", EInputEvent::IE_Pressed, this, &AMonsterBase::StartBlock);
	//InputComponent->BindAction("Block", EInputEvent::IE_Released, this, &AMonsterBase::EndBlock);

	PlayerInputComponent->BindAction("LockOn", EInputEvent::IE_Pressed, this, &AMonsterBase::ToggleLockOn);
	PlayerInputComponent->BindAction("Dodge", EInputEvent::IE_Pressed, this, &AMonsterBase::ExecuteDodge);
}

void AMonsterBase::MoveForward(float inAxis)
{
	AddMovementInput(UKismetMathLibrary::GetForwardVector(GetViewRotator()), inAxis);
}

void AMonsterBase::MoveRight(float inAxis)
{
	AddMovementInput(UKismetMathLibrary::GetRightVector(GetViewRotator()), inAxis);
}

FRotator AMonsterBase::GetViewRotator()
{
	FRotator controlRotation = GetViewRotation();
	controlRotation.Pitch = 0.0f;
	return controlRotation;
}


#pragma region LockOn

void AMonsterBase::ToggleLockOn()
{
	if (currentTarget == nullptr)
	{
		currentTarget = GetLockOnTarget();
		Server_SetLockOnTarget(currentTarget);
	}
	else
	{
		currentTarget = nullptr;
		Server_SetLockOnTarget();
	}
}

bool AMonsterBase::Server_SetLockOnTarget_Validate(AActor* Target)
{
	return true;
}

void AMonsterBase::Server_SetLockOnTarget_Implementation(AActor* Target)
{
	currentTarget = Target;
}

AActor* AMonsterBase::GetLockOnTarget()
{
	TArray<AActor*> lockOnTargets = GetPotentialLockOnTargets();

	int32 targetIndex = lockOnTargets.Num() - 1;

	for (int32 i = targetIndex - 1; i > -1; i--)
	{
		if (GetDistanceTo(lockOnTargets[i]) < GetDistanceTo(lockOnTargets[targetIndex]))
		{
			targetIndex = i;
		}
	}

	return targetIndex > -1 ? lockOnTargets[targetIndex] : nullptr;
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

#pragma endregion

void AMonsterBase::ExecuteDodge()
{
	//GetMonsterMovementComponent()->Dodge();
}

#pragma region Networking
void AMonsterBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMonsterBase, health);
	DOREPLIFETIME(AMonsterBase, currentTarget);
}
#pragma endregion
