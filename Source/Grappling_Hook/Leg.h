// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "Leg.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GRAPPLING_HOOK_API ULeg : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	ULeg();
	//Length of leg stride
	UPROPERTY(EditAnywhere, Category = "RayCast")
	float StepLength;
	UPROPERTY(EditAnywhere, Category = "RayCast")
	float StepLengthFallback; //Same as step length but since step length value changes during tick we will use this value to set StepLength back to it's default
	//Height of the leg stride
	UPROPERTY(EditAnywhere, Category = "RayCast")
	float StepHeight;
	UPROPERTY(EditAnywhere, Category = "RayCast")
	float StepHeightFallBack; //Same as step length but since step length value changes during tick we will use this value to set StepLength back to it's default
	//Check for ground on the sides in no ground in the front
	float SideStepLength;
	FVector RayStart;
	FVector RayEnd;
	FVector RayHeight;
	//OutHit to check if we have hit the ground
	FHitResult OutHit;
	//Hit location
	FVector TraceFootTargetLocation();
	bool TraceSideTargetLocation(FVector Start, FVector End);	

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	//stride speed
	UPROPERTY(EditAnywhere, Category = "SwingLeg")
	float SwingSpeed;
	//If we find the group then swing leg
	UPROPERTY(BlueprintReadOnly, Category = "SwingLeg")
	bool SwingLeg;
	UPROPERTY(BlueprintReadOnly, Category = "SwingLeg")
	FVector StepCurrentLocation;
	UPROPERTY(BlueprintReadOnly, Category = "SwingLeg")
	FVector FootLocation;
	FVector FootTargetLocation; //
	FVector StepTargetLocation; //Same as the foot target location		
};
