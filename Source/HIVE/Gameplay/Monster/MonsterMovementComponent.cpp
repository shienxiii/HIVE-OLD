// Copyright of Rabbit Games


#include "MonsterMovementComponent.h"
#include "GameFramework/Character.h"
#include "Engine.h"

#pragma region FSavedMove_Monster

bool FSavedMove_Monster::CanCombineWith(const FSavedMovePtr& newMove, ACharacter* inCharacter, float maxDelta) const
{
	if (bSavedRequestMaxWalkSpeedChange != ((FSavedMove_Monster*)&newMove)->bSavedRequestMaxWalkSpeedChange)
	{
		return false;
	}

	if (bSavedRequestOverrideMovementMode != ((FSavedMove_Monster*)&newMove)->bSavedRequestOverrideMovementMode)
	{
		return false;
	}

	return Super::CanCombineWith(newMove, inCharacter, maxDelta);
}

void FSavedMove_Monster::Clear()
{
	Super::Clear();

	bSavedRequestMaxWalkSpeedChange = false;
	bSavedRequestOverrideMovementMode = false;
}

uint8 FSavedMove_Monster::GetCompressedFlags() const
{
	uint8 result = Super::GetCompressedFlags();

	if (bSavedRequestMaxWalkSpeedChange)
	{
		result |= FLAG_Custom_0;
	}

	if (bSavedRequestOverrideMovementMode)
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
		bSavedRequestOverrideMovementMode = movement->bRequestOverrideMovementMode;
		SavedAxisDirection = movement->AxisDirection;
		/*bSavedDodge = movement->bDodge;
		savedDodgeDirection = movement->dodgeDirection;*/
	}
}

void FSavedMove_Monster::PrepMoveFor(ACharacter* character)
{
	Super::PrepMoveFor(character);

	UMonsterMovementComponent* movement = Cast<UMonsterMovementComponent>(character->GetCharacterMovement());

	if (movement)
	{
		movement->AxisDirection = SavedAxisDirection;
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


#pragma region NetworkPrediciton
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

	if (bRequestOverrideMovementMode)
	{
		bRequestOverrideMovementMode = false;
		if (NewMovementMode == EMovementMode::MOVE_Custom)
		{
			// Only dodge for now
			RemainingDodgeDistance = DodgeDistance;
		}

		SetMovementMode(NewMovementMode, NewCustomMode);
	}
}


void UMonsterMovementComponent::UpdateFromCompressedFlags(uint8 Flags)
{
	Super::UpdateFromCompressedFlags(Flags);

	bRequestWalkSpeedChange = (Flags & FSavedMove_Character::FLAG_Custom_0) != 0;
	bRequestOverrideMovementMode = (Flags & FSavedMove_Character::FLAG_Custom_1) != 0;
}

FNetworkPredictionData_Client* UMonsterMovementComponent::GetPredictionData_Client() const
{
	check(PawnOwner != NULL);
	check(PawnOwner->GetLocalRole() < ROLE_Authority);

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


void UMonsterMovementComponent::PhysCustom(float DeltaTime, int32 Iterations)
{
	Super::PhysCustom(DeltaTime, Iterations);

	switch (CustomMovementMode)
	{
		case (uint8)ECustomMovement::CM_DODGE:
			DodgeTick(DeltaTime);
			break;
		default:
			break;
	}
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

void UMonsterMovementComponent::SetMaxWalkSpeed(float InWalkSpeed)
{
	if (PawnOwner->IsLocallyControlled())
	{
		NewMaxWalkSpeed = InWalkSpeed; // client side
		Server_SetMaxWalkSpeed(NewMaxWalkSpeed); // server side
	}
	
	bRequestWalkSpeedChange = true;

}
#pragma endregion

bool UMonsterMovementComponent::Server_OverrideMovementMode_Validate(const EMovementMode InNewMovementMode, uint8 InNewCustomMode, FVector InAxisDirection)
{
	return true;
}

void UMonsterMovementComponent::Server_OverrideMovementMode_Implementation(const EMovementMode InNewMovementMode, uint8 InNewCustomMode, FVector InAxisDirection)
{
	NewMovementMode = InNewMovementMode;
	NewCustomMode = InNewCustomMode;
	AxisDirection = InAxisDirection;
}

void UMonsterMovementComponent::OverrideMovementMode(EMovementMode InNewMovementMode, uint8 InNewCustomMode, FVector InAxisDirection)
{
	if(PawnOwner->IsLocallyControlled())
	{
		NewMovementMode = InNewMovementMode;
		NewCustomMode = InNewCustomMode;
		AxisDirection = InAxisDirection;
		AxisDirection.Normalize();
		Server_OverrideMovementMode(NewMovementMode, NewCustomMode, AxisDirection);
	}

	bRequestOverrideMovementMode = true;
}

#pragma region Dodge
void UMonsterMovementComponent::DodgeTick(float DeltaTime)
{
	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, "Dodging");

	FindFloor(UpdatedComponent->GetComponentLocation(), CurrentFloor, false);
	AdjustFloorHeight();
	SetBaseFromFloor(CurrentFloor);

	if (CurrentFloor.bBlockingHit)
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Yellow, "Dodging");
		Velocity = GetDodgeVelocity();

		float dodgeDelta = Velocity.Size() * DeltaTime;
		dodgeDelta = dodgeDelta < RemainingDodgeDistance ? dodgeDelta : RemainingDodgeDistance;

		// To be swapped with dodge delta
		MoveAlongFloor(Velocity, DeltaTime);
		RemainingDodgeDistance -= dodgeDelta;
	}

	if (RemainingDodgeDistance <= 0.0f)
	{
		SetMovementMode(EMovementMode::MOVE_Walking);
		Velocity = FVector::ZeroVector;
	}
}

void UMonsterMovementComponent::Dodge()
{
	if (PawnOwner->IsLocallyControlled())
	{
		OverrideMovementMode(EMovementMode::MOVE_Custom, (uint8)ECustomMovement::CM_DODGE, PawnOwner->GetLastMovementInputVector());
		/*DodgeDirection = PawnOwner->GetLastMovementInputVector();
		if (DodgeDirection.Size() < 0.1f)
		{
			DodgeDirection = PawnOwner->GetActorForwardVector() * -1;
		}

		DodgeDirection.Normalize();

		RemainingDodgeDistance = DodgeDistance;

		Server_Dodge(DodgeDirection);
		SetMovementMode(EMovementMode::MOVE_Custom, (uint8)ECustomMovement::CM_DODGE);*/
	}

	//bDodge = true;
}

FVector UMonsterMovementComponent::GetDodgeVelocity()
{
	return AxisDirection * DodgeSpeed;
}

bool UMonsterMovementComponent::Server_Dodge_Validate(FVector InDodgeDirection)
{
	return true;
}

void UMonsterMovementComponent::Server_Dodge_Implementation(FVector InDodgeDirection)
{
	DodgeDirection = InDodgeDirection;
	SetMovementMode(EMovementMode::MOVE_Custom, (uint8)ECustomMovement::CM_DODGE);
}
#pragma endregion