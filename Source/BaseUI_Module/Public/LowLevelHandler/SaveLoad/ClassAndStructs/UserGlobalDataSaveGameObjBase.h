// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MapInfo.h"
#include "SaveGameObjBase.h"
#include "UObject/Object.h"
#include "UserGlobalDataSaveGameObjBase.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType, meta=(ToolTip="这个类不包含任何成员，应当继承之后添加成员。本类用在USaveBaseHandler类中，只是为了方便筛选子类"))
class BASEUI_MODULE_API UUserGlobalDataSaveGameObjBase : public USaveGameObjBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="", meta=(ToolTip="子类实现，更新用户进度；在保存用户进度之前会被调用，即【USaveBaseHandler::SaveUserGlobalData_CPP()】函数"))
	void DoUpdateUserGlobalData();
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="", meta=(ToolTip="子类实现，切换用户时候会清除原来的用户进度"))
	void DoClearUserGlobalData();

	UPROPERTY(EditAnywhere, SaveGame, BlueprintReadWrite, Category="Save|UserGlobalData", meta=(ToolTip="记录地图的解锁情况"))
	TMap<FName, FMapInfo_User> UserMapsRuntimeInfo;
	
};
