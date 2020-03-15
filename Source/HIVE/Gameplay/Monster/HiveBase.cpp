// Copyright of Honeycomb Studio


#include "HiveBase.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "HIVE/System/GameMode/GM_HiveWar.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

// Sets default values
AHiveBase::AHiveBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USphereComponent>(TEXT("Root"));
	Root->SetSphereRadius(100.0f);
	Root->SetCollisionProfileName(FName("Pawn"));
	SetRootComponent(Root);


	HurtBox = CreateDefaultSubobject<USphereComponent>(TEXT("HurtBox"));
	HurtBox->SetSphereRadius(100.0f);
	HurtBox->SetCollisionProfileName(FName("HurtBox"));
	HurtBox->SetupAttachment(Root);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(Root);

	SetReplicates(true);
	SetReplicatingMovement(true);
	bAlwaysRelevant = true;
	bOnlyRelevantToOwner = false;
	bNetLoadOnClient = true;
}

// Called when the game starts or when spawned
void AHiveBase::BeginPlay()
{
	Super::BeginPlay();
	
	if (GetLocalRole() != ENetRole::ROLE_Authority) { return; }
	AGameModeBase* levelGameMode = UGameplayStatics::GetGameMode(this);
	GM = Cast<AGM_HiveWar>(levelGameMode);

	check(GM);
}

// Called every frame
void AHiveBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

float AHiveBase::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Health -= DamageAmount;

	if (Health <= 0.0f)
	{
		GM->GameOver(this);
	}

	return DamageAmount;
}

void AHiveBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AHiveBase, Health);
	DOREPLIFETIME(AHiveBase, MaxHealth);
}

