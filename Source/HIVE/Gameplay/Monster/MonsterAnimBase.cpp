// Copyright of Honeycomb Studio


#include "Components/ShapeComponent.h"
#include "MonsterAnimBase.h"
#include "MonsterBase.h"
#include "Kismet/KismetMathLibrary.h"

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

	if (AttackRegister != EAttackType::AT_NULL)
	{

	}

}

void UMonsterAnimBase::RegisterAttack(EAttackType InNewAttack)
{
	if (!OwningMonster->IsLocallyControlled()) { return; }

	AttackRegister = InNewAttack;
	Server_RegisterAttack(InNewAttack);
}

bool UMonsterAnimBase::Server_RegisterAttack_Validate(EAttackType InNewAttack)
{
	return true;
}

void UMonsterAnimBase::Server_RegisterAttack(EAttackType InNewAttack)
{
	AttackRegister = InNewAttack;
}

void UMonsterAnimBase::ToggleHitbox(UShapeComponent* InHitBox, ECollisionEnabled InEnable)
{
	InHitBox->SetCollisionEnabled(InEnable);
}

void UMonsterAnimBase::ToggleHitbox(TArray<UShapeComponent*> InHitBoxes, ECollisionEnabled InEnable)
{
	for (int i = 0; i < InHitBoxes.Num(); i++)
	{
		InHitBoxes[i]->SetCollisionEnabled(InEnable);
	}
}
