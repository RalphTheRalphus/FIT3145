// Fill out your copyright notice in the Description page of Project Settings.


#include "Leg.h"

#include "DrawDebugHelpers.h"

// Sets default values for this component's properties
ULeg::ULeg()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
	StepLength = 200;
	StepHeight = 150;
	SideStepLength = 0;
	SwingSpeed = 10;
	SwingLeg = true;	
}


FVector ULeg::TraceFootTargetLocation()
{
	//FRotator SocketRotation;
	//RayStart = GetSocketLocation(GetAttachSocketName());
	//StepLength = abs(GetOwner()->GetVelocity().Size());
	RayStart = GetComponentLocation();
	RayEnd = RayStart + (GetForwardVector() * StepLength);
	RayHeight = RayEnd + (GetComponentLocation().DownVector * StepHeight);
	
	bool isHit = GetWorld()->LineTraceSingleByChannel(OutHit, RayStart, RayEnd, ECC_Visibility);
	DrawDebugLine(GetWorld(), RayStart, RayEnd, FColor::Red, false, -1, 0, 1);
	if(!isHit)
	{
		isHit = GetWorld()->LineTraceSingleByChannel(OutHit, RayEnd, RayHeight, ECC_Visibility);
		DrawDebugLine(GetWorld(), RayEnd, RayHeight, FColor::Red, false, -1, 0, 1);
		if(!isHit)
		{
			FVector SideStart = RayEnd + (FVector::CrossProduct(GetComponentLocation().UpVector, GetForwardVector()) * SideStepLength);
			FVector SideEnd = RayEnd + (FVector::CrossProduct(GetComponentLocation().DownVector, GetForwardVector()) * SideStepLength);
			isHit = TraceSideTargetLocation(SideStart, SideEnd);
		}
		if(isHit)
		{
			SwingLeg = true;
			DrawDebugPoint(GetWorld(), OutHit.Location, 25, FColor::Yellow, false);
			return OutHit.Location;
		}
	}
	if(StepLength > 400)
		StepLength = StepLengthFallback;
	if(StepHeight > 400)
		StepHeight = StepHeightFallBack;
	if(SideStepLength > 100)
		SideStepLength = 0;
	return OutHit.Location;	
}

bool ULeg::TraceSideTargetLocation(FVector Start, FVector End)
{
	bool isHit = GetWorld()->LineTraceSingleByChannel(OutHit, Start, End, ECC_Visibility);
	DrawDebugLine(GetWorld(), Start, End, FColor::Red, false);
	if(!isHit)
	{
		FVector RightSideEnd = Start + (GetComponentLocation().DownVector * StepHeight);
		isHit = GetWorld()->LineTraceSingleByChannel(OutHit, Start, RightSideEnd, ECC_Visibility);
		DrawDebugLine(GetWorld(), Start, RightSideEnd, FColor::Red, false);
		if(!isHit)
		{
			FVector LeftSideEnd = End + (GetComponentLocation().DownVector * StepHeight);
			isHit = GetWorld()->LineTraceSingleByChannel(OutHit, End, LeftSideEnd, ECC_Visibility);
			DrawDebugLine(GetWorld(), End, LeftSideEnd, FColor::Red, false);
			if(!isHit)
			{
				StepLength += 3;
				StepHeight += 3;
				SideStepLength += 3;
				SwingLeg = false;
			}
		}
	}
	return isHit;	
}

// Called when the game starts
void ULeg::BeginPlay()
{
	Super::BeginPlay();

	// ...
	StepCurrentLocation = GetComponentLocation();
	StepCurrentLocation.Z = FootTargetLocation.Z;	
}


// Called every frame
void ULeg::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
	FootTargetLocation = TraceFootTargetLocation();
	if(SwingLeg)
		StepTargetLocation = FootTargetLocation;	
}

