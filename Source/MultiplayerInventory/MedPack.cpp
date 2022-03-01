// Fill out your copyright notice in the Description page of Project Settings.


#include "MedPack.h"
#include "MultiplayerInventoryCharacter.h"

AMedPack::AMedPack()
{
	HealthValue = 10.0f;
	ItemData._ItemClass = StaticClass();
}

void AMedPack::Use(class AMultiplayerInventoryCharacter* Character)
{
	if (Character)
	{
		Character->AddHealth(HealthValue);
	}
}