// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraSystem.h"
#include "NiagaraComponent.h"
#include "WeaponProjectile.generated.h"

UCLASS()
class GRAPPLING_HOOK_API AWeaponProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeaponProjectile();
	//Projectile Mesh
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Projectile Mesh")
	UStaticMeshComponent* ProjectileMesh;
	
	//VFX
	UPROPERTY(EditDefaultsOnly, Category = "Effects")
	UNiagaraSystem* NiagaraSystem;
	//Has Physics enabled
	UPROPERTY(EditDefaultsOnly, Category = "Physics Enabled")
	bool PhysicsEnabled;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Projectile Type")
	bool Skill;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Projectile Type")
	bool MainProjectile;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Projectile Type")
	bool Bullet;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Projectile Type")
	bool Grenade;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Projectile Type")
	float ProjectileTimer;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Projectile Type")
	float DamageRadius;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Projectile Type")
	float SkillDamageAmount;
	float DamageAmount;

	float ProjTimerRef;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	FVector DirectionalVec;
	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* Other, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	void SpawnDestroyEffect(FVector SpawnLoc);

	//grenade Sweep
	TArray<FHitResult> OutHits;
	FVector Start;
	FVector End;
	
};

