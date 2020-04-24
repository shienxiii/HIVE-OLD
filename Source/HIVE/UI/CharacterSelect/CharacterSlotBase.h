// Copyright of Honeycomb Studio

#pragma once

#include "CoreMinimal.h"
#include "Components/Button.h"
#include "UObject/UObjectGlobals.h"
#include "CharacterSlotBase.generated.h"

class AMonsterBase;
class UCharacterSelectBase;
/**
 * 
 */
UCLASS()
class HIVE_API UCharacterSlotBase : public UButton
{
	GENERATED_BODY()

protected:
	UCharacterSelectBase* CharacterSelector;

	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Character, meta = (AllowPrivateAccess = "true", DisplayThumbnail = "true", DisplayName = "CharacterImage", DisallowedClasses = "MediaTexture"))
		UTexture* CharacterImage;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = Character, meta = (AllowPrivateAccess = "true", DisplayThumbnail = "true", DisplayName = "PlayerCharacter"))
		TSubclassOf<AMonsterBase> Monster;

public:
	UCharacterSlotBase(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintCallable)
		void SyncButton(UCharacterSelectBase* InCharacterSelector, UMaterialInterface* NormalMat, UMaterialInterface* HoverMat, UMaterialInterface* ClickMat);

	UFUNCTION()
	void CharacterSelectedEvent();

	//UObject* GetPlayerImage() { return PlayerImage; }
	TSubclassOf<AMonsterBase> GetMonsterClass() { return Monster; }
};
