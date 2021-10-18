// Copyright Epic Games, Inc. All Rights Reserved.

#include "Grappling_HookCharacter.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "PlayerWeapon.h"
#include "BioLegs.h"
#include "Grapple_Hook.h"
#include "Action_Interface.h"
#include "DrawDebugHelpers.h"
#include "Item.h"
#include "InventoryComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"

//////////////////////////////////////////////////////////////////////////
// AGrappling_HookCharacter

AGrappling_HookCharacter::AGrappling_HookCharacter()
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
	FollowCamera->bUsePawnControlRotation = true; // Camera does not rotate relative to arm

	// Create Inventory
	Inventory = CreateDefaultSubobject<UInventoryComponent>(TEXT("Inventory"));
	Inventory->Capacity = 20;

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)

	//overlapped component
	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &AGrappling_HookCharacter::OnOverlapBegin);
}

//////////////////////////////////////////////////////////////////////////
// Input

void AGrappling_HookCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	//Throw Disc
	PlayerInputComponent->BindAction("ThrowDisc", IE_Pressed, this, &AGrappling_HookCharacter::ThrowDisc);
	//Grapple Dsic
	PlayerInputComponent->BindAction("GrappleDisc", IE_Pressed, this, &AGrappling_HookCharacter::Grapple);
	//Shoot and Skill
	PlayerInputComponent->BindAction("Shoot", IE_Pressed, this, &AGrappling_HookCharacter::Shoot);
	PlayerInputComponent->BindAction("Skill", IE_Pressed, this, &AGrappling_HookCharacter::Skill);
	//Glide and stop glide
	PlayerInputComponent->BindAction("Glide", IE_Pressed, this, &AGrappling_HookCharacter::Glide);
	PlayerInputComponent->BindAction("Glide", IE_Released, this, &AGrappling_HookCharacter::StopGlide);
	//Collect Resources
	PlayerInputComponent->BindAction("Collect", IE_Pressed, this, &AGrappling_HookCharacter::CollectResource);
	
	
	PlayerInputComponent->BindAxis("MoveForward", this, &AGrappling_HookCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AGrappling_HookCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AGrappling_HookCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AGrappling_HookCharacter::LookUpAtRate);

	// handle touch devices
	PlayerInputComponent->BindTouch(IE_Pressed, this, &AGrappling_HookCharacter::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &AGrappling_HookCharacter::TouchStopped);

}

void AGrappling_HookCharacter::BeginPlay()
{
	Super::BeginPlay();
	if(PlayerWeaponClass)
	{
		PlayerWeapon = GetWorld()->SpawnActor<APlayerWeapon>(PlayerWeaponClass);
	}
}

void AGrappling_HookCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if(Health <= 0)
	{
		GameOver = true;
	}
	Start = GetFollowCamera()->GetComponentLocation() + GetFollowCamera()->GetForwardVector() * 150;
	End = Start + GetFollowCamera()->GetForwardVector() * 3000;
	IsHit = GetWorld()->LineTraceSingleByChannel(OutHit, Start, End, ECollisionChannel::ECC_Visibility);
	//Check if the player can grapple on surface and change UI element accordingly
	CanGrapple(IsHit);
	//DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 1, 0,1);
}

void AGrappling_HookCharacter::ThrowDisc()
{
	if(BioLegs)
	{
		FVector SpawnLoc =( GetActorLocation() + FVector(0,0,100)) + GetFollowCamera()->GetForwardVector() * 150;
		Disc = GetWorld()->SpawnActor<ABioLegs>(BioLegs, SpawnLoc, (OutHit.TraceEnd - SpawnLoc).Rotation());
	}
}

void AGrappling_HookCharacter::CanGrapple(bool Ishit)
{
	if(Ishit)
		if(OutHit.GetActor() && OutHit.GetActor()->bGenerateOverlapEventsDuringLevelStreaming)
			CanGrappleToSurface = true;
		else
			CanGrappleToSurface = false;
	else
		CanGrappleToSurface = false;
}

void AGrappling_HookCharacter::Grapple()
{
	if(GrappleHooked && Grapple_Hook)
	{
		GrappleHooked = false;
		Grapple_Hook->hook = false;
		Grapple_Hook->Destroy();
	}
	if(IsHit)
	{
		if(OutHit.GetActor())
		{			
			if(GrappleHook_Class && OutHit.GetActor()->bGenerateOverlapEventsDuringLevelStreaming)
			{
				FVector SpawnLoc = (GetFollowCamera()->GetForwardVector() * 150) + (GetActorLocation() + FVector(0,0,100));
				Grapple_Hook = GetWorld()->SpawnActor<AGrapple_Hook>(GrappleHook_Class, SpawnLoc, (OutHit.Location - SpawnLoc).Rotation());
				Grapple_Hook->SurfaceNormal = OutHit.ImpactNormal;
				GrappleHooked = true;
			}
		}
	}
}

void AGrappling_HookCharacter::Shoot()
{
	if(PlayerWeaponClass)
	{
		PlayerWeapon->Shoot();
	}
}

void AGrappling_HookCharacter::Skill()
{
	if(PlayerWeaponClass)
	{
		PlayerWeapon->Skill();	
	}
}

void AGrappling_HookCharacter::Glide()
{
	GetCharacterMovement()->GravityScale = 0.40;
}

void AGrappling_HookCharacter::StopGlide()
{
	GetCharacterMovement()->GravityScale = 1;
}

void AGrappling_HookCharacter::CollectResource()
{
	if(OverlappingActor)
	{
		if(OverlappingActor->GetClass()->ImplementsInterface(UAction_Interface::StaticClass()))
		{
			IAction_Interface::Execute_Actions(OverlappingActor);
		}
	}
}

void AGrappling_HookCharacter::UseItem(class UItem* Item)
{
	if(Item)
	{
		Item->Use(this);
		Item->OnUse(this); //bp event
	}
}

// overlap event
void AGrappling_HookCharacter::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* Other,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(Other && (Other != this))
	{
		OverlappingActor = Other;
		if(OverlappingActor->GetClass()->ImplementsInterface(UPickup_interface::StaticClass()))
		{
			IPickup_interface::Execute_Pickups(OverlappingActor);
		}
	}
}


void AGrappling_HookCharacter::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
		Jump();
}

void AGrappling_HookCharacter::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
		StopJumping();
}

void AGrappling_HookCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AGrappling_HookCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void AGrappling_HookCharacter::MoveForward(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		if (GetCharacterMovement()->MovementMode == MOVE_Swimming) {
			AddMovementInput(GetFollowCamera()->GetForwardVector(), Value);
		}
		else {
			// find out which way is forward
			const FRotator Rotation = Controller->GetControlRotation();
			const FRotator YawRotation(0, Rotation.Yaw, 0);

			// get forward vector
			const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
			AddMovementInput(Direction, Value);
		}
	}
}

void AGrappling_HookCharacter::MoveRight(float Value)
{
	if ( (Controller != nullptr) && (Value != 0.0f) )
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
