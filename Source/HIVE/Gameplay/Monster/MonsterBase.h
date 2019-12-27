// Copyright of Honeycomb Studio

#pragma once

#include "CoreMinimal.h"
#include "MonsterMovementComponent.h"
#include "GameFramework/Character.h"
#include "MonsterBase.generated.h"

UCLASS()
class HIVE_API AMonsterBase : public ACharacter
{
	GENERATED_BODY()

protected:
	// Lock On
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = LockOn)
		float LockOnRange = 5000.0f;
	UPROPERTY(Replicated, BlueprintReadOnly, VisibleAnywhere, Category = LockOn)
		AActor* CurrentTarget = nullptr;

	UPROPERTY(Replicated, BlueprintReadOnly, VisibleAnywhere, Category = "Stats", meta = (DisplayName = "Health", ClampMin = "0", UIMin = "0"))
		float Health = 100.0f;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Stats", meta = (DisplayName = "Dodge Strength", ClampMin = "0", UIMin = "0"))
		float DodgeStrength = 1500.0f;
public:
	// Sets default values for this character's properties
	AMonsterBase(const FObjectInitializer& ObjectInitializer);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	// Called by Tick() on every frame
	void LockedOnTick(float DeltaTime);

#pragma region Input
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Axis Input
	virtual void MoveForward(float inAxis);
	virtual void MoveRight(float inAxis);
	FRotator GetViewRotator();

	UFUNCTION(BlueprintCallable) void ExecuteDodge();
#pragma endregion
	// Returns the character movement component type defined to a UMonsterMovementComponent
	UFUNCTION(BlueprintPure) UMonsterMovementComponent* GetMonsterMovementComponent() { return (UMonsterMovementComponent*)GetCharacterMovement(); }


#pragma region LockOn
	void ToggleLockOn();
	/**
	 * Called when there is a change in the currentTarget variable in this AMonsterBase instance so the server can update it
	 *
	 * @param Target AActor to set currentTarget to, default is null
	 */
	UFUNCTION(Server, Reliable, WithValidation) void Server_SetLockOnTarget(AActor* Target = nullptr);
	/**
	 * Get the closest target this monster can lock on to
	 */
	UFUNCTION(BlueprintPure) AActor* GetLockOnTarget();
	/**
	 * Find all possible lock on target in the level, filtering self and any within the same time
	 *
	 * return All eligible lock on target
	 */
	TArray<AActor*> GetPotentialLockOnTargets();
#pragma endregion


#pragma region Networking
	void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;
#pragma endregion

};
