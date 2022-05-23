// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GUI_FrameWork/UI_Modules/UI_MapSelection/Interfaces/I_UI_MapSelectable.h"
#include "SaveLoad/ClassAndStructs/UserGlobalDataSaveGameObjBase.h"
#include "UObject/Object.h"
#include "TestUserGlobalSaveGameBaseObj.generated.h"


USTRUCT(BlueprintType, meta=(ToolTip="测试用，固化到DataTable"))
struct FMapUserInGameData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame, Category="", meta=(Tooltip="（原则上，运行时不可更改）仅是为了唯一标识或者寻找FMapUIRuntimeInfo对象"))
	FName MapIdentifier;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Map InGame Info", meta=(Tooltip="某关卡的评价等级。"))
	int Rank = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Map InGame Info", meta=(Tooltip="某关卡的评价等级。"))
	float MaxScore = 0;
	
};

/**
 * 
 */
UCLASS(BlueprintType)
class BASEUI_MODULE_API UTestUserGlobalSaveGameBaseObj : public UUserGlobalDataSaveGameObjBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, SaveGame, BlueprintReadWrite, Category="Save|UserGlobalData", meta=(ToolTip="记录地图的解锁情况"))
	TMap<FName, FMapUIRuntimeInfo> UserMapsInfo;
	
	virtual void DoUpdateUserGlobalData_Implementation() override;
};
