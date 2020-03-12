// Copyright of Honeycomb Studio

#pragma once

#include "CoreMinimal.h"
#include "HIVE/Interfaces/TeamInterface.h"
#include "GameFramework/Actor.h"
#include "HiveBase.generated.h"

class USphereComponent;
class AGM_HiveWar;
//class UStaticMeshComponent;

UCLASS()
class HIVE_API AHiveBase : public AActor, public ITeamInterface
{
	GENERATED_BODY()
	

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		AGM_HiveWar* GM;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		ETeamEnum TeamID = ETeamEnum::TE_NEUTRAL;

	UPROPERTY(Replicated, BlueprintReadOnly)
		float Health = 100.0f;
	UPROPERTY(Replicated, BlueprintReadOnly)
		float MaxHealth = 100.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		USphereComponent* Root;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
		USphereComponent* HurtBox;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
		UStaticMeshComponent* Mesh;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Sets default values for this actor's properties
	AHiveBase();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	virtual ETeamEnum GetTeam() override { return TeamID; }

#pragma region Networking
	/**
	 * Needs to be implemented to initialize replicated properties
	 */
	void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;
#pragma endregion

};
