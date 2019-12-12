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

	if (bSavedDodge != ((FSavedMove_Monster*)&newMove)->bSavedDodge)
	{
		return false;
	}

	if (savedDodgeDirection != ((FSavedMove_Monster*)&newMove)->savedDodgeDirection)
	{
		return false;
	}

	return Super::CanCombineWith(newMove, inCharacter, maxDelta);
}

void FSavedMove_Monster::Clear()
{
	Super::Clear();

	bSavedRequestMaxWalkSpeedChange = false;
	bSavedDodge = false;
	savedDodgeDirection = FVector::ZeroVector;
}

uint8 FSavedMove_Monster::GetCompressedFlags() const
{
	uint8 result = Super::GetCompressedFlags();

	if (bSavedRequestMaxWalkSpeedChange)
	{
		result |= FLAG_Custom_0;
	}

	if (bSavedDodge)
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
		bSavedDodge = movement->bDodge;
		savedDodgeDirection = movement->dodgeDirection;
	}
}

void FSavedMove_Monster::PrepMoveFor(ACharacter* character)
{
	Super::PrepMoveFor(character);

	UMonsterMovementComponent* movement = Cast<UMonsterMovementComponent>(character->GetCharacterMovement());

	if (movement)
	{
		movement->dodgeDirection = savedDodgeDirection;
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


void UMonsterMovementComponent::OnMovementUpdated(float deltaTime, const FVector& oldLocation, const FVector& oldVelocity)
{
	Super::OnMovementUpdated(deltaTime, oldLocation, oldVelocity); 

	if (CharacterOwner)
	{
		return;
	}

	if (bRequestWalkSpeedChange)
	{
		bRequestWalkSpeedChange = false;
		MaxWalkSpeed = newMaxWalkSpeed;
	}

	if (bDodge)
	{
		bDodge = false;

		// Only dodge when on ground
		if (IsMovingOnGround())
		{
			dodgeDirection.Normalize();
			FVector dodgeVelocity = dodgeDirection * dodgeStrength;
			dodgeVelocity.Z = 0.0f;

			Launch(dodgeVelocity);
		}
	}
}


void UMonsterMovementComponent::UpdateFromCompressedFlags(uint8 flags)
{
	Super::UpdateFromCompressedFlags(flags);

	bRequestWalkSpeedChange = (flags & FSavedMove_Character::FLAG_Custom_0) != 0;
	bDodge = (flags & FSavedMove_Character::FLAG_Custom_1) != 0;
}

FNetworkPredictionData_Client* UMonsterMovementComponent::GetPredictionData_Client() const
{
	check(PawnOwner != NULL);
	check(PawnOwner->Role < ROLE_Authority);

	if (!ClientPredictionData)
	{
		UMonsterMovementComponent* mutableThis = const_cast<UMonsterMovementComponent*> (this);

		mutableThis->ClientPredictionData = new FNetworkPredictionData_Client_Monster(*this);
		mutableThis->ClientPredictionData->MaxSmoothNetUpdateDist = 92.0f;
		mutableThis->ClientPredictionData->NoSmoothNetUpdateDist = 140.0f;
	}

	return ClientPredictionData;
}


bool UMonsterMovementComponent::Server_SetMaxWalkSpeed_Validate(const float newWalkSpeed)
{
	return true;
}

void UMonsterMovementComponent::Server_SetMaxWalkSpeed_Implementation(const float newWalkSpeed)
{
	newMaxWalkSpeed = newWalkSpeed;
}

void UMonsterMovementComponent::SetMaxWalkSpeed(float newWalkSpeed)
{
	if (PawnOwner->IsLocallyControlled())
	{
		newMaxWalkSpeed = newWalkSpeed; // client side
		newMaxWalkSpeed;
		Server_SetMaxWalkSpeed(newMaxWalkSpeed); // server side
	}
	
	bRequestWalkSpeedChange = true;

}


bool UMonsterMovementComponent::Server_DodgeDirection_Validate(const FVector& dodgeDir)
{
	return true;
}

void UMonsterMovementComponent::Server_DodgeDirection_Implementation(const FVector& dodgeDir)
{
	dodgeDirection = dodgeDir;
}

void UMonsterMovementComponent::Dodge()
{
	if (PawnOwner->IsLocallyControlled())
	{
		dodgeDirection = PawnOwner->GetLastMovementInputVector();
		Server_DodgeDirection(dodgeDirection);
	}

	bDodge = true;
}