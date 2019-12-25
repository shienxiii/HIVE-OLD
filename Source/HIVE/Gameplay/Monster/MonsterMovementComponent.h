// Copyright of Honeycomb Studio

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "MonsterMovementComponent.generated.h"


class FSavedMove_Monster : public FSavedMove_Character
{
protected:
	// Walk Speed update
	uint8 bSavedRequestMaxWalkSpeedChange : 1;

	// Launch Monster
	uint8 bSavedRequestLaunch : 1;
	FVector SavedLaunchDirection;
	float SavedLaunchStrength;

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


UENUM()
enum class ELaunchType : uint8
{
	LT_NULL	UMETA(DisplayName = "NULL"),
	LT_DODGE	UMETA(DisplayName = "Dodge")
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
		float DodgeStrength = 5000.0f;

	UPROPERTY(Replicated)
		ELaunchType LaunchState = ELaunchType::LT_NULL;
	
	virtual void PhysWalking(float DeltaTime, int32 Iterations) override;

public:
	typedef UCharacterMovementComponent Super;

	virtual FVector ConsumeInputVector() override;
	void OnMovementUpdated(float DeltaTime, const FVector& OldLocation, const FVector& OldVelocity); // NOTE: All update to movement component need to happen here, otherwise net correction will happen
	virtual void UpdateFromCompressedFlags(uint8 Flags) override;
	virtual class FNetworkPredictionData_Client* GetPredictionData_Client() const override;
	
	UFUNCTION(BlueprintPure) ELaunchType GetLaunchState() { return LaunchState; }
	
	

#pragma region MaxWalkSpeedChange
	uint8 bRequestWalkSpeedChange : 1;
	float NewMaxWalkSpeed;

	UFUNCTION(Reliable, Server, WithValidation)
		void Server_SetMaxWalkSpeed(const float InWalkSpeed);

	UFUNCTION(BlueprintCallable, Category = "Walk Speed")
		void Client_SetMaxWalkSpeed(float InWalkSpeed);
#pragma endregion

#pragma region LaunchCharacter
	uint8 bRequestLaunch : 1;
	FVector LaunchDirection;
	float LaunchStrength;

	UFUNCTION(Reliable, Server, WithValidation)
		void Server_LaunchMonster(FVector InLaunchDirection, float InLaunchStrength);
	
	UFUNCTION(BlueprintCallable, Category = "Launch")
		void Client_LaunchMonster(FVector InLaunchDirection, float InLaunchStrength = 1000.0f);
#pragma endregion

#pragma region Networking
	void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;
#pragma endregion

};
