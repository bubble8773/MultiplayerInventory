// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "MultiplayerInventoryCharacter.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "Item.h"
#include "Net/UnrealNetwork.h"


//////////////////////////////////////////////////////////////////////////
// AMultiplayerInventoryCharacter

AMultiplayerInventoryCharacter::AMultiplayerInventoryCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)
}

//////////////////////////////////////////////////////////////////////////
// Input

void AMultiplayerInventoryCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAxis("MoveForward", this, &AMultiplayerInventoryCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AMultiplayerInventoryCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AMultiplayerInventoryCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AMultiplayerInventoryCharacter::LookUpAtRate);

	// handle touch devices
	PlayerInputComponent->BindTouch(IE_Pressed, this, &AMultiplayerInventoryCharacter::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &AMultiplayerInventoryCharacter::TouchStopped);

	// VR headset functionality
	PlayerInputComponent->BindAction("ResetVR", IE_Pressed, this, &AMultiplayerInventoryCharacter::OnResetVR);
	
	//Pick up
	PlayerInputComponent->BindAction("PickUp", IE_Pressed, this, &AMultiplayerInventoryCharacter::Interact);
}




void AMultiplayerInventoryCharacter::OnResetVR()
{
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void AMultiplayerInventoryCharacter::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
		Jump();
}

void AMultiplayerInventoryCharacter::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
		StopJumping();
}

void AMultiplayerInventoryCharacter::Interact()
{
	//UE_LOG(LogTemp, Warning, TEXT("I can interact"));
	auto capsule = GetCapsuleComponent();
	FVector start = capsule->GetComponentLocation();
	FVector end = start + capsule->GetForwardVector()* 100.0f;
	//if (HasAuthority())
	{
		Interact(start, end);
	}
	//else
	{
		//Interact(start, end);
		//Server_Interact(start, end);
	}

}

void AMultiplayerInventoryCharacter::Interact(FVector Start, FVector End)
{
	FHitResult hitResult;

	if (GetWorld()->LineTraceSingleByChannel(hitResult, Start, End, ECC_Visibility))
	{
		if (IInteractableInterface *Interface = Cast<IInteractableInterface>(hitResult.GetActor()))
		{
			Interface->Interact(this);
		}
	}
}

//bool AMultiplayerInventoryCharacter::Server_InteractX_Validate(FVector Start, FVector End)
//{
//	return true;
//}
//
//void AMultiplayerInventoryCharacter::Server_InteractX_Implementation(FVector Start, FVector End)
//{
//	
//}
//void AMultiplayerInventoryCharacter::Server_Interact(FVector Start, FVector End)
//{
//	Interact(Start, End);
//}

void AMultiplayerInventoryCharacter::OnRep_InventoryItems()
{
	//if()
	 AddItemToInventoryWidget(InventoryItems[InventoryItems.Num()-1]);
}

void AMultiplayerInventoryCharacter::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
		
	DOREPLIFETIME_CONDITION(AMultiplayerInventoryCharacter, InventoryItems, COND_OwnerOnly);
}

void AMultiplayerInventoryCharacter::AddItemToArray(FItemData Item)
{
	if (HasAuthority())
	{
		InventoryItems.Add(Item);
		if (IsLocallyControlled())
		{
			OnRep_InventoryItems();
		}
	}
	
}

void AMultiplayerInventoryCharacter::UseItem(TSubclassOf<AItem> ItemClass)
{
	if (ItemClass)
	{
		if (AItem* itemobj = ItemClass.GetDefaultObject())
		{
			itemobj->Use(this);
		}

	}
}
void AMultiplayerInventoryCharacter::AddHealth(float Value)
{
	health += Value;
	//GetStats(hunger, health);
	UE_LOG(LogTemp, Warning, TEXT("Health: %f"), health);
}

void AMultiplayerInventoryCharacter::RemoveHunger(float Value)
{
	hunger -= Value;
	//GetStats(hunger, health);
	UE_LOG(LogTemp, Warning, TEXT("RemoveHunger: %f"), hunger);
}


void AMultiplayerInventoryCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AMultiplayerInventoryCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void AMultiplayerInventoryCharacter::MoveForward(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void AMultiplayerInventoryCharacter::MoveRight(float Value)
{
	if ( (Controller != NULL) && (Value != 0.0f) )
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}
