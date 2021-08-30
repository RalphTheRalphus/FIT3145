// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "ChangeHeight.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GRAPPLING_HOOK_API UChangeHeight : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UChangeHeight();

	void ChangeActorHeight(float DeltaTime);
	void ChangeActorRotation(float DeltaTime);
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	//Distance of the actor from the ground
	UPROPERTY(EditDefaultsOnly, Category = "Ground Height")
	float GroundHeight;
	FHitResult OutHit;
	UPROPERTY(EditDefaultsOnly, Category = "Actor Rotation")
	float distance;
	FHitResult OutHit2;
	float ActorPitch;
	FHitResult DownHit;
	float Height;	
};
