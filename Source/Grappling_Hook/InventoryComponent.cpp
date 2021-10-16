// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryComponent.h"

#include "Item.h"

// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
	Capacity = 20;
}


// Called when the game starts
void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	for(auto Item: Items)
	{
		AddItem(Item);
	}
	
}


bool UInventoryComponent::AddItem(UItem* Item)
{
	for(auto& item: Items)
	{
		if(item->IsA(Item->GetClass()))
		{
			item->ItemQuantity += 1;
			OnInventoryUpdated.Broadcast();
			return true;
		}
	}
	if(Items.Num() >= Capacity || !Item)
	{
		return false;
	}
	Item->OwningInventory = this;
	Items.Add(Item);
	//update UI
	OnInventoryUpdated.Broadcast();
	return true;
}

bool UInventoryComponent::RemoveItem(UItem* Item)
{
	for(auto& item: Items)
	{
		if(item->IsA(Item->StaticClass()))
		{
			item->ItemQuantity -= 1;
			OnInventoryUpdated.Broadcast();
			return true;
		}
	}
	if(Item)
	{
		Item->OwningInventory = nullptr;
		Item->World = nullptr;
		Items.RemoveSingle(Item);
		OnInventoryUpdated.Broadcast();
		return true;
	}
	return false;
}

