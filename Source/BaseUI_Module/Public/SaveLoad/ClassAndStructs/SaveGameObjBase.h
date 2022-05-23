// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DongSerializeStatic.h"
#include "GameFramework/SaveGame.h"
#include "SaveGameObjBase.generated.h"

class II_GI_MenuFramework;  // 防止循环引用，这里没法添加头文件。

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class BASEUI_MODULE_API USaveGameObjBase : public USaveGame
{
	GENERATED_BODY()

public:
	// UFUNCTION(BlueprintNativeEvent, Category="Save|SaveGameObject")
	// void FillDataToSaveGameObject();
	//
	// UPROPERTY(SaveGame, meta=(ToolTip="所有要保存的物体的数据；反射系统不支持嵌套数组，只能用1维数组来存储所有的"))
	// TArray<uint8> LevelObjectByteArray;
	//
	// UPROPERTY(SaveGame, meta=(ToolTip="每个物体的数据的头地址（序号），用于恢复"))
	// TArray<uint64> LevelObjectHeaderArray;
	//
	// UPROPERTY(SaveGame, meta=(ToolTip="所有要保存的物体的数据；反射系统不支持嵌套数组，只能用1维数组来存储所有的"))
	// TArray<FActorSavingInfo> LevelObjectInfoList;
	II_GI_MenuFramework* GetFrameworkGameInstance_CPP();

	virtual void Serialize(FArchive& Ar) override
	{
		FDongSerializePropertyParams Param(this, this->GetClass());
		Param.SetMode_SaveGame_SearchSuper();
		Param.SetStopCondition_Type1(USaveGameObjBase::StaticClass(), true);
		
		UDongSerializeStatic::SerializePropertyWithFixedSeq(Ar, Param);
	}
};
