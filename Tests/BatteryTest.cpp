#if (WITH_DEV_AUTOMATION_TESTS || WITH_PERF_AUTOMATION_TESTS) //Code will only compile if these configurations are != 0

#include "BatteryTest.h"
#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"
#include "AutomationTestsProj/Items/Battery.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FBatteryTests, "Items.Battery", EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter) //Product tests are under the Standard Tests 
bool FBatteryTests::RunTest(const FString& Parameters)
{
	AddInfo("Battery with default constructor");
	constexpr Battery BatteryDefault;
	TestTrueExpr(FMath::IsNearlyEqual(BatteryDefault.GetPercent(), 1.0f));	
	TestTrueExpr(BatteryDefault.GetColor() == FColor::Green);
	TestTrueExpr(BatteryDefault.ToString().Equals("100%"));	

	AddInfo("Battery with custom constructor");
	//With lambda expression
	const auto BatteryTestFunc = [this](float Percent, const FColor& Color, const FString& PercentString)
	{
		const Battery BatteryObject{Percent};
		TestTrueExpr(FMath::IsNearlyEqual(BatteryObject.GetPercent(), FMath::Clamp(Percent, 0.0f, 1.0f)));	
		TestTrueExpr(BatteryObject.GetColor() == Color);
		TestTrueExpr(BatteryObject.ToString().Equals(PercentString));
	};

	BatteryTestFunc(1.0f, FColor::Green, "100%");
	BatteryTestFunc(0.50f, FColor::Yellow, "50%");
	BatteryTestFunc(0.12f, FColor::Red, "12%");
	BatteryTestFunc(1000.0f, FColor::Green, "100%");
	BatteryTestFunc(-1000.0f, FColor::Red, "0%");

	AddInfo("Battery Charge/Discharge");
	Battery BatteryObject{0.6f};
	TestTrueExpr(FMath::IsNearlyEqual(BatteryObject.GetPercent(), 0.6f));	
	BatteryObject.Discharge();
	TestTrueExpr(FMath::IsNearlyEqual(BatteryObject.GetPercent(), 0.5f));	
	BatteryObject.Charge();
	TestTrueExpr(FMath::IsNearlyEqual(BatteryObject.GetPercent(), 0.6f));	

	AddInfo("Battery Charge/Discharge Edge Cases");
	for(int32 i = 0; i < 100; ++i)
	{
		BatteryObject.Discharge();
	}
	TestTrueExpr(FMath::IsNearlyEqual(BatteryObject.GetPercent(), 0.0f));	

	for(int32 i = 0; i < 100; ++i)
	{
		BatteryObject.Charge();
	}
	TestTrueExpr(FMath::IsNearlyEqual(BatteryObject.GetPercent(), 1.0f));	
	
	AddInfo("Battery Comparison");
	Battery BatteryHigh{0.9f};
	Battery BatteryLow{0.3f};

	TestTrueExpr(BatteryHigh >= BatteryLow); //Overload operator for comparison 
	TestTrueExpr(FMath::Max(BatteryHigh, BatteryLow) == BatteryHigh);

	AddInfo("Battery in memory Comparison");
	//Error thrown depending on memory address comparison
	TestNotSame("Not the same Batteries", BatteryLow, BatteryHigh);

	Battery& BatteryHighRef = BatteryHigh;
	TestSame("Same Batteries", BatteryHigh, BatteryHighRef);
	
	return true;
}

#endif