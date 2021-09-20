// Fill out your copyright notice in the Description page of Project Settings.

#include "Kismet/GameplayStatics.h"
#include "Grappling_HookCharacter.h"
#include "DamageOnOverlap.h"

// Sets default values
ADamageOnOverlap::ADamageOnOverlap()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
}

// Called when the game starts or when spawned
void ADamageOnOverlap::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADamageOnOverlap::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AGrappling_HookCharacter* Character = Cast<AGrappling_HookCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	if(Character)
	{
		if(this->IsOverlappingActor(Character))
		{
			Character->Health -= DamageAmount;
		}
	}
}
