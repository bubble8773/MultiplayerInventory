// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "Food.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYERINVENTORY_API AFood : public AItem
{
	GENERATED_BODY()
	
public:
	AFood();
	UPROPERTY(EditAnywhere)
		float RemoveHungerValue;

	virtual void Use(class AMultiplayerInventoryCharacter* Character) override;
};
