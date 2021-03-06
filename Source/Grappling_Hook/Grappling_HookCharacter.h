// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Pickup_interface.h"
#include "Grappling_HookCharacter.generated.h"

class ABioLegs;
class AGrapple_Hook;
class APlayerWeapon;


UCLASS(config=Game)
class AGrappling_HookCharacter : public ACharacter
{
	GENERATED_BODY()
	
	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

	/** Inventory Component */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UInventoryComponent* Inventory;

public:
	AGrappling_HookCharacter();

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

protected:

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

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

public:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

	UPROPERTY(EditDefaultsOnly, Category = "BioLegs")
	TSubclassOf<ABioLegs> BioLegs;
	ABioLegs* Disc;

	UPROPERTY(EditDefaultsOnly, Category = "GrappleHook")
	TSubclassOf<AGrapple_Hook> GrappleHook_Class;
	AGrapple_Hook* Grapple_Hook;
	bool GrappleHooked;

	UPROPERTY(EditDefaultsOnly, Category = "PlayerWeapon")
	TSubclassOf<APlayerWeapon> PlayerWeaponClass;
	APlayerWeapon* PlayerWeapon;

	UFUNCTION()
	void ThrowDisc();
	UFUNCTION()
	void Grapple();
	UFUNCTION()
	void CanGrapple(bool IsHit);
	UFUNCTION()
	void Shoot();
	UFUNCTION()
	void Skill();
	UFUNCTION()
	void Glide();
	UFUNCTION()
	void StopGlide();
	UFUNCTION()
	void CollectResource();

	//Trace
	UPROPERTY()
	FHitResult OutHit;
	FVector Start;
	FVector End;
	ECollisionChannel CollisionChannel;
	bool IsHit;

	//Player Attributes
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float Health = 75;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float MaxHealth = 100;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float Ammo = 100;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool CanGrappleToSurface;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool GameOver;

	//Player Inventory
	UFUNCTION(BlueprintCallable, Category = "Items")
	void UseItem(class UItem* Item);
	AActor* OverlappingActor;
	
	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* Other, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
		
};