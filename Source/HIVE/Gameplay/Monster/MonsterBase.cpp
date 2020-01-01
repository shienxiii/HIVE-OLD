// Copyright of Honeycomb Studio


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

	//GetMovementComponent()->bWantsInitializeComponent = true;
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

#pragma region Input
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

void AMonsterBase::ExecuteDodge()
{
	FVector dodgeDirection = GetLastMovementInputVector();
	if (dodgeDirection.IsNearlyZero())
	{
		dodgeDirection = GetActorForwardVector() * -1.0f;
	}
	dodgeDirection.Normalize();

	GetMonsterMovementComponent()->Client_LaunchMonster(dodgeDirection, DodgeStrength);
	//GetMonsterMovementComponent()->Dodge();
}

#pragma endregion


#pragma region LockOn

void AMonsterBase::ToggleLockOn()
{
	if (CurrentTarget == nullptr)
	{
		CurrentTarget = GetLockOnTarget();
		Server_SetLockOnTarget(CurrentTarget);
	}
	else
	{
		CurrentTarget = nullptr;
		Server_SetLockOnTarget();
	}
}

bool AMonsterBase::Server_SetLockOnTarget_Validate(AActor* Target)
{
	return true;
}

void AMonsterBase::Server_SetLockOnTarget_Implementation(AActor* Target)
{
	CurrentTarget = Target;
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
		if (!(lockOnTargets[i]->WasRecentlyRendered()) || GetDistanceTo(lockOnTargets[i]) > LockOnRange || lockOnTargets[i] == this)
		{
			lockOnTargets.RemoveAt(i);
		}
	}

	return lockOnTargets;

	return TArray<AActor*>();
}

#pragma endregion



#pragma region Networking
void AMonsterBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMonsterBase, Health);
	DOREPLIFETIME(AMonsterBase, CurrentTarget);
}
#pragma endregion
