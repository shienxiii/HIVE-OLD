// Copyright of Honeycomb Studio

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "MonsterBase.h"
#include "GameFramework/Character.h"
#include "MonsterAnimBase.generated.h"


/**
 * 
 */
UCLASS()
class HIVE_API UMonsterAnimBase : public UAnimInstance
{
	GENERATED_BODY()
	
protected:
	AMonsterBase* OwningMonster = NULL;

	UPROPERTY(BlueprintReadOnly)
		float ForwardAxis = 0.0f;
	UPROPERTY(BlueprintReadOnly)
		float RightAxis = 0.0f;

	UPROPERTY()
		bool bCanBeginNextAttack = true;
public:
	virtual void NativeBeginPlay() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	
	void ToggleHitbox(UShapeComponent* InHitBox, ECollisionEnabled::Type InEnable);
	void ToggleHitbox(TArray<UShapeComponent*> InHitBoxes, ECollisionEnabled::Type InEnable);

	UFUNCTION(BlueprintPure)
		EAttackType GetMonsterAttack();
};
