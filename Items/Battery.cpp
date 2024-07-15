#include "Battery.h"

constexpr float ChargeAmount = 0.1f;

Battery::Battery(float PercentIn)
{
	SetPercent(PercentIn);
}

void Battery::Charge()
{
	SetPercent(Percent + ChargeAmount);
}

void Battery::Discharge()
{
	SetPercent(Percent - ChargeAmount);
}

FColor Battery::GetColor() const
{
	if(Percent > 0.8f) return FColor::Green;
	if(Percent > 0.2f) return FColor::Yellow;
	return FColor::Red;
}

FString Battery::ToString() const
{
	return FString::Printf(TEXT("%i%%"), FMath::RoundToInt(Percent * 100));
}
