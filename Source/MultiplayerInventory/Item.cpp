// Fill out your copyright notice in the Description page of Project Settings.


#include "Item.h"
#include "Components/StaticMeshComponent.h"
#include "MultiplayerInventoryCharacter.h"


// Sets default values
AItem::AItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;
	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh"));
	RootComponent = ItemMesh;
	ItemData._ItemClass = StaticClass();
}

// Called when the game starts or when spawned
void AItem::BeginPlay()
{
	Super::BeginPlay();
	
}

void AItem::Interact(class AMultiplayerInventoryCharacter* character)
{
	if (HasAuthority() && character)
	{
		character->AddItemToArray(ItemData);
		//AddItemToInventoryWidget(ItemData);
		Destroy();
	}
}

void AItem::Use(class AMultiplayerInventoryCharacter* character)
{
	if (character)
	{
		UE_LOG(LogTemp, Warning, TEXT("can be used"));
	}
}



