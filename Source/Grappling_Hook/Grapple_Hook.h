// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Grappling_HookCharacter.h"
#include "Grapple_Hook.generated.h"

UCLASS()
class GRAPPLING_HOOK_API AGrapple_Hook : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGrapple_Hook();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, Category = "Mesh")
	UStaticMeshComponent* HookMesh;

	UPROPERTY(EditDefaultsOnly)
	float Speed;
	bool hook;
	
	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* Other, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	//Player reference
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AActor* PlayerPawn;
	AGrappling_HookCharacter* Player;

	//Player Grapple Vectors
	FVector GrapplePoint;
	FVector SurfaceNormal;
};
