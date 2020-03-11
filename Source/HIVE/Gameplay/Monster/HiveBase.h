// Copyright of Honeycomb Studio

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HiveBase.generated.h"

class USphereComponent;
//class UStaticMeshComponent;

UCLASS()
class HIVE_API AHiveBase : public AActor
{
	GENERATED_BODY()
	

protected:
	UPROPERTY(Replicated)
		float Health = 100.0f;
	UPROPERTY(Replicated)
		float MaxHealth = 100.0f;

	USphereComponent* Root;
	UStaticMeshComponent* Mesh;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Sets default values for this actor's properties
	AHiveBase();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

#pragma region Networking
	/**
	 * Needs to be implemented to initialize replicated properties
	 */
	void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;
#pragma endregion
};
