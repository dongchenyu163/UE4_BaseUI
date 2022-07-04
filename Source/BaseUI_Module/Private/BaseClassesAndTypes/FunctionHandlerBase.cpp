// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseClassesAndTypes/FunctionHandlerBase.h"

#include "GameplayClasses/GameInstanceBase/MainMenuGameInstanceConfig.h"


const FFunctionHandlerDef UFunctionHandlerBase::HandlerDef = FFunctionHandlerDef();

void UFunctionHandlerBase::InitHandler(II_GI_MenuFramework* InGameInstancePtr,
	TMap<FName, UFunctionHandlerBase*>& InDependencyHandlerDict)
{
	auto const Map_Purpose_To_HandlerName = UMainMenuGameInstanceConfig::Get()->HandlerClassDict[HandlerName].Map_Purpose_To_HandlerName;
	// Map_Purpose_To_HandlerName
	
	GameInstancePtr = InGameInstancePtr;

	// Map_Purpose_To_HandlerInstance
		
	TSet<UClass*> NotMatchInterfaceList(GetDependenceHandlerClassCollection());
	for (const auto Pair : InDependencyHandlerDict)
	{
		UClass* MatchedInterface = nullptr;
		for (const auto InterfacePtr : NotMatchInterfaceList)
		{
			if (Pair.Value->GetClass()->ImplementsInterface(InterfacePtr))
			{
				MatchedInterface = InterfacePtr;
				AssignInterfacePtr(Pair.Value, MatchedInterface);
				break;
			}
		}

		if (MatchedInterface && NotMatchInterfaceList.Num() > 1)
		{
			NotMatchInterfaceList.Remove(MatchedInterface);
		}
		
		const auto PurposeStringPtr = Map_Purpose_To_HandlerName.FindKey(Pair.Key);
		if (!PurposeStringPtr)
		{
			UE_LOG(LogTemp, Error, TEXT("Function:[%s] Didn't match the Purpose: [%s] with a handler name."), ANSI_TO_TCHAR(__FUNCTION__), **PurposeStringPtr);
			continue;
		}
		Map_Purpose_To_HandlerInstance.Add(*PurposeStringPtr, Pair.Value);
	}

	AssignDependentHandlerPtr();
}
