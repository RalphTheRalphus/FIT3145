// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Leg.h"
#include "Curves/CurveFloat.h"
#include "Components/ActorComponent.h"
#include "LegGait.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GRAPPLING_HOOK_API ULegGait : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	ULegGait();

	//Speed of movement
	UPROPERTY(EditDefaultsOnly, Category = "Gait Manager")
	float Speed;
	//Number of legs our owner actor has
	int NumberOfLegs;
	//angle to get the function of
	float Theta;
	//Does multiple full rotations from 0 to 360 and used for Theta
	float GaitTimer;
	//Stores all the legs owner actor has
	TArray<ULeg*> Legs;

	TArray<USceneComponent*> SceneComponents;
	UPROPERTY(BlueprintReadOnly, Category = "Legs")
	ULeg* RightLeg_Front;
	UPROPERTY(BlueprintReadOnly, Category = "Legs")
	ULeg* LeftLeg_Front;
	UPROPERTY(BlueprintReadOnly, Category = "Legs")
	ULeg* RightLeg_Back;
	UPROPERTY(BlueprintReadOnly, Category = "Legs")	
	ULeg* LeftLeg_Back;
	UPROPERTY(BlueprintReadOnly, Category = "Legs")
	ULeg* RightLeg2_Front;
	UPROPERTY(BlueprintReadOnly, Category = "Legs")
	ULeg* LeftLeg2_Front;
	UPROPERTY(BlueprintReadOnly, Category = "Legs")
	ULeg* RightLeg2_Back;
	UPROPERTY(BlueprintReadOnly, Category = "Legs")	
	ULeg* LeftLeg2_Back;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	//Height curve to change the height of the leg as it strides
	UPROPERTY(EditAnywhere, Category = "Curves")
	UCurveFloat* HieghtCurve;
	//Pre Defined Gait Sequence
	FVector MoveLeg;
	void Pace();
	void Trot();
	void Gallup();
		
};
