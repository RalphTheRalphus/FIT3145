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
	ShootTimer = 0.f;
	SkillTimer = 0.f;
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
	if(Player)
	{
		FVector SocketLoc = Player->GetMesh()->GetSocketLocation("Sentinal_Weapon_Socket");
		FVector SetLoc = FMath::VInterpTo(GetActorLocation(), SocketLoc, DeltaTime, 15);
		if(Player->OutHit.bBlockingHit)
		{
			FRotator SetRot = FMath::RInterpTo(GetActorRotation(), (Player->OutHit.Location - GetActorLocation()).Rotation(), DeltaTime, 5);
			SetActorRotation(SetRot);			
		}
		else
		{
			FRotator SetRot = FMath::RInterpTo(GetActorRotation(), (Player->OutHit.TraceEnd - GetActorLocation()).Rotation(), DeltaTime, 5);
			SetActorRotation(SetRot);
		}

		SetActorLocation(SetLoc);
	}

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
	if(ShootTimer > 0)
		ShootTimer -= DeltaTime;
	if(SkillTimer > 0)
		SkillTimer -= DeltaTime;
}

void APlayerWeapon::Shoot()
{
	if(ShootTimer <= 0)
	{
		ShootTimer = ShootDelay;
		RotateFire = true;
		RotateTimeOut = 0.5f;
		if(WeaponProjectileClass)
		{
			GEngine->AddOnScreenDebugMessage(1,1,FColor::Red, TEXT("FIRE!!!!"));
			FVector SpawnLoc = GetActorLocation();
			WeaponProjectile = GetWorld()->SpawnActor<AWeaponProjectile>(WeaponProjectileClass, SpawnLoc, GetActorRotation());
		}
	}
}

void APlayerWeapon::Skill()
{
	if(SkillTimer <= 0)
	{
		SkillTimer = SkillDelay;
		RotateFire = true;
		RotateTimeOut = 0.5f;
		if(SkillProjectileClass)
		{
			GEngine->AddOnScreenDebugMessage(1,1,FColor::Red, TEXT("Skill!!!!"));			
			FVector SpawnLoc = GetActorLocation();
			SkillProjectile = GetWorld()->SpawnActor<AWeaponProjectile>(SkillProjectileClass, SpawnLoc, GetActorRotation());
		}
	}
}

