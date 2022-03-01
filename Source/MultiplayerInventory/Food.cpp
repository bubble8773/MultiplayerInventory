// Fill out your copyright notice in the Description page of Project Settings.


#include "Food.h"
#include "MultiplayerInventoryCharacter.h"

AFood::AFood()
{
	RemoveHungerValue = 30.0f;
	ItemData._ItemClass = StaticClass();
}
void AFood::Use(AMultiplayerInventoryCharacter * Character)
{
	if (Character)
	{
		Character->RemoveHunger(RemoveHungerValue);
	}
}
