// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Pickup_interface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UPickup_interface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class GRAPPLING_HOOK_API IPickup_interface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Pickup_Interface")
		void Pickups();
};
