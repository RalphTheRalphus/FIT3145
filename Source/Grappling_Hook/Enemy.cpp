// Fill out your copyright notice in the Description page of Project Settings.

#include "Enemy.h"
#include "ShieldDrone.h"
#include "EnemyAI_Controller.h"
// Sets default values
AEnemy::AEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	if(ShieldDroneClass)
	{
		ShieldDrone = GetWorld()->SpawnActor<AShieldDrone>(ShieldDroneClass);
		HasShield = true;
	}

}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if(Health <= 0)
	{
		Destroy();
	}
	if(ShieldDrone)
	{
		FVector TargetDroneLoc = GetMesh()->GetSocketLocation("Drone");
		FVector SetLoc = FMath::VInterpTo(ShieldDrone->GetActorLocation(), TargetDroneLoc, DeltaTime, 15);
		ShieldDrone->SetActorLocation(SetLoc);
	}

}

// Called to bind functionality to input
void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

