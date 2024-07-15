#pragma once

#include "CoreMinimal.h"

class AUTOMATIONTESTSPROJ_API Battery
{
public:
	Battery() =	default;
	Battery(float PercentIn);

	void Charge();
	void Discharge();
	
	float GetPercent() const {return Percent;}
	FColor GetColor() const; //Returns a color given the percentage of the battery
	FString ToString() const; //Percentage to string

	//Overload operators to compare battery objects
	bool operator>=(const Battery& RHS) const
	{
		return GetPercent() >= RHS.GetPercent();
	}
	bool operator==(const Battery& RHS) const
	{
		return FMath::IsNearlyEqual(GetPercent(), RHS.GetPercent());
	}
	bool operator<(const Battery& RHS) const
	{
		return GetPercent() < RHS.GetPercent();
	}

private:
	float Percent{1.0f};
	void SetPercent(float PercentIn) {Percent = FMath::Clamp(PercentIn, 0.0f, 1.0f);}
};
