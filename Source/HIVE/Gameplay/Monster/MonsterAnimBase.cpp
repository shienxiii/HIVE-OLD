// Copyright of Honeycomb Studio


#include "MonsterAnimBase.h"
#include "Components/ShapeComponent.h"
#include "MonsterBase.h"
#include "Kismet/KismetMathLibrary.h"
#include "Engine/Engine.h"

void UMonsterAnimBase::NativeBeginPlay()
{
	// Do not allow any animation that derives from this class to be used on an actor that is not derived from AMonsterBase
	OwningMonster = Cast<AMonsterBase>(GetOwningActor());
	check(OwningMonster);
}

void UMonsterAnimBase::NativeUpdateAnimation(float DeltaSeconds)
{
	if (!OwningMonster) { return; }

	// Check if player monster is still alive
	if (OwningMonster->GetHealthPercentRatio() <= 0.0f)
	{
		if (!bIsAlive)
		{
			return;
		}
		else
		{
			PlayDeathMontage();
			bIsAlive = false;
			return;
		}
	}

	FVector OwnerVelocity = GetOwningActor()->GetVelocity();
	FTransform OwnerTransform = GetOwningActor()->GetActorTransform();

	ForwardAxis = FVector::DotProduct(GetOwningActor()->GetActorForwardVector(), OwnerVelocity);
	RightAxis = FVector::DotProduct(GetOwningActor()->GetActorRightVector(), OwnerVelocity);

}

void UMonsterAnimBase::ToggleHitbox(UShapeComponent* InHitBox, ECollisionEnabled::Type InEnable)
{
	OwningMonster->ToggleHitbox(InHitBox, InEnable);
}

void UMonsterAnimBase::ToggleHitbox(TArray<UShapeComponent*> InHitBoxes, ECollisionEnabled::Type InEnable)
{
	OwningMonster->ToggleHitbox(InHitBoxes, InEnable);
}

EAttackType UMonsterAnimBase::GetMonsterAttack()
{
	return OwningMonster->GetAttackRegister();
}

void UMonsterAnimBase::PlayDeathMontage()
{
	if (DeathMontage)
	{
		UE_LOG(LogTemp, Warning, TEXT("Playing Death Montage"));
		Montage_Play(DeathMontage);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Death Montage not found"));
	}
}
