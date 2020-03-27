// Copyright of Honeycomb Studio


#include "MonsterBase.h"
#include "Components/CapsuleComponent.h"
#include "Components/BoxComponent.h"
#include "Components/InputComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "MonsterAnimBase.h"
#include "Engine/World.h"
#include "Net/UnrealNetwork.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/PlayerState.h"
#include "HIVE/Gameplay/Controller/MonsterController.h"
#include "HIVE/Gameplay/Camera/HIVE_ThirdPersonCamera.h"
#include "GameFramework/SpringArmComponent.h"

// Sets default values
AMonsterBase::AMonsterBase(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UMonsterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	
	PrimaryActorTick.bCanEverTick = true;

	HurtBox = CreateDefaultSubobject<UCapsuleComponent>(TEXT("HurtBox"));
	HurtBox->SetCapsuleRadius(GetCapsuleComponent()->GetScaledCapsuleRadius());
	HurtBox->SetCapsuleHalfHeight(GetCapsuleComponent()->GetScaledCapsuleHalfHeight());
	HurtBox->SetCollisionProfileName(FName("HurtBox"));
	HurtBox->SetupAttachment(RootComponent);
	HurtBox->bHiddenInGame = false;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("Camera Boom"));
	CameraBoom->SetRelativeLocation(FVector(0.0f, 0.0f, 90.0f));
	CameraBoom->bUsePawnControlRotation = true;
	//CameraBoom->bEnableCameraLag = true;
	//CameraBoom->bEnableCameraRotationLag = true;
	CameraBoom->CameraLagMaxDistance = 100.0f;
	CameraBoom->TargetArmLength = 300.0f;
	CameraBoom->SetupAttachment(RootComponent);

	Camera = CreateDefaultSubobject<UHIVE_ThirdPersonCamera>(TEXT("Camera"));
	Camera->SetRelativeLocation(FVector(0.0f, 120.0f, 0.0f));
	Camera->SetOwningPlayer(this);
	Camera->bWantsInitializeComponent = true;
	Camera->SetupAttachment(CameraBoom);


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


#include "Engine/Engine.h"
void AMonsterBase::OnHealthRep()
{
	GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Yellow, TEXT("HealthReplicated"));
}

#pragma region Input
// Called to bind functionality to input
void AMonsterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	InputComponent->BindAxis("Forward", this, &AMonsterBase::MoveForward);
	InputComponent->BindAxis("Right", this, &AMonsterBase::MoveRight);
	InputComponent->BindAxis("Turn", this, &AMonsterBase::Turn);
	InputComponent->BindAxis("LookUp", this, &AMonsterBase::LookUp);

	InputComponent->BindAction("Jump", EInputEvent::IE_Pressed, this, &AMonsterBase::Jump);
	InputComponent->BindAction("Jump", EInputEvent::IE_Released, this, &AMonsterBase::StopJumping);
	InputComponent->BindAction("LockOn", EInputEvent::IE_Pressed, this, &AMonsterBase::ToggleLockOn);
	InputComponent->BindAction("LightAttack", EInputEvent::IE_Pressed, this, &AMonsterBase::LightAttack);
	InputComponent->BindAction("HeavyAttack", EInputEvent::IE_Pressed, this, &AMonsterBase::HeavyAttack);
	InputComponent->BindAction("Dodge", EInputEvent::IE_Pressed, this, &AMonsterBase::ExecuteDodge);
}

void AMonsterBase::MoveForward(float inAxis)
{
	if (!bCanMove) { return; }
	AddMovementInput(UKismetMathLibrary::GetForwardVector(GetViewRotator()), inAxis);
}

void AMonsterBase::MoveRight(float inAxis)
{
	if (!bCanMove) { return; }
	AddMovementInput(UKismetMathLibrary::GetRightVector(GetViewRotator()), inAxis);
}

void AMonsterBase::Turn(float inAxis)
{
	if (!CurrentTarget)
	{
		AddControllerYawInput(inAxis * TurnRate * UGameplayStatics::GetWorldDeltaSeconds(this));
	}
}

void AMonsterBase::LookUp(float inAxis)
{
	if (!CurrentTarget)
	{
		AddControllerPitchInput(inAxis * TurnRate * UGameplayStatics::GetWorldDeltaSeconds(this));
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
	if (!bCanRegisterAttackInput) { return; }

	UMonsterAnimBase* animClass = Cast<UMonsterAnimBase>(GetMesh()->GetAnimClass());
	if (!animClass)
	{
		return;
	}
	
	AttackRegister = EAttackType::AT_LIGHT;
}

void AMonsterBase::HeavyAttack()
{
	if (!bCanRegisterAttackInput) { return; }
	UMonsterAnimBase* animClass = Cast<UMonsterAnimBase>(GetMesh()->GetAnimClass());
	if (!animClass)
	{
		return;
	}

	AttackRegister = EAttackType::AT_HEAVY;
}

void AMonsterBase::ExecuteDodge()
{
	FVector dodgeDirection = GetLastMovementInputVector();
	if (dodgeDirection.IsNearlyZero())
	{
		dodgeDirection = GetActorForwardVector() * -1.0f;
	}
	dodgeDirection.Normalize();

	GetMonsterMovement()->Client_Dodge(dodgeDirection, DodgeStrength);
}

#pragma endregion

#pragma region LockOn


EAttackType AMonsterBase::ConsumeAttackRegister()
{
	EAttackType retVal = AttackRegister;
	AttackRegister = EAttackType::AT_NULL;
	return retVal;
}

void AMonsterBase::RecoverFromAttack()
{
	bCanRegisterAttackInput = true;
}

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
	FRotator deltaRotation = UKismetMathLibrary::RInterpTo_Constant(Controller->GetControlRotation(), finalRotation, DeltaTime, GetCharacterMovement()->RotationRate.Yaw);

	Controller->SetControlRotation(deltaRotation);
}


#pragma endregion

#pragma region Damage
bool AMonsterBase::Server_ToggleHitBox_Validate()
{
	return true;
}


void AMonsterBase::Server_ToggleHitBox_Implementation()
{
	/*if (HitBox->IsCollisionEnabled())
	{
		HitBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
	else
	{
		HitBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	}*/
}


float AMonsterBase::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	// Currently called on server side
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	Health = FMath::Clamp(Health - DamageAmount, 0.0f, 100.0f);

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
		// Currently written under the assumption that this function runs only when possessed
		GetMonsterController()->PawnRestarted(this);
	}
}
#pragma endregion

#pragma region TeamInterface
ETeamEnum AMonsterBase::GetTeam()
{
	ITeamInterface* teamInterface = Cast<ITeamInterface>(GetPlayerState());
	
	if (teamInterface)
	{
		return teamInterface->GetTeam();
	}

	return ETeamEnum::TE_INVALID;
}
#pragma endregion

AMonsterController* AMonsterBase::GetMonsterController()
{
	return Cast<AMonsterController>(GetController());
}


void AMonsterBase::HitBoxOverlapEvent(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// Make sure only process this overlap event on server side and ignore self
	if (GetLocalRole() < ENetRole::ROLE_Authority || OtherActor == this)
	{
		return;
	}

	ITeamInterface* otherTeam = Cast<ITeamInterface>(OtherActor);

	// Check if OtherActor implements ITeamInterface and is from another team
	if (!otherTeam || otherTeam->GetTeam() == GetTeam())
	{
		return;
	}

	OtherActor->TakeDamage(10.0f, FDamageEvent(), GetController(), this);
}