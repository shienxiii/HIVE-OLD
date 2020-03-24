// Copyright of Honeycomb Studio

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
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
		float ForwardAxis = 0.0f;
	UPROPERTY(BlueprintReadOnly)
		float RightAxis = 0.0f;

public:
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
};
