// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Pickup_interface.h"
#include "Action_Interface.h"
#include "GameFramework/Actor.h"
#include "Resource_pickups.generated.h"

UCLASS()
class GRAPPLING_HOOK_API AResource_pickups : public AActor, public IPickup_interface, public IAction_Interface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AResource_pickups();
	UPROPERTY(EditDefaultsOnly, Category = "Mesh")
	UStaticMeshComponent* Mesh;
	USceneComponent* RootComponent;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Pickup_Interface")
	void Pickups();
	virtual void Pickups_Implementation() override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Action_Interface")
	void Actions();
	virtual void Actions_Implementation() override;
};
