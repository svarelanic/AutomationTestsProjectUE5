
#if (WITH_DEV_AUTOMATION_TESTS || WITH_PERF_AUTOMATION_TESTS) //Code will only compile if these configurations are != 0

#include "InventoryItemTest.h"
#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"
#include "Tests/AutomationCommon.h"
#include "Components/SphereComponent.h"
#include "AutomationTestsProj/Items/InventoryItem.h"

namespace 
{
	const char* InventoryItemBPName = "/Script/Engine.Blueprint'/Game/Inventory/BP_InventoryItem.BP_InventoryItem'";
	
	UWorld* GetTestGameWorld()
	{
		const TIndirectArray<FWorldContext>& WorldContexts = GEngine->GetWorldContexts();
		for ( const FWorldContext& Context : WorldContexts )
		{
			if ( ( ( Context.WorldType == EWorldType::PIE ) || ( Context.WorldType == EWorldType::Game ) ) && ( Context.World()) )
			{
				return Context.World();
				
			}
		}

		return nullptr;
	}

	//To close the OpenMap everytime it leaves the scope: 
	class LevelScope
	{
	public:
		LevelScope(const FString& MapName) {AutomationOpenMap(MapName);}
		~LevelScope() {ADD_LATENT_AUTOMATION_COMMAND(FExitGameCommand);}
	};

}//namespace


IMPLEMENT_SIMPLE_AUTOMATION_TEST(FCppActorCanBeCreated, "Items.Inventory.CppActorCanBeCreated", EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter) //Product tests are under the Standard Tests 
bool FCppActorCanBeCreated::RunTest(const FString& Parameters)
{
	AddInfo("Inventory Component Creation Test"); 

	//To spawn actor, we need a pointer to the game world and, for the game world, we need to open a level
	AutomationOpenMap("/Game/Test/EmptyTestLevel");

	UWorld* World = GetTestGameWorld();
	if(!TestNotNull("World Does Not Exist", World)) return false;

	const FTransform InitialTransform{FVector{1000.0f}}; 
	const AInventoryItem* InventoryItem = World->SpawnActor<AInventoryItem>(AInventoryItem::StaticClass(), InitialTransform);
	if(!TestNotNull("Inventory Item Does Not Exist", InventoryItem)) return false;
	
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FCollisionSetup, "Items.Inventory.FCollisionSetup", EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter) //Product tests are under the Standard Tests 
bool FCollisionSetup::RunTest(const FString& Parameters)
{
	AddInfo("Inventory Component Creation Test"); 

	//To spawn actor, we need a pointer to the game world and, for the game world, we need to open a level
	AutomationOpenMap("/Game/Test/EmptyTestLevel");

	UWorld* World = GetTestGameWorld();
	if(!TestNotNull("World Does Not Exist", World)) return false;

	//Load Blueprint on the Test Map
	const UBlueprint* Blueprint = LoadObject<UBlueprint>(nullptr, *FString(InventoryItemBPName));
	if(!TestNotNull("Blueprint Does Not Exist", Blueprint)) return false;

	
	const FTransform InitialTransform{FVector{1000.0f}}; 
	const AInventoryItem* InventoryItem = World->SpawnActor<AInventoryItem>(Blueprint->GeneratedClass, InitialTransform);
	if(!TestNotNull("Inventory Item Does Not Exist", InventoryItem))  return false;

	const auto CollisionComponent = InventoryItem->FindComponentByClass<USphereComponent>();
	if(!TestNotNull("Collision Component Does Not Exist", CollisionComponent)) return false;

	TestTrueExpr(CollisionComponent->GetUnscaledSphereRadius() >= 30.0f);
	TestTrueExpr(CollisionComponent->GetCollisionEnabled() == ECollisionEnabled::QueryOnly);
	/*
	 Should check for the rest of the Collision Component setup
	 */
	
	
	return true;
}

#endif