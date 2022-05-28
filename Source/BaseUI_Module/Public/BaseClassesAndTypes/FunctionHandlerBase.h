// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
// #include "GameplayClasses/GameInstanceBase/Interfaces/I_GI_MenuFramework.h"
#include "UObject/Object.h"
#include "FunctionHandlerBase.generated.h"

/**
 * 提供一个成本较低的【UFunctionHandlerBase】子类的类型的判断方法。
 * 不用调用UStruct::IsChildOf()
 */
UENUM()
enum EFunctionHandlerType
{
	WorldHandler,
	LowLevelHandler
};
class II_GI_MenuFramework;
/**
 * 
 */
UCLASS(Abstract)
class BASEUI_MODULE_API UFunctionHandlerBase : public UObject
{
	GENERATED_BODY()

	/**
	 * @brief 让GameInstance可以拿到该Handler的名称，用于创建【Handler名称】->【Handler对象】
	 *		的字典；也方便GameInstance暴露接口函数，方便在任何时候任何地点（蓝图或者其他Handler中）
	 *		可以拿到【Handler对象】
	 * @return 
	 */
	virtual FName GetHandlerFName() { return FName(); }

	/**
	 * @brief 某个Handler依赖的Handler名称列表
	 * @return 
	 */
	virtual TArray<FName> GetDependenceHandlerFNameList() { return TArray<FName>(); }

	/**
	 * @brief 由GameInstance 在创建完成本Handler的依赖Handler对象后，进行调用，并传入依赖的
	 */
	virtual void InitHandler(II_GI_MenuFramework* InGameInstancePtr) {}

	virtual EFunctionHandlerType GetHandlerType() { return HandlerType; }

	/**
	 * @brief 转发GameInstance的同名函数给子类。
	 */
	virtual void OnStart() {}
	/**
	 * @brief 转发GameInstance的同名函数给子类。
	 * @param OldWorld 之前的世界
	 * @param NewWorld 新的世界
	 */
	virtual void OnWorldChanged(UWorld* OldWorld, UWorld* NewWorld) {}


protected:
	EFunctionHandlerType HandlerType;
};
