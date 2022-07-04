// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"


class BASEUI_MODULE_API FFunctionHandlerDependent : public TSharedFromThis<FFunctionHandlerDependent>
{
public:
	FFunctionHandlerDependent(UClass* InDependentClass, const FText& InTooltip)
		: DependentClass(InDependentClass),
		  Tooltip(InTooltip)
	{
	}

	FFunctionHandlerDependent(UClass* InDependentClass)
		: DependentClass(InDependentClass) {}

	UClass* DependentClass = nullptr;
	FText Tooltip;
};

using HandlerDependentMap = TMap<FString, TSharedPtr<FFunctionHandlerDependent>>;
using HandlerDependentPair = TPair<FString, TSharedPtr<FFunctionHandlerDependent>>;
/**
 * 
 */
class BASEUI_MODULE_API FFunctionHandlerDef : public TSharedFromThis<FFunctionHandlerDef>
{
public:
	FFunctionHandlerDef() { HandlerClass =nullptr; }
	FFunctionHandlerDef(UClass* InHandlerClass)
		: HandlerClass(InHandlerClass)
	{
		Map_HandlerClass_To_Definition.Add(InHandlerClass);
	}
	FFunctionHandlerDef(UClass* InHandlerClass, TMap<FString, TSharedPtr<FFunctionHandlerDependent>> InDependentDict)
		: HandlerClass(InHandlerClass),
		  Map_Purpose_To_Dependent(InDependentDict)
	{
		Map_HandlerClass_To_Definition.Add(TPair<UClass*, FFunctionHandlerDef*>(InHandlerClass, this));
	}
	// ~FFunctionHandlerDef()
	// {
	// 	if (HandlerClass)
	// 	{
	// 		Map_HandlerClass_To_Definition.Remove(HandlerClass);
	// 	}
	// }

	static bool Get_Map_Purpose_To_Dependent(const UClass* InHandlerClass, HandlerDependentMap*& OutDependentMapPointer)
	{
		const auto DependentInfoPtr = Map_HandlerClass_To_Definition.Find(InHandlerClass);
		if (DependentInfoPtr == nullptr)
		{
			return false;
		}
		OutDependentMapPointer = &(*DependentInfoPtr)->Map_Purpose_To_Dependent;
		return true;
	}
	
	static void GetPurposeList_Static(const UClass* InDependentClass, TArray<FString> OutPurposeStringList)
	{
		const auto DependentInfoPtr = Map_HandlerClass_To_Definition.Find(InDependentClass);
		if (DependentInfoPtr == nullptr)
		{
			return;
		}
		(*DependentInfoPtr)->Map_Purpose_To_Dependent.GetKeys(OutPurposeStringList);
	}

	void GetPurposeList(const UClass* InDependentClass, TArray<FString> OutPurposeStringList) const
	{
		OutPurposeStringList.Empty();
		for (auto DependentIt = Map_Purpose_To_Dependent.CreateConstIterator(); DependentIt; ++DependentIt)
		{
			if (DependentIt->Value->DependentClass == InDependentClass)
			{
				OutPurposeStringList.Add(DependentIt->Key);
			}
		}
	}
	
	UClass* HandlerClass;
	TMap<FString, TSharedPtr<FFunctionHandlerDependent>> Map_Purpose_To_Dependent;


	static TMap<UClass*, FFunctionHandlerDef*> Map_HandlerClass_To_Definition;
	
};


