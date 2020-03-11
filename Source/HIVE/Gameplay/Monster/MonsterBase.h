// Copyright of Honeycomb Studio

#pragma once

#include "CoreMinimal.h"
#include "MonsterMovementComponent.h"
#include "HIVE/Interfaces/TeamInterface.h"
#include "GameFramework/Character.h"
#include "MonsterBase.generated.h"

class UBoxComponent;
class AMonsterController;
class UHIVE_ThirdPersonCamera;
class USpringArmComponent;

UCLASS()
class HIVE_API AMonsterBase : public ACharacter, public ITeamInterface
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
		UBoxComponent* HitBox;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
		UCapsuleComponent* HurtBox;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
		USpringArmComponent* CameraBoom;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
		UHIVE_ThirdPersonCamera* Camera;


	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Movement")
		float TurnRate = 90.0f;

	// Lock On
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "LockOn")
		float LockOnRange = 5000.0f;

	UPROPERTY(Replicated, BlueprintReadOnly, VisibleAnywhere, Category = "LockOn")
		AActor* CurrentTarget = nullptr;

	UPROPERTY(Replicated, BlueprintReadOnly, VisibleAnywhere, Category = "Stats", meta = (DisplayName = "Health", ClampMin = "0", UIMin = "0"))
		float Health = 100.0f;

	UPROPERTY(Replicated, BlueprintReadOnly, VisibleAnywhere, Category = "Stats", meta = (DisplayName = "Max Health", ClampMin = "0", UIMin = "0"))
		float MaxHealth = 100.0f;

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

#pragma region Input
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Axis Input
	virtual void MoveForward(float inAxis);
	virtual void MoveRight(float inAxis);
	virtual void Turn(float inAxis);
	virtual void LookUp(float inAxis);
	FRotator GetViewRotator();

	// Action Input
	UFUNCTION(BlueprintCallable)
		void LightAttack();
	UFUNCTION(BlueprintCallable)
		void HeavyAttack();
	UFUNCTION(BlueprintCallable)
		void ExecuteDodge();

#pragma endregion

	// Returns the character movement component type defined to a UMonsterMovementComponent
	UFUNCTION(BlueprintPure)
		UMonsterMovementComponent* GetMonsterMovement() { return Cast<UMonsterMovementComponent>(GetCharacterMovement()); };

	

#pragma region LockOn
	AActor* GetCurrentLockOnTarget() { return CurrentTarget; };

	void ToggleLockOn();

	/**
	 * Called when there is a change in the currentTarget variable in this AMonsterBase instance so the server can update it
	 *
	 * @param Target AActor to set currentTarget to, default is null
	 */
	UFUNCTION(Server, Reliable, WithValidation)
		void Server_SetLockOnTarget(AActor* Target = nullptr);

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


	/**
	 * Turns the control rotation to face the lock on target
	 */
	void TurnToLockOnTarget(float DeltaTime);
#pragma endregion


#pragma region Damage
	UFUNCTION(Server, Reliable, WithValidation)
		virtual void Server_ToggleHitBox();

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
#pragma endregion


#pragma region Networking
	/**
	 * Needs to be implemented to initialize replicated properties
	 */
	void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;

	// Override to perform tasks when pawn is restarted, usually possessed
	virtual void Restart() override;
#pragma endregion

#pragma region TeamInterface
	virtual ETeamEnum GetTeam() override;
#pragma endregion

	/**
	 * Get the player controller casted into AMonsterController player controller
	 *
	 * Returns null if this monster is controlled by AI or non-child of AMonsterController
	 */
	AMonsterController* GetMonsterController();

	UFUNCTION(BlueprintPure)
		float GetHealthPercentRatio() { return Health / MaxHealth; }


	UFUNCTION()
		void HitBoxOverlapEvent(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
