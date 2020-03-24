// Copyright of Honeycomb Studio


#include "MonsterAnimBase.h"
#include "Kismet/KismetMathLibrary.h"

void UMonsterAnimBase::NativeUpdateAnimation(float DeltaSeconds)
{
	if (!GetOwningActor()) { return; }

	FVector OwnerVelocity = GetOwningActor()->GetVelocity();
	FTransform OwnerTransform = GetOwningActor()->GetActorTransform();

	ForwardAxis = FVector::DotProduct(GetOwningActor()->GetActorForwardVector(), OwnerVelocity);
	RightAxis = FVector::DotProduct(GetOwningActor()->GetActorRightVector(), OwnerVelocity);
}