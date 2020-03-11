// Copyright of Honeycomb Studio


#include "HiveBase.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Net/UnrealNetwork.h"

// Sets default values
AHiveBase::AHiveBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USphereComponent>(TEXT("RootComponent"));
	Root->SetSphereRadius(100.0f);
	Root->SetCollisionProfileName(FName("HurtBox"));
	this->SetRootComponent(Root);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(Root);
}

// Called when the game starts or when spawned
void AHiveBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AHiveBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AHiveBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AHiveBase, Health);
	DOREPLIFETIME(AHiveBase, MaxHealth);
}

