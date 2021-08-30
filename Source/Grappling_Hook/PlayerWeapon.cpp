// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerWeapon.h"
#include "Kismet/GameplayStatics.h"
#include "Grappling_HookCharacter.h"

#include "Camera/CameraComponent.h"

// Sets default values
APlayerWeapon::APlayerWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Timer = 0.f;
}

// Called when the game starts or when spawned
void APlayerWeapon::BeginPlay()
{
	Super::BeginPlay();
	
	Player = Cast<AGrappling_HookCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
}

// Called every frame
void APlayerWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector SocketLoc = Player->GetMesh()->GetSocketLocation("Sentinal_Weapon_Socket");
	FVector SetLoc = FMath::VInterpTo(GetActorLocation(), SocketLoc, DeltaTime, 15);
	FRotator SetRot = FMath::RInterpTo(GetActorRotation(), Player->GetFollowCamera()->GetComponentRotation(), DeltaTime, 5);
	SetActorLocation(SetLoc);
	SetActorRotation(SetRot);

	if(RotateFire)
	{
		if(RotateTimeOut > 0)
		{
			RotateTimeOut -= DeltaTime;

			//Rotation
			FRotator Rotator(0, 0, 1000 * DeltaTime); //rotate the yaw
			FQuat RotationQuaternion(Rotator);
			AddActorLocalRotation(RotationQuaternion, false, nullptr, ETeleportType::None);
		}
		else
			RotateFire = false;
	}
	if(Timer > 0)
		Timer -= DeltaTime;
}

void APlayerWeapon::Shoot()
{
	if(Timer <= 0)
	{
		Timer = ShootDelay;
		GEngine->AddOnScreenDebugMessage(1,1,FColor::Red, TEXT("FIRE!!!!"));
		RotateFire = true;
		RotateTimeOut = 0.5f;
		if(WeaponProjectileClass)
		{
			FVector SpawnLoc = GetActorLocation();
			WeaponProjectile = GetWorld()->SpawnActor<AWeaponProjectile>(WeaponProjectileClass, SpawnLoc, GetActorRotation());
		}
	}
}

