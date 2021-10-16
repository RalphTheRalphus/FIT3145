// Fill out your copyright notice in the Description page of Project Settings.


#include "Item.h"

UItem::UItem()
{
	ItemDisplayName = FText::FromString("ItemName");
	UseTextAction = FText::FromString("Use Info");
	ItemQuantity = 1;
}

void UItem::Use(AGrappling_HookCharacter* Character)
{
	
}
