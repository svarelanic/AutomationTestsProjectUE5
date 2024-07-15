#pragma once

#include "CoreMinimal.h"
#include "HelperTypes.generated.h"

//Define inventory item types -> Static mesh will change depending on type
UENUM(BlueprintType)
enum class EInventoryItemType : uint8
{
	SPHERE = 0 UMETA(DisplayName = "Spheres"),
	CUBE UMETA(DisplayName = "Cubes"),
	CYLINDER UMETA(DisplayName = "Cylinders")
};

USTRUCT(BlueprintType)
struct FInventoryData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EInventoryItemType Type;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0")) //Cannot take negative values
	int32 Score;
};

USTRUCT(BlueprintType)
struct FHealthData
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (ClampMin = "1.0"))
	float MaxHealth{100.0f};
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (ClampMin = "0.1"))
	float HealModifier{1.0f};

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (ClampMin = "0.1", Units = "s"))
	float HealRate{0.5f};

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (ClampMin = "0.1", Units = "s"))
	float LifeSpan{5.0f};

	FString ToString() const
	{
		return FString::Printf(TEXT("MaxHealth=%f,HealModifier=%f,HealRate=%f,LifeSpan=%f"), MaxHealth, HealModifier, HealRate, LifeSpan);
	}
};