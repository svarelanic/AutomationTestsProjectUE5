#include "InventoryComponent.h"

UInventoryComponent::UInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

}

bool UInventoryComponent::AddItem(const FInventoryData& Data)
{
	if(Data.Score < 0) return false;

	//If item type picked up for the first time, add it to the Map
	if(!Inventory.Contains(Data.Type))
	{
		Inventory.Add(Data.Type, 0);
	}

	const auto NextScore = Inventory[Data.Type] + Data.Score;
	if(NextScore > InventoryLimits[Data.Type]) return false;
	
	//Add score to the item type
	Inventory[Data.Type] = NextScore;
	
	return true;
}

int32 UInventoryComponent::GetInventoryAmountByType(EInventoryItemType Type) const
{
	return Inventory.Contains(Type) ? Inventory[Type] : 0;
}


void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	
}


