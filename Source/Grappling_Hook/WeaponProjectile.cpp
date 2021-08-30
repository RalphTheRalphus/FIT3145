// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponProjectile.h"

#include "GameFramework/ProjectileMovementComponent.h"

// Sets default values
AWeaponProjectile::AWeaponProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Projectile Mesh"));
	ProjectileMesh->SetupAttachment(RootComponent);

	ProjectileMesh->OnComponentBeginOverlap.AddDynamic(this, &AWeaponProjectile::OnOverlapBegin);
	ShootDelay = 5.f;
}

// Called when the game starts or when spawned
void AWeaponProjectile::BeginPlay()
{
	Super::BeginPlay();
	DirectionalVec = GetActorForwardVector();

}

// Called every frame
void AWeaponProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	FVector CurrLoc = GetActorLocation();
	CurrLoc += DirectionalVec * DeltaTime * 2000;
	SetActorLocation(CurrLoc);
	// Destroy projectile after delay
	if(ShootDelay > 0)
		ShootDelay -= DeltaTime;
	if(ShootDelay <= 0)
	{
		SpawnDestroyEffect(GetActorLocation());
		Destroy();
	}
}

void AWeaponProjectile::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* Other, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	const FHitResult& SweepResult)
{
	GEngine->AddOnScreenDebugMessage(2,1,FColor::Red,TEXT("HIT!!!"));
	FString Name = Other->GetName();
	SpawnDestroyEffect(SweepResult.Location);
	Destroy();
	UE_LOG(LogTemp, Warning, TEXT("HIT"));
}

void AWeaponProjectile::SpawnDestroyEffect(FVector SpawnLoc)
{
	if(NiagaraSystem)
	{
		UNiagaraComponent* SpawnEffect = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), NiagaraSystem, GetActorLocation(), GetActorRotation(),
			FVector(1,1,1));
	}
}
