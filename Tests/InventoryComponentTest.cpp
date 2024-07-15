#if (WITH_DEV_AUTOMATION_TESTS || WITH_PERF_AUTOMATION_TESTS) //Code will only compile if these configurations are != 0

#include "InventoryComponentTest.h"
#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"
#include "AutomationTestsProj/Components/InventoryComponent.h"
#include "..\HelperTypes.h"

//Class to access protected values
namespace 
{
	class UInventoryComponentTestTable : public UInventoryComponent
	{
	public:
		void SetLimits(const TMap<EInventoryItemType, int32>& Limits) {InventoryLimits = Limits;}
	};
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FComponentCreation, "Components.Inventory.ComponentCreation", EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter) //Product tests are under the Standard Tests 
bool FComponentCreation::RunTest(const FString& Parameters)
{
	AddInfo("Inventory Component Creation Test"); 

	const UInventoryComponent* InventoryComponent = NewObject<UInventoryComponent>();
	if(!TestNotNull("Inventory Component Does Not Exist", InventoryComponent)) return false;
	
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FItemScoreZeroByDefault, "Components.Inventory.ItemScoreZeroByDefault", EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter) //Product tests are under the Standard Tests 
bool FItemScoreZeroByDefault::RunTest(const FString& Parameters)
{
	AddInfo("Item Score Must be Zero by Default"); 

	const UInventoryComponent* InventoryComponent = NewObject<UInventoryComponent>();
	if(!TestNotNull("Inventory Component Does Not Exist", InventoryComponent)) return false;

	/*
	TestTrueExpr(InventoryComponent->GetInventoryAmountByType(EInventoryItemType::CUBE) == 0);
	TestTrueExpr(InventoryComponent->GetInventoryAmountByType(EInventoryItemType::SPHERE) == 0);
	TestTrueExpr(InventoryComponent->GetInventoryAmountByType(EInventoryItemType::CYLINDER) == 0);
	*/
	
	const UEnum* InvEnum = StaticEnum<EInventoryItemType>();
	check(InvEnum);
	for(int32 i = 0; i < InvEnum->NumEnums() - 1; ++i)
	{
		const EInventoryItemType EnumElement = static_cast<EInventoryItemType>(i);
		TestTrueExpr(InventoryComponent->GetInventoryAmountByType(EnumElement) == 0);
	}
	
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FNegativeScoreNotAdded, "Components.Inventory.FNegativeScoreNotAdded", EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter) //Product tests are under the Standard Tests 
bool FNegativeScoreNotAdded::RunTest(const FString& Parameters)
{
	AddInfo("Negative Item Score Must Not Be Added"); 

	UInventoryComponent* InventoryComponent = NewObject<UInventoryComponent>();

	//Test that adding negative score returns false and that said score is not added
	TestTrueExpr(!InventoryComponent->AddItem({EInventoryItemType::SPHERE, -120}));
	TestTrueExpr(InventoryComponent->GetInventoryAmountByType(EInventoryItemType::SPHERE) == 0);
	
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FPositiveScoreAdded, "Components.Inventory.FPositiveScoreAdded", EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter) //Product tests are under the Standard Tests 
bool FPositiveScoreAdded::RunTest(const FString& Parameters)
{
	AddInfo("Positive Score Should Be Added When Under The Limits"); 

	UInventoryComponentTestTable* InventoryComponent = NewObject<UInventoryComponentTestTable>();

	TMap<EInventoryItemType, int32> InvLimits;

	InvLimits.Add(EInventoryItemType::CUBE, 100);
	InvLimits.Add(EInventoryItemType::SPHERE, 100);
	InvLimits.Add(EInventoryItemType::CYLINDER, 100);

	InventoryComponent->SetLimits(InvLimits);
	
	TestTrueExpr(InventoryComponent->AddItem({EInventoryItemType::SPHERE, 5}));
	TestTrueExpr(InventoryComponent->GetInventoryAmountByType(EInventoryItemType::SPHERE) == 5);

	
	return true;
}

#endif