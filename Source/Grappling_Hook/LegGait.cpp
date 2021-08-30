// Fill out your copyright notice in the Description page of Project Settings.


#include "LegGait.h"

// Sets default values for this component's properties
ULegGait::ULegGait()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
	GaitTimer = 1;
	Speed = 750;	
}

void ULegGait::Pace()
{
	Theta = FMath::Sin((GaitTimer * (PI/180))/2);
	if(Theta > 0 && Theta <= 1)
	{
		for(auto& Leg: Legs)
		{
			if(Leg->ComponentHasTag(FName("LeftLeg")))
			{
				if(FVector::Dist(Leg->StepCurrentLocation, Leg->StepTargetLocation) > 1)
				{
					Leg->StepCurrentLocation = FMath::Lerp(Leg->StepCurrentLocation, Leg->StepTargetLocation, Theta);
					Leg->FootLocation = Leg->StepCurrentLocation + FVector(0,0,HieghtCurve->GetFloatValue(Theta) * 40);
				}					
			}
		}
	}
	if(Theta < 0 && Theta >= -1)
	{
		for(auto& Leg: Legs)
		{
			if(Leg->ComponentHasTag(FName("RightLeg")))
			{
				if(FVector::Dist(Leg->StepCurrentLocation, Leg->StepTargetLocation) > 1)
				{
					Leg->StepCurrentLocation = FMath::Lerp(Leg->StepCurrentLocation, Leg->StepTargetLocation, abs(Theta));
					Leg->FootLocation = Leg->StepCurrentLocation + FVector(0,0,HieghtCurve->GetFloatValue(Theta) * 40);
				}	
			}
		}
	}	
}

void ULegGait::Trot()
{
	Theta = FMath::Sin((GaitTimer * (PI/180))/4) *4;
	//UE_LOG(LogTemp, Warning, TEXT("Theta = %f"), Theta);
	if(Theta > 0 && Theta <= 4)
	{
		for(auto& Leg: Legs) // Right Leg Front and Left Leg Back
		{
			if(Leg->ComponentHasTag(FName("RightLeg")) && Leg->ComponentHasTag(FName("Front")))
			{
				if(FVector::Dist(Leg->StepCurrentLocation, Leg->StepTargetLocation) > 1)
				{
					Leg->StepCurrentLocation = FMath::Lerp(Leg->StepCurrentLocation, Leg->StepTargetLocation, Theta/4);
					Leg->FootLocation = Leg->StepCurrentLocation + FVector(0,0,HieghtCurve->GetFloatValue(Theta) * 40);
				}
				else
				{
					Leg->FootLocation = Leg->StepTargetLocation;
					Leg->StepLength = Leg->StepLengthFallback;
					Leg->StepHeight = Leg->StepHeightFallBack;
				}			
			}

			if(Leg->ComponentHasTag(FName("LeftLeg")) && Leg->ComponentHasTag(FName("Back")))
			{
				if(FVector::Dist(Leg->StepCurrentLocation, Leg->StepTargetLocation) > 1)
				{
					Leg->StepCurrentLocation = FMath::Lerp(Leg->StepCurrentLocation, Leg->StepTargetLocation, Theta/4);
					Leg->FootLocation = Leg->StepCurrentLocation + FVector(0,0,HieghtCurve->GetFloatValue(Theta) * 40);
				}
				else
				{
					Leg->FootLocation = Leg->StepTargetLocation;
					Leg->StepLength = Leg->StepLengthFallback;
					Leg->StepHeight = Leg->StepHeightFallBack;
				}
			}
		}
	}
	if(Theta < 0 && Theta >= -4)
	{
		for(auto& Leg: Legs) //Left Leg Front and Right Leg Back
		{
			if(Leg->ComponentHasTag(FName("LeftLeg")) && Leg->ComponentHasTag(FName("Front")))
			{				
				if(FVector::Dist(Leg->StepCurrentLocation, Leg->StepTargetLocation) > 1)
				{
					Leg->StepCurrentLocation = FMath::Lerp(Leg->StepCurrentLocation, Leg->StepTargetLocation, abs(Theta/4));
					Leg->FootLocation = Leg->StepCurrentLocation + FVector(0,0,HieghtCurve->GetFloatValue(Theta) * 40);
				}
				else
				{
					Leg->FootLocation = Leg->StepTargetLocation;
					Leg->StepLength = Leg->StepLengthFallback;
					Leg->StepHeight = Leg->StepHeightFallBack;
				}
			}

			if(Leg->ComponentHasTag(FName("RightLeg")) && Leg->ComponentHasTag(FName("Back")))
			{				
				if(FVector::Dist(Leg->StepCurrentLocation, Leg->StepTargetLocation) > 1)
				{
					Leg->StepCurrentLocation = FMath::Lerp(Leg->StepCurrentLocation, Leg->StepTargetLocation, abs(Theta/4));
					Leg->FootLocation = Leg->StepCurrentLocation + FVector(0,0,HieghtCurve->GetFloatValue(Theta) * 40);
				}
				else
				{
					Leg->FootLocation = Leg->StepTargetLocation;
					Leg->StepLength = Leg->StepLengthFallback;
					Leg->StepHeight = Leg->StepHeightFallBack;
				}
			}	
		}	
	}	
}

void ULegGait::Gallup()
{
	Theta = FMath::Sin((GaitTimer * (PI/180))/4);
	UE_LOG(LogTemp, Warning, TEXT("Theta = %f"), Theta);
	if(Theta > 0 && Theta <= 1)
	{
		for(auto& Leg: Legs)
		{
			if(Leg->ComponentHasTag(FName("LeftLeg")) && Leg->ComponentHasTag(FName("Back")))
			{
				if(FVector::Dist(Leg->StepCurrentLocation, Leg->StepTargetLocation) > 1)
				{
					Leg->StepCurrentLocation = FMath::Lerp(Leg->StepCurrentLocation, Leg->StepTargetLocation, Theta);
					Leg->FootLocation = Leg->StepCurrentLocation + FVector(0,0,HieghtCurve->GetFloatValue(Theta) * 40);
				}					
			}
			
			if(Leg->ComponentHasTag(FName("RightLeg")) && Leg->ComponentHasTag(FName("Back")) && Theta > 0.5)
			{
				if(FVector::Dist(Leg->StepCurrentLocation, Leg->StepTargetLocation) > 1)
				{
					Leg->StepCurrentLocation = FMath::Lerp(Leg->StepCurrentLocation, Leg->StepTargetLocation, Theta);
					Leg->FootLocation = Leg->StepCurrentLocation + FVector(0,0,HieghtCurve->GetFloatValue(Theta) * 40);
				}	
			}
		}		
	}
	if(Theta < 0 && Theta >= -1)
	{
		for(auto& Leg: Legs)
		{
			if(Leg->ComponentHasTag(FName("RightLeg")) && Leg->ComponentHasTag(FName("Front")) && Theta < -0.5)
			{
				if(FVector::Dist(Leg->StepCurrentLocation, Leg->StepTargetLocation) > 1)
				{
					Leg->StepCurrentLocation = FMath::Lerp(Leg->StepCurrentLocation, Leg->StepTargetLocation, abs(Theta));
					Leg->FootLocation = Leg->StepCurrentLocation + FVector(0,0,HieghtCurve->GetFloatValue(Theta) * 40);
				}	
			}
			
			if(Leg->ComponentHasTag(FName("LeftLeg")) && Leg->ComponentHasTag(FName("Front")))
			{
				if(FVector::Dist(Leg->StepCurrentLocation, Leg->StepTargetLocation) > 1)
				{
					Leg->StepCurrentLocation = FMath::Lerp(Leg->StepCurrentLocation, Leg->StepTargetLocation, abs(Theta));
					Leg->FootLocation = Leg->StepCurrentLocation + FVector(0,0,HieghtCurve->GetFloatValue(Theta) * 40);
				}	
			}
		}		
	}	
}

// Called when the game starts
void ULegGait::BeginPlay()
{
	Super::BeginPlay();

	// ...
	TArray<USceneComponent*> OwnerChildComponents;
	GetOwner()->GetRootComponent()->GetChildrenComponents(true, OwnerChildComponents); //gets all the scene components of the owner
	for(auto& ChildComponent: OwnerChildComponents) //cast all the scene components to legs
	{
		if(ChildComponent->IsA(ULeg::StaticClass()))
			Legs.Add(Cast<ULeg>(ChildComponent));
	}
	for(auto& Leg: Legs)
	{
		if(Leg->ComponentHasTag(FName("LeftLeg")) && Leg->ComponentHasTag(FName("Front")) && Leg->ComponentHasTag(FName("Set1")))
		{
			LeftLeg_Front = Leg;
		}
		if(Leg->ComponentHasTag(FName("RightLeg")) && Leg->ComponentHasTag(FName("Back")) && Leg->ComponentHasTag(FName("Set1")))
		{
			RightLeg_Back = Leg;
		}
		if(Leg->ComponentHasTag(FName("RightLeg")) && Leg->ComponentHasTag(FName("Front")) && Leg->ComponentHasTag(FName("Set1")))
		{
			RightLeg_Front = Leg;
		}
		if(Leg->ComponentHasTag(FName("LeftLeg")) && Leg->ComponentHasTag(FName("Back")) && Leg->ComponentHasTag(FName("Set1")))
		{
			LeftLeg_Back = Leg;
		}
		if(Leg->ComponentHasTag(FName("LeftLeg")) && Leg->ComponentHasTag(FName("Front")) && Leg->ComponentHasTag(FName("Set2")))
		{
			LeftLeg2_Front = Leg;
		}
		if(Leg->ComponentHasTag(FName("RightLeg")) && Leg->ComponentHasTag(FName("Back")) && Leg->ComponentHasTag(FName("Set2")))
		{
			RightLeg2_Back = Leg;
		}
		if(Leg->ComponentHasTag(FName("RightLeg")) && Leg->ComponentHasTag(FName("Front")) && Leg->ComponentHasTag(FName("Set2")))
		{
			RightLeg2_Front = Leg;
		}
		if(Leg->ComponentHasTag(FName("LeftLeg")) && Leg->ComponentHasTag(FName("Back")) && Leg->ComponentHasTag(FName("Set2")))
		{
			LeftLeg2_Back = Leg;
		}		
	}
	NumberOfLegs = Legs.Num();	
	
}


// Called every frame
void ULegGait::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
	//Gait Needle that moves 360*
	if(GaitTimer > 0 && GaitTimer < 360)
		GaitTimer += DeltaTime * Speed;
	if(GaitTimer < 0 && GaitTimer > -360)
		GaitTimer -= DeltaTime * Speed;

	if(GaitTimer >= 360)
		GaitTimer = -1;
	if(GaitTimer <= -360)
		GaitTimer = 1;
	Trot();	
}



