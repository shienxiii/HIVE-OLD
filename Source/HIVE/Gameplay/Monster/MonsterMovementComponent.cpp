// Copyright of Honeycomb Studio


#include "MonsterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Net/UnrealNetwork.h"
#include "MonsterBase.h"
#include "Engine.h"

#pragma region FSavedMove_Monster

bool FSavedMove_Monster::CanCombineWith(const FSavedMovePtr& newMove, ACharacter* inCharacter, float maxDelta) const
{
	if (bSavedRequestMaxWalkSpeedChange != ((FSavedMove_Monster*)&newMove)->bSavedRequestMaxWalkSpeedChange)
	{
		return false;
	}
	return Super::CanCombineWith(newMove, inCharacter, maxDelta);
}

void FSavedMove_Monster::Clear()
{
	Super::Clear();

	bSavedRequestMaxWalkSpeedChange = false;
	bSavedRequestLaunch = false;
	SavedLaunchDirection = FVector::ZeroVector;
	SavedLaunchStrength = 0.0f;
}

uint8 FSavedMove_Monster::GetCompressedFlags() const
{
	uint8 result = Super::GetCompressedFlags();

	if (bSavedRequestMaxWalkSpeedChange)
	{
		result |= FLAG_Custom_0;
	}

	if (bSavedRequestLaunch)
	{
		result |= FLAG_Custom_1;
	}

	return result;
}

void FSavedMove_Monster::SetMoveFor(ACharacter* character, float inDeltaTime, FVector const& newAccel, FNetworkPredictionData_Client_Character& clientData)
{
	Super::SetMoveFor(character, inDeltaTime, newAccel, clientData);

	UMonsterMovementComponent* movement = Cast<UMonsterMovementComponent>(character->GetCharacterMovement());

	if (movement)
	{
		bSavedRequestMaxWalkSpeedChange = movement->bRequestWalkSpeedChange;
		bSavedRequestLaunch = movement->bRequestLaunch;
		SavedLaunchDirection = movement->LaunchDirection;
		SavedLaunchStrength = movement->LaunchStrength;
	}
}

void FSavedMove_Monster::PrepMoveFor(ACharacter* character)
{
	Super::PrepMoveFor(character);

	UMonsterMovementComponent* movement = Cast<UMonsterMovementComponent>(character->GetCharacterMovement());

	if (movement)
	{
		movement->LaunchDirection = SavedLaunchDirection;
		movement->LaunchStrength = SavedLaunchStrength;
	}
}

#pragma endregion


#pragma region FNetworkPredictionData_Client_Monster

FNetworkPredictionData_Client_Monster::FNetworkPredictionData_Client_Monster(const UCharacterMovementComponent& clientMovement)
	: Super(clientMovement)
{
}

FSavedMovePtr FNetworkPredictionData_Client_Monster::AllocateNewMove()
{
	return FSavedMovePtr(new FSavedMove_Monster());
}

#pragma endregion


void UMonsterMovementComponent::InitializeComponent()
{
	Super::InitializeComponent();

	MonsterOwner = Cast<AMonsterBase>(PawnOwner);
	BaseYawRotation = RotationRate.Yaw;
}



#pragma region WalkSpeed
bool UMonsterMovementComponent::Server_SetMaxWalkSpeed_Validate(const float InWalkSpeed)
{
	return true;
}

void UMonsterMovementComponent::Server_SetMaxWalkSpeed_Implementation(const float InWalkSpeed)
{
	NewMaxWalkSpeed = InWalkSpeed;
}

void UMonsterMovementComponent::Client_SetMaxWalkSpeed(float InWalkSpeed)
{
	if (PawnOwner->IsLocallyControlled())
	{
		NewMaxWalkSpeed = InWalkSpeed; // client side
		Server_SetMaxWalkSpeed(NewMaxWalkSpeed); // server side
	}
	
	bRequestWalkSpeedChange = true;

}
#pragma endregion

#pragma region LaunchMonster
bool UMonsterMovementComponent::Server_LaunchMonster_Validate(FVector InLaunchDirection, float InLaunchStrength, ELaunchType InLaunchState)
{
	return true;
}

void UMonsterMovementComponent::Server_LaunchMonster_Implementation(FVector InLaunchDirection, float InLaunchStrength, ELaunchType InLaunchState)
{
	LaunchDirection = InLaunchDirection;
	LaunchStrength = InLaunchStrength;
	NewLaunchState = InLaunchState;
}

void UMonsterMovementComponent::Client_Dodge(FVector InLaunchDirection, float InLaunchStrength, ELaunchType InLaunchState)
{
	LaunchDirection = InLaunchDirection;
	LaunchStrength = InLaunchStrength;
	NewLaunchState = InLaunchState;
	Server_LaunchMonster(LaunchDirection, LaunchStrength, InLaunchState);
	bRequestLaunch = true;
}


#pragma endregion

#pragma region NetworkPrediciton
void UMonsterMovementComponent::PhysWalking(float DeltaTime, int32 Iterations)
{
	if (LaunchState != ELaunchType::LT_NULL)
	{
		if (Velocity.IsNearlyZero())
		{
			LaunchState = ELaunchType::LT_NULL;
			GroundFriction = 8.0f;
		}
	}

	Super::PhysWalking(DeltaTime, Iterations);
}


FVector UMonsterMovementComponent::ConsumeInputVector()
{
	return PawnOwner && LaunchState == ELaunchType::LT_NULL ? PawnOwner->Internal_ConsumeMovementInputVector() : FVector::ZeroVector;
}

FRotator UMonsterMovementComponent::GetDeltaRotation(float DeltaTime) const
{
	// No point using this UMonsterMovementComponent outside of a class of AMonsterBase
	check(MonsterOwner != NULL);

	if (MonsterOwner->GetCurrentLockOnTarget() && LaunchState == ELaunchType::LT_NULL)
	{
		return Super::GetDeltaRotation(DeltaTime * (Acceleration.Size() / MaxAcceleration));
	}

	return Super::GetDeltaRotation(DeltaTime);
}

FRotator UMonsterMovementComponent::ComputeOrientToMovementRotation(const FRotator& CurrentRotation, float DeltaTime, FRotator& DeltaRotation) const
{
	// No point using this UMonsterMovementComponent outside of a class of AMonsterBase
	check(MonsterOwner != NULL);


	if (MonsterOwner->GetCurrentLockOnTarget() &&
		(Acceleration.Size() > 0.0f || LaunchState == ELaunchType::LT_DODGE))
	{
		// Rotate towards the currently locked on target if is accelerating or dodging
		return UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), MonsterOwner->GetCurrentLockOnTarget()->GetActorLocation());
	}

	return Super::ComputeOrientToMovementRotation(CurrentRotation, DeltaTime, DeltaRotation);
}

void UMonsterMovementComponent::OnMovementUpdated(float DeltaTime, const FVector& OldLocation, const FVector& OldVelocity)
{
	Super::OnMovementUpdated(DeltaTime, OldLocation, OldVelocity); 

	if (!CharacterOwner)
	{
		return;
	}

	if (bRequestWalkSpeedChange)
	{
		bRequestWalkSpeedChange = false;
		MaxWalkSpeed = NewMaxWalkSpeed;
	}

	if (bRequestLaunch)
	{
		bRequestLaunch = false;
		// Only dodge when on ground
		bool canLaunch = (IsMovingOnGround() && NewLaunchState == ELaunchType::LT_DODGE) || (NewLaunchState == ELaunchType::LT_KNOCKBACK);

		if (canLaunch)
		{
			LaunchDirection.Normalize();
			FVector launchVelocity = LaunchDirection * LaunchStrength;
			launchVelocity.Z = 0.0f;
			LaunchState = NewLaunchState;
			GroundFriction = 0.0f;
			Launch(launchVelocity);
			NewLaunchState = ELaunchType::LT_NULL;
		}
	}
}


void UMonsterMovementComponent::UpdateFromCompressedFlags(uint8 Flags)
{
	Super::UpdateFromCompressedFlags(Flags);

	bRequestWalkSpeedChange = (Flags & FSavedMove_Character::FLAG_Custom_0) != 0;
	bRequestLaunch = (Flags & FSavedMove_Character::FLAG_Custom_1) != 0;
}

FNetworkPredictionData_Client* UMonsterMovementComponent::GetPredictionData_Client() const
{
	check(PawnOwner != NULL);
	//check(PawnOwner->GetLocalRole() < ROLE_Authority);

	if (!ClientPredictionData)
	{
		UMonsterMovementComponent* mutableThis = const_cast<UMonsterMovementComponent*> (this);

		mutableThis->ClientPredictionData = new FNetworkPredictionData_Client_Monster(*this);
		mutableThis->ClientPredictionData->MaxSmoothNetUpdateDist = 92.0f;
		mutableThis->ClientPredictionData->NoSmoothNetUpdateDist = 140.0f;
	}

	return ClientPredictionData;
}
#pragma endregion


#pragma region Networking
void UMonsterMovementComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UMonsterMovementComponent, LaunchState);
}
#pragma endregion