// Fill out your copyright notice in the Description page of Project Settings.


#include "Upgrade_Item.h"
#include "PlayerWeapon.h"
#include "Grappling_HookCharacter.h"

void UUpgrade_Item::Use(AGrappling_HookCharacter* Character)
{
	UE_LOG(LogTemp, Warning, TEXT("Upgrade item used"));
	if(Health_Upgrade)
	{
		Character->MaxHealth += 10;
		GEngine->AddOnScreenDebugMessage(1, 1, FColor::Green, FString::Printf(TEXT("Max Health Increased To: %f"), Character->MaxHealth));
	}
	if(Sentinel_Damage_Upgrade)
	{
		APlayerWeapon* PlayerWeapon = Cast<APlayerWeapon>(Character->PlayerWeapon);
		PlayerWeapon->WeaponDamage += 50;
	}

}
