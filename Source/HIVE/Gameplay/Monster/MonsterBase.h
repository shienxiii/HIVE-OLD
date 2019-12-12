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
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Stats", meta = (DisplayName = "Health", ClampMin = "0", UIMin = "0"))float health = 100.0f;

public:
	// Sets default values for this character's properties
	AMonsterBase(const FObjectInitializer& ObjectInitializer);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void PostInitializeComponents() override;


	// Returns the character movement component type defined to a UMonsterMovementComponent
	UFUNCTION(BlueprintPure) UMonsterMovementComponent* GetMonsterMovementComponent() { return (UMonsterMovementComponent*)GetCharacterMovement(); }

};
