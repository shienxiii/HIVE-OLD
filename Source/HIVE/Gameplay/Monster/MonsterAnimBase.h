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
	UPROPERTY(BlueprintReadOnly)
		AMonsterBase* OwningMonster = NULL;

	UPROPERTY(BlueprintReadOnly)
		float ForwardAxis = 0.0f;
	UPROPERTY(BlueprintReadOnly)
		float RightAxis = 0.0f;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		UAnimMontage* MeleeMontage = NULL;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		UAnimMontage* DeathMontage = NULL;



	UPROPERTY()
		bool bCanBeginNextAttack = true;

	UPROPERTY(BlueprintReadOnly)
		bool bIsAlive = true;

public:
	virtual void NativeBeginPlay() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	
	void ToggleHitbox(UShapeComponent* InHitBox, ECollisionEnabled::Type InEnable);
	void ToggleHitbox(TArray<UShapeComponent*> InHitBoxes, ECollisionEnabled::Type InEnable);

	UFUNCTION(BlueprintCallable)
		void PlayDeathMontage();

	UFUNCTION(BlueprintPure)
		bool IsAlive() { return bIsAlive; }

	UFUNCTION(BlueprintNativeEvent)
		void OnLightAttack();

	UFUNCTION(BlueprintNativeEvent)
		void OnHeavyAttack();
};
