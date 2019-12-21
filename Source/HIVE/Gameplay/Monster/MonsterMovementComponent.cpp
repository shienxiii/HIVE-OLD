// Copyright of Rabbit Games


#include "MonsterMovementComponent.h"
#include "GameFramework/Character.h"

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
}

uint8 FSavedMove_Monster::GetCompressedFlags() const
{
	uint8 result = Super::GetCompressedFlags();

	if (bSavedRequestMaxWalkSpeedChange)
	{
		result |= FLAG_Custom_0;
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
	}
}

void FSavedMove_Monster::PrepMoveFor(ACharacter* character)
{
	Super::PrepMoveFor(character);

	UMonsterMovementComponent* movement = Cast<UMonsterMovementComponent>(character->GetCharacterMovement());

	if (movement)
	{
		//movement->DodgeDirection = savedDodgeDirection;
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
}


void UMonsterMovementComponent::UpdateFromCompressedFlags(uint8 Flags)
{
	Super::UpdateFromCompressedFlags(Flags);

	bRequestWalkSpeedChange = (Flags & FSavedMove_Character::FLAG_Custom_0) != 0;
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

void UMonsterMovementComponent::PhysCustom(float DeltaTime, int32 Iterations)
{
	Super::PhysCustom(DeltaTime, Iterations);

	switch (CustomMovementMode)
	{
		case (uint8)ECustomMovement::CM_DODGE:
			//DodgeTick(DeltaTime);
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

void UMonsterMovementComponent::Dodge()
{
	if (PawnOwner->IsLocallyControlled())
	{
		DodgeDirection = PawnOwner->GetLastMovementInputVector();
		DodgeDirection.Normalize();

		SetMovementMode(EMovementMode::MOVE_Custom, (uint8)ECustomMovement::CM_DODGE);
	}
}


//void UMonsterMovementComponent::Dodge()
//{
//	if (PawnOwner->IsLocallyControlled())
//	{
//		DodgeDirection = PawnOwner->GetLastMovementInputVector();
//		Server_DodgeDirection(DodgeDirection);
//		SetMovementMode(EMovementMode::MOVE_Custom, (uint8)ECustomMovement::CM_DODGE);
//	}
//
//	//bDodge = true;
//}