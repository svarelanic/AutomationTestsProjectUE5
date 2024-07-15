#if (WITH_DEV_AUTOMATION_TESTS || WITH_PERF_AUTOMATION_TESTS) //Code will only compile if these configurations are != 0

#include "ScienceFuncLibTest.h"
#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"
#include "AutomationTestsProj/Science/ScienceFuncLib.h"
#include "TestUtils.h"


IMPLEMENT_SIMPLE_AUTOMATION_TEST(FFibonacciSimple, "ScienceFuncLibTests.Fibonacci.Simple", EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter) //Product tests are under the Standard Tests 
bool FFibonacciSimple::RunTest(const FString& Parameters)
{
	AddInfo("Fibonacci Simple Test");

	//Init Data using TestUtils template
	const TArray<AutomationGame::TestPayload<int32, int32>> TestData{{0,0}, {1,1}, {2,1},{3,2}, {4,3}, {5,5}};
	for(const auto Data : TestData)
	{
		//TestTrueExpr(UScienceFuncLib::Fibonacci(Data.TestValue) == Data.ExpectedValue);

		const FString InfoString = FString::Printf(TEXT("Value: %i, Expected: %i"), Data.TestValue, Data.ExpectedValue);
		TestEqual(InfoString, UScienceFuncLib::Fibonacci(Data.TestValue), Data.ExpectedValue);
	}
	
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FFibonacciStress, "ScienceFuncLibTests.Fibonacci.Stress", EAutomationTestFlags::EditorContext | EAutomationTestFlags::StressFilter) //Product tests are under the Standard Tests 
bool FFibonacciStress::RunTest(const FString& Parameters)
{
	AddInfo("Fibonacci Stress Test");

	int32 PrevValue = 1;
	int32 PrevPrevValue = 0;
	for(int32 i = 2; i<40; ++i)
	{
		const int32 CurrentValue = UScienceFuncLib::Fibonacci(i);
		TestTrueExpr(CurrentValue == PrevPrevValue + PrevValue);

		PrevPrevValue = PrevValue;
		PrevValue = CurrentValue;
	}
		
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FFibonacciLogHasErrors, "ScienceFuncLibTests.Fibonacci.LogHasErrors", EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter) //Product tests are under the Standard Tests 
bool FFibonacciLogHasErrors::RunTest(const FString& Parameters)
{
	AddInfo("Fibonacci Negative Input Causes Error");

	//AddExpectedError will check the error log and, if it contains the specified text, will ignore said error
	//Without this line, the test would fail even though we want to test for Fibonacci failure
	AddExpectedError("Invalid input for Fibonacci", EAutomationExpectedErrorFlags::Contains);
	UScienceFuncLib::Fibonacci(-10);
	
	return true;
}


#endif