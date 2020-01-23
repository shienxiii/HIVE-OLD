// Copyright of Honeycomb Studio

#pragma once

#include "CoreMinimal.h"
#include "Components/Button.h"
#include "UObject/UObjectGlobals.h"
#include "CharacterSlotBase.generated.h"

class AMonsterBase;

/**
 * 
 */
UCLASS()
class HIVE_API UCharacterSlotBase : public UButton
{
	GENERATED_BODY()

protected:
	//UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Character, meta = (AllowPrivateAccess = "true", DisplayThumbnail = "true", DisplayName = "PlayerImage", AllowedClasses = "Texture,MaterialInterface,SlateTextureAtlasInterface", DisallowedClasses = "MediaTexture"))
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Character, meta = (AllowPrivateAccess = "true", DisplayThumbnail = "true", DisplayName = "PlayerImage", DisallowedClasses = "MediaTexture"))
		UTexture* PlayerImage;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = Character, meta = (AllowPrivateAccess = "true", DisplayThumbnail = "true", DisplayName = "PlayerCharacter"))
		TSubclassOf<AMonsterBase> Monster;

public:
	UCharacterSlotBase(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintCallable)
		void SyncButtonAppearance(UMaterialInterface* NormalMat, UMaterialInterface* HoverMat, UMaterialInterface* ClickMat);

	//UObject* GetPlayerImage() { return PlayerImage; }
	TSubclassOf<AMonsterBase> GetMonsterClass() { return Monster; }
};
