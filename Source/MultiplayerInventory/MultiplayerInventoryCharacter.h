// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ItemDataStruct.h"
#include "MultiplayerInventoryCharacter.generated.h"

UCLASS(config=Game)
class AMultiplayerInventoryCharacter : public ACharacter
{
	GENERATED_BODY()

		/** Camera boom positioning the camera behind the character */
		UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* FollowCamera;
public:
	AMultiplayerInventoryCharacter();

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float BaseLookUpRate;

protected:

	/** Resets HMD orientation in VR. */
	void OnResetVR();

	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	/**
	 * Called via input to turn at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	/** Handler for when a touch input begins. */
	void TouchStarted(ETouchIndex::Type FingerIndex, FVector Location);

	/** Handler for when a touch input stops. */
	void TouchStopped(ETouchIndex::Type FingerIndex, FVector Location);

	void Interact();
	void Interact(FVector Start, FVector End);
	/*UFUNCTION(Server, Reliable)
	void Server_Interact(FVector Start, FVector End);*/

	UPROPERTY(BlueprintReadWrite)
		float health;
	UPROPERTY(BlueprintReadWrite)
		float hunger;

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	UFUNCTION(BlueprintImplementableEvent)
		void AddItemToInventoryWidget(FItemData Item);

	UPROPERTY(ReplicatedUsing = OnRep_InventoryItems, BlueprintReadWrite)
	TArray<FItemData> InventoryItems;
	UFUNCTION()
	void OnRep_InventoryItems();

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty> &OutLifeTimeProperty) const;

	void AddItemToArray(FItemData Item);

	UFUNCTION(BlueprintCallable)
	void UseItem(TSubclassOf<AItem> ItemClass);
	UFUNCTION(BlueprintCallable)
		void AddHealth(float Value);
	UFUNCTION(BlueprintCallable)
		void RemoveHunger(float Value);
	

};

