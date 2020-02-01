// Copyright of Honeycomb Studio

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "TeamInterface.generated.h"

UENUM(BlueprintType)		//"BlueprintType" is essential to include
enum class ETeamEnum : uint8
{
	TE_NEUTRAL	UMETA(DisplayName="NEUTRAL"),
	TE_RED		UMETA(DisplayName = "RED"),
	TE_GREEN	UMETA(DisplayName = "GREEN")
};


// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UTeamInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class HIVE_API ITeamInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual bool AssignTeam(ETeamEnum InTeam) { return false; };

	virtual ETeamEnum GetTeam() { return ETeamEnum::TE_NEUTRAL; };
};
