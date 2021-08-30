// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BioLegs.generated.h"

UCLASS()
class GRAPPLING_HOOK_API ABioLegs : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABioLegs();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UPROPERTY()
	bool throwdisc;
	UPROPERTY()
	FVector forwardvector;
	float discspeed;
	float TimeToReturn;
	FVector DirectionalVector;
};
