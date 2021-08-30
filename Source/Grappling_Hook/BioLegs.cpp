// Fill out your copyright notice in the Description page of Project Settings.


#include "BioLegs.h"
#include "GameFramework/Character.h"

// Sets default values
ABioLegs::ABioLegs()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	throwdisc = false;
	discspeed = 2000;
	TimeToReturn = 3;
}

// Called when the game starts or when spawned
void ABioLegs::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ABioLegs::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	TimeToReturn -= DeltaTime;
	if(TimeToReturn > 0)
	{
		DirectionalVector = GetActorForwardVector();
	}
	else
	{
		DirectionalVector = (GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation() - GetActorLocation());
		DirectionalVector.Normalize();
		if(FVector::Dist(GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation(), GetActorLocation()) < 50)
			Destroy();
	}
	FVector CurrentLoc = GetActorLocation();
	CurrentLoc += DirectionalVector * DeltaTime * discspeed;
	SetActorLocation(CurrentLoc);
	//SetActorRotation(GetActorRotation() + FRotator(0,0,1) * DeltaTime * discspeed);
	GEngine->AddOnScreenDebugMessage(1, 1, FColor::Red, FString::Printf(TEXT("Disc location %s"), *GetActorLocation().ToString()));
	
}

