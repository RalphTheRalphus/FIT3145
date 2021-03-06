// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DamageOnOverlap.h"
#include "GameFramework/Character.h"
#include "Enemy.generated.h"


//class ADamageOnOverlap;
UCLASS()
class GRAPPLING_HOOK_API AEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//Enemy Attributes
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float Health = 100;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float Shield = 100;
	
	UPROPERTY(EditDefaultsOnly, Category = "Enemy Attack")
	float AttackDistance;
	UPROPERTY(EditDefaultsOnly, Category = "Enemy Attack")
	float AttackSpeed;

	UPROPERTY(EditDefaultsOnly, Category = "Shield")
	TSubclassOf<class AShieldDrone> ShieldDroneClass;
	UPROPERTY(BlueprintReadOnly, Category = "Shield")
	class AShieldDrone* ShieldDrone;
	UPROPERTY(BlueprintReadWrite, Category = "Shield")
	bool HasShield;
	
	//Enemy weapon
	UPROPERTY(EditDefaultsOnly, Category = "Enemy Weapon")
	TSubclassOf<ADamageOnOverlap> EnemyWeaponClass;
};
