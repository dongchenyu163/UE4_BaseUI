// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseClassesAndTypes/FunctionHandlerBase.h"
#include "FunctionHandlerInfo.generated.h"


USTRUCT(BlueprintType, Category="Default")
/**
 * @brief 
 * 
 */
struct BASEUI_MODULE_API FFunctionHandlerInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="FunctionHandlerInfo", meta=(AllowedClasses="ULowLevelFunctionHandlerBase, UWorldLevelHandlerBase", ExactClass="false"))
	TSubclassOf<UFunctionHandlerBase> HandlerClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="FunctionHandlerInfo")
	TSet<FName> DependencyHandlerNameCollection;

	FFunctionHandlerInfo(){}
	FFunctionHandlerInfo(UClass* InHandlerClass)
	{
		HandlerClass = InHandlerClass;
	}

	FFunctionHandlerInfo(UClass* InHandlerClass, TSet<FName> InDependencyNameList)
	{
		HandlerClass = InHandlerClass;
		DependencyHandlerNameCollection = InDependencyNameList;
	}
};
