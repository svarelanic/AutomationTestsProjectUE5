#include "TestUtils.h"
#include "Misc/OutputDeviceNull.h"


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

void CallFuncByNameWithParams(UObject* Object, const FString& FuncName, const TArray<FString>& Params)
{
	if (!Object) return;

	// Command pattern: "FunctionName Param1 Param2 Param3"
	FString Command = FuncName;
	for (const auto Param : Params)
	{
		Command.Append(" ").Append(Param);
	}
	FOutputDeviceNull OutputDeviceNull;
	Object->CallFunctionByNameWithArguments(*Command, OutputDeviceNull, nullptr, true);
}

//Input -> Not working with enhanced input

int32 GetActionBindingIndexByName(UInputComponent* InputComp, const FString& ActionName, EInputEvent InputEvent)
{
	if (!InputComp) return INDEX_NONE;

	for (int32 i = 0; i < InputComp->GetNumActionBindings(); ++i)
	{
		const FInputActionBinding Action = InputComp->GetActionBinding(i);
		if (Action.GetActionName().ToString().Equals(ActionName) && Action.KeyEvent == InputEvent)
		{
			return i;
		}
	}

	return INDEX_NONE;
}

void DoInputAction(UInputComponent* InputComponent, const FString& ActionName, const FKey& Key)
{
	if (!InputComponent) return;

	const int32 ActionIndex = GetActionBindingIndexByName(InputComponent, ActionName, EInputEvent::IE_Pressed);
	if (ActionIndex != INDEX_NONE)
	{
		const auto ActionBind = InputComponent->GetActionBinding(ActionIndex);
		ActionBind.ActionDelegate.Execute(Key);
	}
}

void JumpPressed(UInputComponent* InputComponent)
{
	DoInputAction(InputComponent, "jump", EKeys::SpaceBar);
}