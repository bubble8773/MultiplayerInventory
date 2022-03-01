// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "MultiplayerInventory/InteractableInterface.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ItemDataStruct.h"
#include "Item.generated.h"

UCLASS()
class MULTIPLAYERINVENTORY_API AItem : public AActor, public IInteractableInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItem();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:

	UPROPERTY(EditAnywhere, Category = "Mesh")
		class UStaticMeshComponent *ItemMesh;

	UPROPERTY(EditAnywhere, Category = "Data")
		FItemData ItemData;

	virtual void Interact(class AMultiplayerInventoryCharacter* character) override;
	virtual void Use(class AMultiplayerInventoryCharacter* character) override;
};
