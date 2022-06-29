// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
// #include "GameplayClasses/GameInstanceBase/Interfaces/I_GI_MenuFramework.h"
#include "FunctionHandlerDef.h"
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

public:
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
	virtual TSet<UClass*> GetDependenceHandlerInterfaceCollection() { return TSet<UClass*>(); }
	virtual TMap<FString, UClass*> GetPurpose_To_DependenceHandlerDict() { return Map_Purpose_To_DependenceHandlerClass; }
	virtual FText GetPurposeTooltipText(FString InPurposeString) { return Map_Purpose_To_PurposeTooltip[InPurposeString]; }
	/**
	 * @brief 由GameInstance 在创建完成本Handler的依赖Handler对象后，进行调用，并传入依赖的
	 */
	virtual void InitHandler(II_GI_MenuFramework* InGameInstancePtr, TMap<FName, UFunctionHandlerBase*>& InDependencyHandlerDict)
	{
		checkf(GameInstancePtr!=nullptr, TEXT("Gameinstance CANNOT be nullptr!!"));
		GameInstancePtr = InGameInstancePtr;
	}

	virtual void AssignInterfacePtr(UObject* MatchedObjectPtr, UClass* MatchedInterfaceClassPtr) {}
	/**
	 * @brief （可选，子类实现）虽然有Map_Purpose_To_HandlerInstance可以直接引用Handler，但是我们还需要更短的名称来引用各个Handler。
	 *			子类中根据自己的需要将Map_Purpose_To_HandlerInstance变量中的Handler赋给某个变量，方便后续引用。
	 *			本函数在InitHandler函数的最后被调用。
	 */
	virtual void AssignDependentHandlerPtr() {}

	virtual EFunctionHandlerType GetHandlerType() { return HandlerType; }

	virtual void Get_Map_Purpose_To_DependenceHandlerClass(TMap<FString, UClass*>& Out_Map_Purpose_To_DependenceHandlerClass)
	{
		Out_Map_Purpose_To_DependenceHandlerClass = Map_Purpose_To_DependenceHandlerClass;
	}
protected:
	/**
	 * @brief 游戏本身开始的时候被调用；转发GameInstance的同名函数给子类。
	 */
	virtual void OnStart() {}
	/**
	 * @brief 有程序调用了【UGameplayStatics::OpenLevel】函数进行世界的切换，且切换已经完成的时候被调用；转发GameInstance的同名函数给子类。
	 * @param OldWorld 之前的世界
	 * @param NewWorld 新的世界
	 */
	virtual void OnWorldChanged(UWorld* OldWorld, UWorld* NewWorld) {}


public:
	FName GetHandlerName() const { return HandlerName; }
	void SetHandlerName(const FName& InHandlerName) { this->HandlerName = InHandlerName; }
protected:
	FName HandlerName = "";

protected:
	EFunctionHandlerType HandlerType;
	II_GI_MenuFramework* GameInstancePtr;
	TMap<FString, UFunctionHandlerBase*> Map_Purpose_To_HandlerInstance;

public:
	const static TMap<FString, UClass*> Map_Purpose_To_DependenceHandlerClass;
	const static TMap<FString, FText> Map_Purpose_To_PurposeTooltip;
	const static FFunctionHandlerDef HandlerDef;
};
