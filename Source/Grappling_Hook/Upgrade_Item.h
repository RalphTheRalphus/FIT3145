// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "Upgrade_Item.generated.h"

/**
 * 
 */
UCLASS()
class GRAPPLING_HOOK_API UUpgrade_Item : public UItem
{
	GENERATED_BODY()
	virtual void Use(AGrappling_HookCharacter* Character) override;

	UPROPERTY(EditDefaultsOnly, Category = "Upgrade Item Type")
	bool Health_Upgrade;
	UPROPERTY(EditDefaultsOnly, Category = "Upgrade Item Type")
	bool Sentinel_Damage_Upgrade;
};
