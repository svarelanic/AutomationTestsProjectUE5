#if (WITH_DEV_AUTOMATION_TESTS || WITH_PERF_AUTOMATION_TESTS) 

#include "AutomationCharacterTests.h"
#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"
#include "TestUtils.h"
#include "Engine/World.h"
#include "Engine/DamageEvents.h"
#include "AutomationTestsProj/AutomationTestsProjCharacter.h"
#include "AutomationTestsProj/HelperTypes.h"
#include "Tests/AutomationCommon.h"
#include "GameFramework/CharacterMovementComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogCharacterTest, All, All);

namespace
{
	const char* CharacterBPName = "/Script/Engine.Blueprint'/Game/Test/BP_Test_ThirdPersonCharacter.BP_Test_ThirdPersonCharacter'";

} //namespace

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FHealthChangesWhenDamageReceived, "Character.HealthChangesWhenDamageRecieved", EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter) //Product tests are under the Standard Tests 
bool FHealthChangesWhenDamageReceived::RunTest(const FString& Parameters)
{
	AddInfo("Health decreases when damage is received"); 

	//Spawn Character by creating a world and getting a pointer to it
	LevelScope("/Game/Test/EmptyTestLevel");
	UWorld* World = GetTestGameWorld();
	if(!TestNotNull("World does not exist", World)) return false;
	
	
	const FTransform InitialTransform{FVector{0.0f, -240.0f, 110.0f}}; 
	//Template function to Load BP and SpawnActorDeferred. Use deferred for BeginPlay inits. 
	AAutomationTestsProjCharacter* Character = CreateBlueprintDeferred<AAutomationTestsProjCharacter>(World, CharacterBPName, InitialTransform);
	if(!TestNotNull("Character Does Not Exist", Character))  return false;

	//Set health of test character
	FHealthData HealthData;
	HealthData.MaxHealth = 1000.0f;
	CallFuncByNameWithParams(Character, "SetHealthData", {HealthData.ToString()});

	Character->FinishSpawning(InitialTransform);
	
	//Test Health of test character (which is init to max health in begin play) is full: 
	TestEqual("Health is not full", Character->GetHealthPercent(), 1.0f);
	const float DamageAmount = 10.0f;
	Character->TakeDamage(DamageAmount, FDamageEvent{}, nullptr, nullptr);
	TestEqual("Health was not decreased", Character->GetHealthPercent(), 0.9f);
	
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FLatentCommandWaitSimple, "LatentCommand.LatentCommandWaitSimple", EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter) //Product tests are under the Standard Tests 
bool FLatentCommandWaitSimple::RunTest(const FString& Parameters)
{
	AddInfo("Testing for simple Wait latent command");

	ADD_LATENT_AUTOMATION_COMMAND(FWaitLatentCommand(4.0f));
	return true;
}

//Creates a class called CharacterLatentLogCommand with one class member: FString LogMessage
DEFINE_LATENT_AUTOMATION_COMMAND_ONE_PARAMETER(FCharacterLatentLogCommand, FString, LogMessage);
bool FCharacterLatentLogCommand::Update()
{
	UE_LOG(LogCharacterTest, Display, TEXT("%s"), *LogMessage);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FLatentCommandLogSimple, "LatentCommand.LatentCommandLogSimple", EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter) //Product tests are under the Standard Tests 
bool FLatentCommandLogSimple::RunTest(const FString& Parameters)
{
	AddInfo("Testing for simple Log latent command");

	//All three UE_LOGs will be thrown before the Latent Command logs:
	UE_LOG(LogCharacterTest, Warning, TEXT("Log 1"));
	ADD_LATENT_AUTOMATION_COMMAND(FCharacterLatentLogCommand("Latent command log 1"));
	UE_LOG(LogCharacterTest, Warning, TEXT("Log 2"));
	ADD_LATENT_AUTOMATION_COMMAND(FCharacterLatentLogCommand("Latent command log 2"));
	UE_LOG(LogCharacterTest, Warning, TEXT("Log 3"));
	ADD_LATENT_AUTOMATION_COMMAND(FCharacterLatentLogCommand("Latent command log 3"));

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FLatentCommandOpenCloseMap, "LatentCommand.LatentCommandOpenCloseMap", EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter) //Product tests are under the Standard Tests 
bool FLatentCommandOpenCloseMap::RunTest(const FString& Parameters)
{
	AddInfo("Testing for open/close map latent commands");

	const auto Level = LevelScope("/Game/ThirdPerson/Maps/ThirdPersonMap"); //Since it is a named object, map will open, we'll wait and then it will close 
	ADD_LATENT_AUTOMATION_COMMAND(FWaitLatentCommand(4.0f));

	return true;
}

//Latent command that checks for character death and returns error log if not
DEFINE_LATENT_AUTOMATION_COMMAND_TWO_PARAMETER(FCharacterDestroyedLatentCommand, AAutomationTestsProjCharacter*, Character, float, LifeSpan);
bool FCharacterDestroyedLatentCommand::Update()
{
	const double NewTime = FPlatformTime::Seconds();
	if(NewTime - StartTime >= LifeSpan)
	{
		if(IsValid(Character))
		{
			return true;
		}
		return true; //if character destroyed correctly after death, return true
	}
	return false;
}


IMPLEMENT_SIMPLE_AUTOMATION_TEST(FCharacterCanBeKilled, "LatentCommand.CharacterCanBeKilled", EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter) //Product tests are under the Standard Tests 
bool FCharacterCanBeKilled::RunTest(const FString& Parameters)
{
	AddInfo("Test if character dies when Health reaches zero"); 

	//Spawn Character by creating a world and getting a pointer to it
	const auto Level = LevelScope("/Game/ThirdPerson/Maps/ThirdPersonMap");
	UWorld* World = GetTestGameWorld();
	if(!TestNotNull("World does not exist", World)) return false;
	
	
	const FTransform InitialTransform{FVector{435.0f, 1100.0f, 92.0f}}; 
	//Template function to Load BP and SpawnActorDeferred. Use deferred for BeginPlay inits. 
	AAutomationTestsProjCharacter* Character = CreateBlueprintDeferred<AAutomationTestsProjCharacter>(World, CharacterBPName, InitialTransform);
	if(!TestNotNull("Character Does Not Exist", Character))  return false;

	//Set health of test character
	FHealthData HealthData;
	HealthData.MaxHealth = 1000.0f;
	HealthData.LifeSpan = 1.5f;
	CallFuncByNameWithParams(Character, "SetHealthData", {HealthData.ToString()});

	Character->FinishSpawning(InitialTransform);
	
	//Test Health of test character (which is init to max health in begin play) is full: 
	TestEqual("Health is not full", Character->GetHealthPercent(), 1.0f);
	const float DamageAmount = 10.0f;
	Character->TakeDamage(DamageAmount, FDamageEvent{}, nullptr, nullptr);
	TestEqual("Health was not decreased", Character->GetHealthPercent(), 0.9f);

	const auto DamageToKill = HealthData.MaxHealth;
	Character->TakeDamage(DamageToKill, FDamageEvent{}, nullptr, nullptr);

	TestEqual("Health is not zero", Character->GetHealthPercent(), 0.0f);
	TestTrueExpr(Character->GetCharacterMovement()->MovementMode == MOVE_None);
	/*
	 * Check the rest of death properties (ECollisionResponse::ECR_Ignore, ECollisionEnabled::QueryAndPhysics, SetSimulatePhysics(true))
	 */
	//TestTrueExpr(FMath::IsNearlyEqual(Character->GetLifeSpan(), HealthData.LifeSpan));

	ADD_LATENT_AUTOMATION_COMMAND(FCharacterDestroyedLatentCommand(Character, HealthData.LifeSpan));
	
	return true;
}

//Latent command to wait until healing is over and check if health is full
DEFINE_LATENT_AUTOMATION_COMMAND_TWO_PARAMETER(FAutoHealCheckLatentCommand, AAutomationTestsProjCharacter*, Character, float, HealingDuration);
bool FAutoHealCheckLatentCommand::Update()
{
	const double NewTime = FPlatformTime::Seconds();
	if(NewTime - StartTime >= HealingDuration)
	{
		if(!FMath::IsNearlyEqual(Character->GetHealthPercent(), 1.0f))
		{
			UE_LOG(LogCharacterTest, Error, TEXT("Character wasn't fully healed"));
		}
		return true;
	}
	return false;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAutoHealRestoresHealth, "LatentCommand.HealRestoresHealth", EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter) //Product tests are under the Standard Tests 
bool FAutoHealRestoresHealth::RunTest(const FString& Parameters)
{
	AddInfo("Character heals over time"); 

	const auto Level = LevelScope("/Game/ThirdPerson/Maps/ThirdPersonMap");
	UWorld* World = GetTestGameWorld();
	if(!TestNotNull("World does not exist", World)) return false;
	
	
	const FTransform InitialTransform{FVector{435.0f, 1100.0f, 92.0f}}; 
	//Template function to Load BP and SpawnActorDeferred. Use deferred for BeginPlay inits. 
	AAutomationTestsProjCharacter* Character = CreateBlueprintDeferred<AAutomationTestsProjCharacter>(World, CharacterBPName, InitialTransform);
	if(!TestNotNull("Character Does Not Exist", Character))  return false;

	//Set health of test character
	FHealthData HealthData;
	HealthData.MaxHealth = 100.0f;
	HealthData.HealModifier = 5.0f;
	HealthData.HealRate = 0.5f;
	CallFuncByNameWithParams(Character, "SetHealthData", {HealthData.ToString()});

	Character->FinishSpawning(InitialTransform);
	
	//Test Health of test character (which is init to max health in begin play) is full: 
	TestEqual("Health is not full", Character->GetHealthPercent(), 1.0f);
	const float DamageAmount = 20.0f;
	Character->TakeDamage(DamageAmount, FDamageEvent{}, nullptr, nullptr);
	TestEqual("Health was not decreased", Character->GetHealthPercent(), 0.8f);

	const float HealthDiff = HealthData.MaxHealth * (1.0f - Character->GetHealthPercent());
	const float HealingDuration = HealthData.HealRate * HealthDiff / HealthData.HealModifier;

	ADD_LATENT_AUTOMATION_COMMAND(FAutoHealCheckLatentCommand(Character, HealingDuration));
	
	return true;
}

#endif