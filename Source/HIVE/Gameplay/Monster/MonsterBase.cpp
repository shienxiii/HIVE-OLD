// Copyright of Honeycomb Studio


#include "MonsterBase.h"
#include "Components/InputComponent.h"
#include "Engine/World.h"
#include "Net/UnrealNetwork.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "HIVE/Gameplay/Controller/MonsterController.h"

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


	bUseControllerRotationYaw = false;
	GetMonsterMovement()->bOrientRotationToMovement = true;
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
	TurnToLockOnTarget(DeltaTime);
}

#pragma region Input
// Called to bind functionality to input
void AMonsterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	InputComponent->BindAxis("Forward", this, &AMonsterBase::MoveForward);
	InputComponent->BindAxis("Right", this, &AMonsterBase::MoveRight);
	InputComponent->BindAxis("Turn", this, &AMonsterBase::Turn);

	InputComponent->BindAction("Jump", EInputEvent::IE_Pressed, this, &AMonsterBase::Jump);
	InputComponent->BindAction("Jump", EInputEvent::IE_Released, this, &AMonsterBase::StopJumping);
	InputComponent->BindAction("LockOn", EInputEvent::IE_Pressed, this, &AMonsterBase::ToggleLockOn);
	InputComponent->BindAction("LightAttack", EInputEvent::IE_Pressed, this, &AMonsterBase::LightAttack);
	InputComponent->BindAction("HeavyAttack", EInputEvent::IE_Pressed, this, &AMonsterBase::HeavyAttack);
	InputComponent->BindAction("Dodge", EInputEvent::IE_Pressed, this, &AMonsterBase::ExecuteDodge);
}

void AMonsterBase::MoveForward(float inAxis)
{
	AddMovementInput(UKismetMathLibrary::GetForwardVector(GetViewRotator()), inAxis);
}

void AMonsterBase::MoveRight(float inAxis)
{
	AddMovementInput(UKismetMathLibrary::GetRightVector(GetViewRotator()), inAxis);
}

void AMonsterBase::Turn(float inAxis)
{
	if (!CurrentTarget)
	{
		AddControllerYawInput(inAxis * TurnRate * UGameplayStatics::GetWorldDeltaSeconds(this));
	}
}

FRotator AMonsterBase::GetViewRotator()
{
	FRotator controlRotation = GetViewRotation();
	controlRotation.Pitch = 0.0f;
	controlRotation.Roll = 0.0f;
	return controlRotation;
}

void AMonsterBase::LightAttack()
{
	if (!CurrentTarget)
	{
		return;
	}
	Server_AttackHit(10.0f, FDamageEvent(), GetController(), this);
}

void AMonsterBase::HeavyAttack()
{
	if (!CurrentTarget)
	{
		return;
	}

	Server_AttackHit(100.0f, FDamageEvent(), GetController(), this);
}

void AMonsterBase::ExecuteDodge()
{
	FVector dodgeDirection = GetLastMovementInputVector();
	if (dodgeDirection.IsNearlyZero())
	{
		dodgeDirection = GetActorForwardVector() * -1.0f;
	}
	dodgeDirection.Normalize();

	GetMonsterMovement()->Client_LaunchMonster(dodgeDirection, DodgeStrength);
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

void AMonsterBase::TurnToLockOnTarget(float DeltaTime)
{
	if (!CurrentTarget || GetLocalRole() < ROLE_AutonomousProxy)
	{
		return;
	}

	// Calculate the desired final rotation
	FRotator finalRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), CurrentTarget->GetActorLocation());
	finalRotation.Pitch = 0.0f;
	finalRotation.Roll = 0.0f;

	// Calculate the rotation delta
	FRotator deltaRotation = UKismetMathLibrary::RInterpTo_Constant(Controller->GetControlRotation(), finalRotation, DeltaTime, GetCharacterMovement()->RotationRate.Yaw * 2.0f);

	Controller->SetControlRotation(deltaRotation);
}


#pragma endregion

#pragma region Damage
bool AMonsterBase::Server_AttackHit_Validate(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	return true;
}

void AMonsterBase::Server_AttackHit_Implementation(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Health = FMath::Clamp(Health - DamageAmount, 0.0f, 100.0f);
}

float AMonsterBase::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	//Health = FMath::Clamp(Health - DamageAmount, 0.0f, 100.0f);
	Server_AttackHit(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	return DamageAmount;
}

#pragma endregion

#pragma region Networking

void AMonsterBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMonsterBase, Health);
	DOREPLIFETIME(AMonsterBase, CurrentTarget);
}

void AMonsterBase::Restart()
{
	Super::Restart();

	if (GetMonsterController() && IsLocallyControlled())
	{
		GetMonsterController()->ToggleCharacterSelectScreen(false);
	}
}
#pragma endregion

AMonsterController* AMonsterBase::GetMonsterController()
{
	return Cast<AMonsterController>(GetController());
}