// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/UserDefinedStruct.h"
#include "ItemDataStruct.generated.h"

USTRUCT(BlueprintType)
struct FItemData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<class AItem> _ItemClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UTexture2D * _itemImage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float cost;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float amount; // amount to add or remove
};

UCLASS()
class MULTIPLAYERINVENTORY_API UItemDataStruct : public UUserDefinedStruct
{
	GENERATED_BODY()
	
};



