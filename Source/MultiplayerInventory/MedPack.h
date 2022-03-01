// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "MedPack.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYERINVENTORY_API AMedPack : public AItem
{
	GENERATED_BODY()
public:
	AMedPack();
	UPROPERTY(EditAnywhere)
		float HealthValue;

	virtual void Use(class AMultiplayerInventoryCharacter* Character) override;
};
