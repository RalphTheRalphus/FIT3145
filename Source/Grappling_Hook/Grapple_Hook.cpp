// Fill out your copyright notice in the Description page of Project Settings.

#include "Grapple_Hook.h"

#include <string>

#include "Kismet/GameplayStatics.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
AGrapple_Hook::AGrapple_Hook()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	HookMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Hook"));
	HookMesh->SetupAttachment(RootComponent);
	
	//On overlap event
	HookMesh->OnComponentBeginOverlap.AddDynamic(this, &AGrapple_Hook::OnOverlapBegin);
	Speed = 200;
	hook = false;
}

// Called when the game starts or when spawned
void AGrapple_Hook::BeginPlay()
{
	Super::BeginPlay();
	Player = Cast<AGrappling_HookCharacter>(UGameplayStatics::GetPlayerPawn(this, 0));
}

// Called every frame
void AGrapple_Hook::Tick(float DeltaTime)
{
	if(Player)
	{
		
		if(!hook) // if not attached
		{	//Moving the hook
			Super::Tick(DeltaTime);
			FVector DirectionVec = GetActorForwardVector();
			FVector CurrLoc = GetActorLocation();
			CurrLoc += DirectionVec * Speed * DeltaTime;
			SetActorLocation(CurrLoc);

			//Rotation
			FRotator Rotator(0, 0, 1000 * DeltaTime); //rotate the yaw
			FQuat RotationQuaternion(Rotator);
			AddActorLocalRotation(RotationQuaternion, false, nullptr, ETeleportType::None);
			if(FVector::Dist(Player->GetActorLocation(), GetActorLocation()) > 3000)
			{
				Destroy();			
			}
		}

		if(hook) //if attached
		{
			FVector PlayerToGrapple = GrapplePoint - Player->GetActorLocation();
			PlayerToGrapple.Normalize(0.0001);
			FVector vec = Player->GetActorLocation() - GrapplePoint;
			vec.Normalize();
			SurfaceNormal.Normalize();
			if(FVector::DotProduct(SurfaceNormal, vec) > 0)
			{
				FVector DirectionVec = PlayerToGrapple + Player->GetFollowCamera()->GetForwardVector() + (Player->GetFollowCamera()->GetUpVector() * 1.3);
				Player->LaunchCharacter(DirectionVec * 1200 * DeltaTime, false, false);
				
			}
			else //if(FVector::DotProduct(SurfaceNormal, vec) < -0.2)
			{
				hook = false;
				Player->LaunchCharacter(FVector(0,0,-2000), false, false);
				Destroy();
			}
			if(FVector::Dist(GrapplePoint, Player->GetActorLocation()) < 200 || FVector::Dist(GrapplePoint, Player->GetActorLocation()) > 3000)
			{
				hook = false;
				Player->LaunchCharacter(FVector(0,0,-2000), false, false);
				Destroy();
			}
		}
	}
}

void AGrapple_Hook::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* Other, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	const FHitResult& SweepResult)
{
	hook = true;
	GrapplePoint = GetActorLocation();
	// get reference to attached object
	Player->LaunchCharacter(FVector(0,0,500), false, true);
}

