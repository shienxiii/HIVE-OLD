// Copyright of Rabbit Games

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "MonsterMovementComponent.generated.h"


class FSavedMove_Monster : public FSavedMove_Character
{
protected:
	// Walk Speed update
	uint8 bSavedRequestMaxWalkSpeedChange : 1;

	// Override Movement Mode
	uint8 bSavedRequestOverrideMovementMode : 1;
	EMovementMode SavedMovementMode;
	uint8 SavedCustomMovement;
	FVector SavedAxisDirection;

public:
	typedef FSavedMove_Character Super;

	virtual bool CanCombineWith(const FSavedMovePtr& newMove, ACharacter* inCharacter, float maxDelta) const override;
	virtual void Clear() override;
	virtual uint8 GetCompressedFlags() const override;
	virtual void SetMoveFor(ACharacter* character, float inDeltaTime, FVector const& newAccel, class FNetworkPredictionData_Client_Character& clientData) override;
	virtual void PrepMoveFor(class ACharacter* character) override;

};

class FNetworkPredictionData_Client_Monster : public FNetworkPredictionData_Client_Character
{
public:
	typedef FNetworkPredictionData_Client_Character Super;

	FNetworkPredictionData_Client_Monster(const UCharacterMovementComponent& clientMovement);

	virtual FSavedMovePtr AllocateNewMove() override;
};

enum class ECustomMovement : uint8
{
	CM_DODGE UMETA(DisplayName="Dodge")
};



/**
 * MovementComponent class shared by all monsters
 */
UCLASS()
class HIVE_API UMonsterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, Category = "Dodge")
		float DodgeSpeed = 5000.0f;
	UPROPERTY(EditAnywhere, Category = "Dodge")
		float DodgeDistance = 200.0f;
	FVector DodgeDirection = FVector::ZeroVector;
	float RemainingDodgeDistance;


public:
	typedef UCharacterMovementComponent Super;

	
	void OnMovementUpdated(float DeltaTime, const FVector& OldLocation, const FVector& OldVelocity); // NOTE: All update to movement component need to happen here, otherwise net correction will happen
	virtual void UpdateFromCompressedFlags(uint8 Flags) override;
	virtual class FNetworkPredictionData_Client* GetPredictionData_Client() const override;

	// Custom Movement setting
	void PhysCustom(float DeltaTime, int32 Iterations) override;
	
	
	

#pragma region MaxWalkSpeedChange
	// Set MaxWalkSpeed
	uint8 bRequestWalkSpeedChange : 1;
	float NewMaxWalkSpeed; // NOTE: Needed as an intermediate otherwise net correction will happen

	UFUNCTION(Reliable, Server, WithValidation)
		void Server_SetMaxWalkSpeed(const float InWalkSpeed);

	UFUNCTION(BlueprintCallable, Category = "Walk Speed")
		void SetMaxWalkSpeed(float InWalkSpeed);
#pragma endregion

#pragma region OverrideMovementMode
	uint8 bRequestOverrideMovementMode : 1;
	EMovementMode NewMovementMode;
	uint8 NewCustomMode;
	FVector AxisDirection;

	UFUNCTION(Reliable, Server, WithValidation)
		void Server_OverrideMovementMode(const EMovementMode InNewMovementMode, uint8 InNewCustomMode, FVector InAxisDirection = FVector::ZeroVector);

	UFUNCTION(BlueprintCallable, Category = "Override Movement Mode")
		void OverrideMovementMode(EMovementMode InNewMovementMode, uint8 InNewCustomMode, FVector InAxisDirection = FVector::ZeroVector);
#pragma endregion

#pragma region Dodge
	void DodgeTick(float DeltaTime);
	void Dodge();
	FVector GetDodgeVelocity();

	UFUNCTION(Reliable, Server, WithValidation)
		void Server_Dodge(FVector InDodgeDirection);
#pragma endregion

};
