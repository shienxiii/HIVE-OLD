// Copyright of Honeycomb Studio


#include "MonsterAnimBase.h"
#include "MonsterBase.h"
#include "Components/ShapeComponent.h"
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

	switch (OwningMonster->ConsumeAttackRegister())
	{
		case EAttackType::AT_LIGHT:
			OnLightAttack();
			break;
		case EAttackType::AT_HEAVY:
			OnHeavyAttack();
			break;
		default:
			break;
	}


	// Locomotion
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

void UMonsterAnimBase::PlayDeathMontage()
{
	if (DeathMontage)
	{
		Montage_Play(DeathMontage);

	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Death Montage not found"));
	}
}

void UMonsterAnimBase::OnLightAttack_Implementation()
{
	/**
	 * Attack animation process
	 * 1. Get attack chain(c) number
	 * 2. if c == 0, run the montage, disable attack and movement input
	 * 3. montage runs attack animation, enable necessary hitbox
	 * 4. attack animation finish, enable attack input
	 * 5a. no next input, finish recovery animation, enable movement input
	 * 5b. next input detected, repeat from 1
	 */

	if (OwningMonster->GetAttackChain() > 0)
	{
		FString section = FString("light_");
		section.Append(FString::FromInt(OwningMonster->GetAttackChain()));

		FName sectionName = FName(*section);
		Montage_SetNextSection(Montage_GetCurrentSection(), sectionName, GetCurrentActiveMontage());
	}
	else
	{
		Montage_Play(MeleeMontage);
	}

	GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Cyan, TEXT("Light Attack"));
}

void UMonsterAnimBase::OnHeavyAttack_Implementation()
{
	GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Cyan, TEXT("Heavy Attack"));
}
