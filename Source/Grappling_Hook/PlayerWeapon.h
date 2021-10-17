// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeaponProjectile.h"
#include "PlayerWeapon.generated.h"

class AGrappling_HookCharacter;
UCLASS()
class GRAPPLING_HOOK_API APlayerWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APlayerWeapon();

	//Weapon Damage
	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	float WeaponDamage;
	
	//Weapon Mesh
	UPROPERTY(EditDefaultsOnly, Category = "Mesh")
	UStaticMeshComponent* WeaponMesh;
	//Weapon Projectile
	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
	TSubclassOf<AWeaponProjectile> WeaponProjectileClass;
	AWeaponProjectile* WeaponProjectile;

	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
	TSubclassOf<AWeaponProjectile> SkillProjectileClass;
	AWeaponProjectile* SkillProjectile;
	
	//ShootDelay
	UPROPERTY(EditDefaultsOnly, Category = "Shoot Timeout")
	float ShootDelay;
	UPROPERTY(EditDefaultsOnly, Category = "Skill Timer")
	float SkillDelay;
	float ShootTimer;
	float SkillTimer;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	AGrappling_HookCharacter* Player;
	void Shoot();
	void Skill();
	bool RotateFire;
	float RotateTimeOut;
};
