// Fill out your copyright notice in the Description page of Project Settings.


#include "ShieldDrone.h"
#include "WeaponProjectile.h"

// Sets default values
AShieldDrone::AShieldDrone()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root Component"));
	DroneMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Drone Mesh"));
	DroneMesh->SetupAttachment(RootComponent);

	DroneMesh->OnComponentBeginOverlap.AddDynamic(this, &AShieldDrone::OnOverlapBegin);
}

// Called when the game starts or when spawned
void AShieldDrone::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AShieldDrone::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AShieldDrone::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* Other, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(Other->IsA(AWeaponProjectile::StaticClass()))
		OnDamageTaken.Broadcast();
}

