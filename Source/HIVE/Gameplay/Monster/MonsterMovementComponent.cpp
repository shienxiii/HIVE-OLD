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

	if (bRequestLaunch)
	{
		bRequestLaunch = false;
		// Only dodge when on ground
		if (IsMovingOnGround())
		{
			LaunchDirection.Normalize();
			FVector dodgeVelocity = LaunchDirection * LaunchStrength;
			dodgeVelocity.Z = 0.0f;

			Launch(dodgeVelocity);
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
bool UMonsterMovementComponent::Server_LaunchMonster_Validate(FVector InLaunchDirection, float InLaunchStrength)
{
	return true;
}

void UMonsterMovementComponent::Server_LaunchMonster_Implementation(FVector InLaunchDirection, float InLaunchStrength)
{
	LaunchDirection = InLaunchDirection;
	LaunchStrength = InLaunchStrength;
}

void UMonsterMovementComponent::Client_LaunchMonster(FVector InLaunchDirection, float InLaunchStrength)
{
	if (PawnOwner->IsLocallyControlled())
	{
		LaunchDirection = InLaunchDirection;
		LaunchStrength = InLaunchStrength;
		Server_LaunchMonster(LaunchDirection, LaunchStrength);
	}

	bRequestLaunch = true;
}

#pragma endregion