// Copyright of Honeycomb Studio

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraComponent.h"
#include "HIVE_ThirdPersonCamera.generated.h"

class AMonsterBase;

/**
 * Player camera that can be rotated on both the Pitch by the player and Yaw and Pitch by the camera AI during player lock on without affecting the player control
 */
UCLASS()
class HIVE_API UHIVE_ThirdPersonCamera : public UCameraComponent
{
	GENERATED_BODY()
	
protected:
	AMonsterBase* OwningPlayer = nullptr;

	float PitchRate = 180.0f;

public:
	virtual void InitializeComponent() override;
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void PlayerRotateCameraPitch(float InValue);
	void SetPitchSpeed(float InPitchRate) { PitchRate = InPitchRate; }

	void SetOwningPlayer(AMonsterBase* InOwningPlayer) { OwningPlayer = InOwningPlayer; }
};
