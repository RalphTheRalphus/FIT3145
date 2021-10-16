// Fill out your copyright notice in the Description page of Project Settings.


#include "Resource_pickups.h"
#include "Kismet/GameplayStatics.h"
#include "Grappling_HookCharacter.h"
#include "InventoryComponent.h"
#include "Item.h"

// Sets default values
AResource_pickups::AResource_pickups()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Resource Mesh"));
	Mesh->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AResource_pickups::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AResource_pickups::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AResource_pickups::Pickups_Implementation()
{
	GEngine->AddOnScreenDebugMessage(1,1,FColor::Red, TEXT("Press E to collect"));
}

void AResource_pickups::Actions_Implementation()
{
	AGrappling_HookCharacter* Player = Cast<AGrappling_HookCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	if(Player)
	{
		//AResource_pickups* Copy = Cast<AResource_pickups>(this);

		UInventoryComponent* NewInventory = Cast<UInventoryComponent>(Player->GetComponentByClass(UInventoryComponent::StaticClass()));
		if(NewInventory && Item)
		{
			NewInventory->AddItem(Item);
		}
		GEngine->AddOnScreenDebugMessage(1,1,FColor::Red, TEXT("Resource Collected"));

		Destroy();
	}
}

