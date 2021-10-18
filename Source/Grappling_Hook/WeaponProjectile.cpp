// Fill out your copyright notice in the Description page of Project Settings.
#define COLLISION_ENEMY ECC_GameTraceChannel2

#include "WeaponProjectile.h"
#include "Enemy.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AWeaponProjectile::AWeaponProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Projectile Mesh"));
	ProjectileMesh->SetupAttachment(RootComponent);

	ProjectileMesh->OnComponentBeginOverlap.AddDynamic(this, &AWeaponProjectile::OnOverlapBegin);
}

// Called when the game starts or when spawned
void AWeaponProjectile::BeginPlay()
{
	Super::BeginPlay();
	DirectionalVec = GetActorForwardVector();
	if(PhysicsEnabled)
		ProjectileMesh->SetSimulatePhysics(true);
	
	if(Grenade)
	{
		ProjectileMesh->AddForce(GetActorForwardVector() * 250000);
	}
	ProjTimerRef = ProjectileTimer;
}

// Called every frame
void AWeaponProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	ProjTimerRef -= DeltaTime;

	if(Bullet)
	{		
		FVector CurrLoc = GetActorLocation();
		CurrLoc += DirectionalVec * DeltaTime * 2000;
		SetActorLocation(CurrLoc);
	}
	if(Grenade && ProjTimerRef <= 0)
	{

		Start = ProjectileMesh->GetComponentLocation();
		FCollisionShape CollisionShape = FCollisionShape::MakeSphere(800);
		bool IsHit = GetWorld()->SweepMultiByChannel(OutHits, Start, Start, FQuat::Identity, ECC_Visibility, CollisionShape);
		if(IsHit)
		{
			UE_LOG(LogTemp, Warning, TEXT("Hit"));
			TArray<AEnemy*> IgnoreEnemy;
			for(auto& OutHit: OutHits)
			{
				AEnemy* Enemy = Cast<AEnemy>(OutHit.GetActor());
				if(Enemy && !IgnoreEnemy.Contains(Enemy))
				{
					IgnoreEnemy.Add(Enemy);
					Enemy->Health -= DamageAmount;
					SpawnDestroyEffect(Enemy->GetActorLocation());
					
				}
			}
		}
		Destroy();
	}
}

void AWeaponProjectile::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* Other, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	const FHitResult& SweepResult)
{
	//GEngine->AddOnScreenDebugMessage(2,1,FColor::Red,TEXT("HIT!!!"));
	AEnemy* EnemyCharacter = Cast<AEnemy>(Other);
	if(EnemyCharacter)
	{
		if(!EnemyCharacter->HasShield)
			EnemyCharacter->Health -= DamageAmount;
	}
	SpawnDestroyEffect(GetActorLocation());
	Destroy();
}

void AWeaponProjectile::SpawnDestroyEffect(FVector SpawnLoc)
{
	if(NiagaraSystem)
	{
		UNiagaraComponent* SpawnEffect = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), NiagaraSystem, SpawnLoc, GetActorRotation(),
			FVector(1,1,1));
	}
}
