#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "..\HelperTypes.h"
#include "InventoryComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class AUTOMATIONTESTSPROJ_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UInventoryComponent();

	bool AddItem(const FInventoryData& Data);

	UFUNCTION(BlueprintCallable)
	int32 GetInventoryAmountByType(EInventoryItemType Type) const;

protected:
	virtual void BeginPlay() override;

	//Maximum amount we can pick up
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TMap<EInventoryItemType, int32> InventoryLimits;

private:
	//Map of types and amount picked up of said type
	TMap<EInventoryItemType, int32> Inventory;
};
