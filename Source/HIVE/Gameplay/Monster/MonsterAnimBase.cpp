// Copyright of Honeycomb Studio


#include "MonsterAnimBase.h"
#include "Components/ShapeComponent.h"
#include "MonsterBase.h"
#include "Kismet/KismetMathLibrary.h"
#include "Engine/Engine.h"
#include "Net/UnrealNetwork.h"

void UMonsterAnimBase::NativeBeginPlay()
{
	// Do not allow any animation that derives from this class to be used on an actor that is not derived from AMonsterBase
	OwningMonster = Cast<AMonsterBase>(GetOwningActor());
	check(OwningMonster);
}

void UMonsterAnimBase::NativeUpdateAnimation(float DeltaSeconds)
{
	if (!GetOwningActor()) { return; }

	FVector OwnerVelocity = GetOwningActor()->GetVelocity();
	FTransform OwnerTransform = GetOwningActor()->GetActorTransform();

	ForwardAxis = FVector::DotProduct(GetOwningActor()->GetActorForwardVector(), OwnerVelocity);
	RightAxis = FVector::DotProduct(GetOwningActor()->GetActorRightVector(), OwnerVelocity);

	/*if (AttackRegister != EAttackType::AT_NULL)
	{

	}*/

}

void UMonsterAnimBase::RegisterAttack(EAttackType InNewAttack)
{
	if (!(OwningMonster->IsLocallyControlled())) { return; }

	AttackRegister = InNewAttack;
	//Server_RegisterAttack(InNewAttack);
}

bool UMonsterAnimBase::Server_RegisterAttack_Validate(EAttackType InNewAttack)
{
	return true;
}

void UMonsterAnimBase::Server_RegisterAttack_Implementation(EAttackType InNewAttack)
{
	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Cyan, TEXT("Server Register Attack"));
	AttackRegister = InNewAttack;
}

void UMonsterAnimBase::ToggleHitbox(UShapeComponent* InHitBox, ECollisionEnabled::Type InEnable)
{
	OwningMonster->ToggleHitbox(InHitBox, InEnable);
}

void UMonsterAnimBase::ToggleHitbox(TArray<UShapeComponent*> InHitBoxes, ECollisionEnabled::Type InEnable)
{
	OwningMonster->ToggleHitbox(InHitBoxes, InEnable);
}

void UMonsterAnimBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UMonsterAnimBase, AttackRegister);
}
