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

	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="FunctionHandlerInfo", meta=(AllowedClasses="ULowLevelFunctionHandlerBase, UWorldLevelHandlerBase", ExactClass="false"))
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="FunctionHandlerInfo")
	TSubclassOf<UFunctionHandlerBase> HandlerClass;

	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="FunctionHandlerInfo")
	TSet<FName> DependencyHandlerNameCollection;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(ToolTip="用于各个Handler初始化的时候，获取对应的；应当用ComboBox来编辑。"))
	TMap<FString, FName> Map_Purpose_To_HandlerName;

	// 验证Handler依赖的有效性时使用，只是读取，所以用指针效率更高。
	// 测试 【依赖的Handler的名称 所对应的 Handler类别】 和 【各个Handler类中定义的依赖Handler的类型】是否相同。
	TMap<FString, UClass*>* Map_Purpose_To_DependenceHandlerClassPtr = nullptr;

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

	// FFunctionHandlerInfo(UClass* InHandlerClass, TMap<FString, UClass*> InMap_Purpose_To_DependenceHandlerClass)
	// {
	// 	HandlerClass = InHandlerClass;
	// 	TArray<UClass*> ClassList;
	// 	InMap_Purpose_To_DependenceHandlerClass.GenerateValueArray(ClassList);
	// 	DependencyHandlerClassCollection = TSet<UClass*>(ClassList);
	// 	// DependencyHandlerNameCollection = InDependencyNameList;
	// }
	//
	// FFunctionHandlerInfo(UClass* InHandlerClass, TMap<FString, FName> InMap_Purpose_To_HandlerName)
	// {
	// 	HandlerClass = InHandlerClass;
	// 	TArray<FName> ClassList;
	// 	Map_Purpose_To_HandlerName = InMap_Purpose_To_HandlerName;
	// 	InMap_Purpose_To_HandlerName.GenerateValueArray(ClassList);
	// 	DependencyHandlerNameCollection = TSet<FName>(ClassList);
	// }

	// FFunctionHandlerInfo(UClass* InHandlerClass/, TMap<FString, UClass*>* InMap_Purpose_To_DependenceHandlerClassPtr, TMap<FString, FName> InMap_Purpose_To_HandlerName)
	FFunctionHandlerInfo(UClass* InHandlerClass, TMap<FString, FName> InMap_Purpose_To_HandlerName)
	{
		HandlerClass = InHandlerClass;
		Map_Purpose_To_HandlerName = InMap_Purpose_To_HandlerName;
		// Map_Purpose_To_DependenceHandlerClassPtr = InMap_Purpose_To_DependenceHandlerClassPtr;
		
		TArray<FName> ClassList;
		InMap_Purpose_To_HandlerName.GenerateValueArray(ClassList);
		DependencyHandlerNameCollection = TSet<FName>(ClassList);
	}
};
