#include "GameFramework/CharacterMovementComponent.h"
#if (WITH_DEV_AUTOMATION_TESTS || WITH_PERF_AUTOMATION_TESTS) //Code will only compile if these configurations are != 0

#include "GameplayTests.h"
#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"
#include "TestUtils.h"
#include "Kismet/GameplayStatics.h"
#include "AutomationTestsProj/Items/InventoryItem.h"
#include "GameFramework/Character.h"
#include "Components/InputComponent.h"

DEFINE_LATENT_AUTOMATION_COMMAND_ONE_PARAMETER(FJumpLatentCommand, UInputComponent*, InputComponent);
bool FJumpLatentCommand::Update()
{
	JumpPressed(InputComponent);
	return true;
}

DEFINE_LATENT_AUTOMATION_COMMAND_ONE_PARAMETER(FJumpingLatentCommand, ACharacter*, Character);
bool FJumpingLatentCommand::Update()
{
	Character->Jump();
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FItemTakenOnJump, "Gameplay.ItemTakenOnJump", EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter) //Product tests are under the Standard Tests 
bool FItemTakenOnJump::RunTest(const FString& Parameters)
{
	AddInfo("Taking an item after jumping");

	const auto Level = LevelScope("/Game/Test/InventoryTestMap1");

	UWorld* World = GetTestGameWorld();
	if(!TestNotNull("World does not exist", World)) return false;

	ACharacter* Character = UGameplayStatics::GetPlayerCharacter(World, 0);
	if(!TestNotNull("Character does not exist", Character)) return false;

	TArray<AActor*> InventoryItems;
	UGameplayStatics::GetAllActorsOfClass(World, AInventoryItem::StaticClass(), InventoryItems); //Store all inventory items in the world in the array
	if(!TestEqual("Only one item should exist", InventoryItems.Num(), 1)) return false;

	ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(1.0f)); //Added for viewing reasons
	//Handle Jumping:
	ADD_LATENT_AUTOMATION_COMMAND(FJumpingLatentCommand(Character))
	ADD_LATENT_AUTOMATION_COMMAND(FDelayedFunctionLatentCommand([=, this]()
	{
		TArray<AActor*> InventoryItems;
		UGameplayStatics::GetAllActorsOfClass(World, AInventoryItem::StaticClass(), InventoryItems);
		TestTrueExpr(InventoryItems.Num() == 0);
	}, 2.0f)); //call function after 2 seconds since jumping is not instant 

	
	
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FItemCantReachOnJump, "Gameplay.ItemCantReachOnJump", EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter) //Product tests are under the Standard Tests 
bool FItemCantReachOnJump::RunTest(const FString& Parameters)
{
	AddInfo("Item too high to reach on jump, is not added to inventory");

	const auto Level = LevelScope("/Game/Test/InventoryTestMap2");

	UWorld* World = GetTestGameWorld();
	if(!TestNotNull("World does not exist", World)) return false;

	ACharacter* Character = UGameplayStatics::GetPlayerCharacter(World, 0);
	if(!TestNotNull("Character does not exist", Character)) return false;

	TArray<AActor*> InventoryItems;
	UGameplayStatics::GetAllActorsOfClass(World, AInventoryItem::StaticClass(), InventoryItems); //Store all inventory items in the world in the array
	if(!TestEqual("Only one item should exist", InventoryItems.Num(), 1)) return false;

	ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(1.0f)); //Added for viewing reasons
	//Handle Jumping:
	ADD_LATENT_AUTOMATION_COMMAND(FJumpingLatentCommand(Character))
	ADD_LATENT_AUTOMATION_COMMAND(FDelayedFunctionLatentCommand([=, this]()
	{
		TArray<AActor*> InventoryItems;
		UGameplayStatics::GetAllActorsOfClass(World, AInventoryItem::StaticClass(), InventoryItems);
		TestTrueExpr(InventoryItems.Num() == 1);
	}, 2.0f)); //call function after 2 seconds since jumping is not instant 

	
	
	return true;
}

#endif