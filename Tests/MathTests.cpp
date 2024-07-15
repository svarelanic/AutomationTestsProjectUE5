#if (WITH_DEV_AUTOMATION_TESTS || WITH_PERF_AUTOMATION_TESTS) //Code will only compile if these configurations are != 0

#include "MathTests.h"
#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FMathMaxInt, "MathTests.MathMaxInt", EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter) //Product tests are under the Standard Tests 
bool FMathMaxInt::RunTest(const FString& Parameters)
{
	AddInfo("Max func testing"); //log msg to test result

	TestTrue("2 different positive numbers", FMath::Max(15,40) == 40);

	TestEqual("2 equal positive numbers", FMath::Max(25,25), 25);

	TestTrueExpr(FMath::Max(0, 200) == 200); //Returns test failed and logs error if expr is false

	
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FMathSqrt, "MathTests.MathSqrt", EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)
bool FMathSqrt::RunTest(const FString& Parameters)
{
	AddInfo("Sqrt func testing");

	TestEqual("Sqrt(4) [0]", FMath::Sqrt(4.0f), 2.0f);
	TestEqual("Sqrt(3) [2]", FMath::Sqrt(3.0f), 1.7f, 0.1f); //Tolerance of 0.1 on the result

	return true;
}

#endif