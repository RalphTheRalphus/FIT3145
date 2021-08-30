// Fill out your copyright notice in the Description page of Project Settings.


#include "ChangeHeight.h"

#include "DrawDebugHelpers.h"

// Sets default values for this component's properties
UChangeHeight::UChangeHeight()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UChangeHeight::ChangeActorHeight(float DeltaTime)
{
	FVector TraceStart = GetComponentLocation();
	FVector TraceEnd = TraceStart + (GetComponentLocation().DownVector * GroundHeight);

	bool IsHit = GetWorld()->LineTraceSingleByChannel(OutHit, TraceStart, TraceEnd, ECC_Visibility);
	DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor::Green, false);
	DrawDebugPoint(GetWorld(), OutHit.Location, 10, FColor::Yellow, false);
	if(IsHit)
	{
		Height = FVector::Dist(TraceStart, OutHit.Location);
		//FVector NewHeight = OutHit.Location + (OutHit.Location.UpVector * (GroundHeight/2));
		//if(FVector::Dist(GetOwner()->GetActorLocation(), NewHeight) >= 10)
		//{
			//GetOwner()->SetActorLocation(FMath::Lerp(GetOwner()->GetActorLocation(), NewHeight, DeltaTime * 5));
		//}
	}	
}

void UChangeHeight::ChangeActorRotation(float DeltaTime)
{
	FVector TraceStart = GetComponentLocation();
	FVector TraceEnd = TraceStart + (GetForwardVector() * distance);
	FVector TraceEndDown = TraceEnd + (GetComponentLocation().DownVector * distance);

	bool IsHit = GetWorld()->LineTraceSingleByChannel(OutHit2, TraceStart, TraceEnd, ECC_Visibility);
	DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor::Green, false);
	DrawDebugPoint(GetWorld(), OutHit2.Location, 10, FColor::Yellow, false);

	if(IsHit)
	{
		if(FVector::Dist(GetComponentLocation(), OutHit2.Location) < (distance-10))
		{			
			ActorPitch += DeltaTime *20;
			FRotator Rotatetrace = (GetComponentLocation() - OutHit2.Location).Rotation();
			GetOwner()->SetActorRotation(FRotator(ActorPitch, GetOwner()->GetActorRotation().Yaw, 0));
			SetWorldRotation(GetOwner()->GetActorRotation());
		}
	}
	else
	{
		if(ActorPitch > 0)
		{
			ActorPitch -= DeltaTime *20;
			GetOwner()->SetActorRotation(FRotator(ActorPitch, GetOwner()->GetActorRotation().Yaw, 0));
			SetWorldRotation(GetOwner()->GetActorRotation());
		}
		bool IsHit2 = GetWorld()->LineTraceSingleByChannel(DownHit, TraceEnd, TraceEndDown, ECC_Visibility);
		DrawDebugLine(GetWorld(), TraceEnd, TraceEndDown, FColor::Green, false);
		DrawDebugPoint(GetWorld(), DownHit.Location, 10, FColor::Yellow, false);
		if(IsHit2)
		{
			if(FVector::Dist(TraceEnd, DownHit.Location) > ((1.5*Height)+10))
			{
				ActorPitch -= DeltaTime *20;
				GetOwner()->SetActorRotation(FRotator(ActorPitch, GetOwner()->GetActorRotation().Yaw, 0));
				SetWorldRotation(GetOwner()->GetActorRotation());
			}
			if(FVector::Dist(TraceEnd, DownHit.Location) <= (Height*1.5))
			{

				ActorPitch += DeltaTime *20;
				GetOwner()->SetActorRotation(FRotator(ActorPitch, GetOwner()->GetActorRotation().Yaw, 0));
				SetWorldRotation(GetOwner()->GetActorRotation());
			}
		}
	}	
}


// Called when the game starts
void UChangeHeight::BeginPlay()
{
	Super::BeginPlay();

	// ...
	ActorPitch = GetOwner()->GetActorRotation().Pitch;	
}


// Called every frame
void UChangeHeight::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
	ChangeActorHeight(DeltaTime);
	ChangeActorRotation(DeltaTime);	
}

