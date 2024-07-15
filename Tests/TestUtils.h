#pragma once
#include "Tests/AutomationCommon.h"

namespace AutomationGame
{
	//Create template struct for tests
	template <typename Type1, typename Type2>
	struct TestPayload
	{
		Type1 TestValue;
		Type2 ExpectedValue;
	};
}

class LevelScope
{
public:
	LevelScope(const FString& MapName) {AutomationOpenMap(MapName);}
	~LevelScope() {ADD_LATENT_AUTOMATION_COMMAND(FExitGameCommand);}
};

UWorld* GetTestGameWorld();

void CallFuncByNameWithParams(UObject* Object, const FString& FuncName, const TArray<FString>& Params);

#define ENUM_LOOP_START(TYPE, EnumElem)                                        \
for (int32 Index = 0; Index < StaticEnum<TYPE>()->NumEnums() - 1; ++Index) \
{                                                                          \
const auto EnumElem = static_cast<TYPE>(StaticEnum<TYPE>()->GetValueByIndex(Index));
#define ENUM_LOOP_END }

template <typename T>
T* CreateBlueprint(UWorld* World, const FString& Name, const FTransform& Transform = FTransform::Identity)
{
	const UBlueprint* Blueprint = LoadObject<UBlueprint>(nullptr, *Name);
	return (World && Blueprint) ? World->SpawnActor<T>(Blueprint->GeneratedClass, Transform) : nullptr;
}

template <typename T>
T* CreateBlueprintDeferred(UWorld* World, const FString& Name, const FTransform& Transform = FTransform::Identity)
{
	const UBlueprint* Blueprint = LoadObject<UBlueprint>(nullptr, *Name);
	return (World && Blueprint) ? World->SpawnActorDeferred<T>(Blueprint->GeneratedClass, Transform) : nullptr;
}

//input:

int32 GetActionBindingIndexByName(UInputComponent* InputComp, const FString& ActionName, EInputEvent InputEvent);

void DoInputAction(UInputComponent* InputComponent, const FString& ActionName, const FKey& Key);

void JumpPressed(UInputComponent* InputComponent);