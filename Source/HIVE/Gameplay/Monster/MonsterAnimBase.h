// Copyright of Honeycomb Studio

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "GameFramework/Character.h"
#include "MonsterAnimBase.generated.h"

class AMonsterBase;

UENUM(BlueprintType)
enum class EAttackType : uint8
{
	AT_NULL		UMETA(DisplayName = "NULL"),
	AT_LIGHT	UMETA(DisplayName = "Light Attack"),
	AT_HEAVY	UMETA(DisplayName = "Heavy Attack")
};

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

	UPROPERTY(BlueprintReadWrite)
		EAttackType AttackRegister = EAttackType::AT_NULL;

	UPROPERTY()
		bool bCanBeginNextAttack = true;
public:
	virtual void NativeBeginPlay() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	virtual void RegisterAttack(EAttackType InNewAttack);
	
	void ToggleHitbox(UShapeComponent* InHitBox, ECollisionEnabled::Type InEnable);
	void ToggleHitbox(TArray<UShapeComponent*> InHitBoxes, ECollisionEnabled::Type InEnable);
};
