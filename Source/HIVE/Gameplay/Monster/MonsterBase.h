// Copyright of Rabbit Games

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
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = LockOn) float lockOnRange = 5000.0f;
	UPROPERTY(Replicated, BlueprintReadOnly, VisibleAnywhere, Category = LockOn) AActor* currentTarget = nullptr;

	UPROPERTY(Replicated, BlueprintReadOnly, VisibleAnywhere, Category = "Stats", meta = (DisplayName = "Health", ClampMin = "0", UIMin = "0"))float health = 100.0f;

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

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void PostInitializeComponents() override;

	// Returns the character movement component type defined to a UMonsterMovementComponent
	UFUNCTION(BlueprintPure) UMonsterMovementComponent* GetMonsterMovementComponent() { return (UMonsterMovementComponent*)GetCharacterMovement(); }


	void ToggleLockOn();
	/**
	 * Called when there is a change in the currentTarget variable in this AMonsterBase instance so the server can update it
	 *
	 * @param Target AActor to set currentTarget to, default is null
	 */
	UFUNCTION(Server, Reliable, WithValidation) void Server_SetLockOnTarget(AActor* Target = nullptr);


	UFUNCTION(BlueprintPure) AActor* GetLockOnTarget();

	/**
	 * Find all possible lock on target in the level, filtering self and any within the same time
	 *
	 * return All eligible lock on target
	 */
	TArray<AActor*> GetPotentialLockOnTargets();

#pragma region Networking
	void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;
#pragma endregion

};
