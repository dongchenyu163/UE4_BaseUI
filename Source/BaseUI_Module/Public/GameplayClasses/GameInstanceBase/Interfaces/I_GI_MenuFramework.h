// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseClassesAndTypes/FunctionHandlerBase.h"
#include "WorldLevelHandler/UI_Modules/UI_MapSelection/Interfaces/I_UI_MapSelectable.h"
#include "WorldLevelHandler/UI_Modules/UI_NextLevel/Interfaces/I_UI_NextLevel.h"
#include "WorldLevelHandler/UI_Modules/UI_ResumeMenu/Interfaces/I_UI_ResumeMenu.h"
#include "WorldLevelHandler/UI_Modules/UI_Save/Interfaces/I_UI_Savable.h"
#include "WorldLevelHandler/UI_Modules/UI_SinglePlayerMenu/Interfaces/I_UI_SinglePlayerFunctions.h"
#include "UObject/Interface.h"
#include "LowLevelHandler/UserManagement/Interfaces/I_UserManager.h"
#include "I_GI_MenuFramework.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAnyWorldBeginPlay);

// This class does not need to be modified.
UINTERFACE(BlueprintType)
class UI_GI_MenuFramework : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class BASEUI_MODULE_API II_GI_MenuFramework
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	/*******************************
	 * BaseHandler对象获取函数区域
	 *******************************/
#pragma region 功能函数区域
	// UResumeMenuBaseHandler
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="GameInstance|MenuFramework|BaseFunction", meta=(ToolTip="返回主菜单时用"))
	TSoftObjectPtr<UWorld> GetMainMenuMap();
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="GameInstance|MenuFramework|BaseFunction")
	UDataTable* GetMapInfoDataTable();
	// USaveBaseHandler
	virtual UDataTable* GetMapInfoDataTable_CPP() = 0;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="GameInstance|MenuFramework|BaseFunction")
	FMapUIInfo GetMapUIInfo(FName InMapIdentifier);
	// UNextLevelBaseHandler  UAC_CTRL_MenuBase
	virtual FMapUIInfo* GetMapUIInfo_CPP(FName InMapIdentifier) = 0;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="GameInstance|MenuFramework|BaseFunction", meta=(ToolTip="USaveBaseHandler保存UserGlobalData的时候需要的地图进度信息"))
	TMap<FName, FMapUIRuntimeInfo> GetUIRuntimeInfoTMap();
	virtual TMap<FName, FMapUIRuntimeInfo>* GetUIRuntimeInfoTMap_CPP() = 0;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="GameInstance|MenuFramework|BaseFunction")
	FMapUIRuntimeInfo GetMapUIRuntimeInfo(FName InMapIdentifier);
	virtual FMapUIRuntimeInfo* GetMapUIRuntimeInfo_CPP(FName InMapIdentifier) = 0;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="GameInstance|MenuFramework|BaseFunction", meta=(ToolTip="通过UserGlobalData来更新GameInstance里面的成员"))
	void UpdateFromUserGlobalData();
	virtual void UpdateFromUserGlobalData_CPP() = 0;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="GameInstance|MenuFramework|BaseFunction")
	FMapUIInfo GetPlayingMapUIInfo();
	// USaveBaseHandler UNextLevelBaseHandler
	virtual FMapUIInfo* GetPlayingMapUIInfo_CPP() = 0;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="GameInstance|MenuFramework|BaseFunction")
	void SetPlayingMapUIInfo(FMapUIInfo InNewMapInfo);
	// UMapSelectionBaseHandler
	virtual void SetPlayingMapUIInfo_CPP(const FMapUIInfo* InNewMapInfo) = 0;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="GameInstance|MenuFramework|BaseFunction", meta=(ToolTip="重置UDA_WidgetInfo中的WidgetRef变量，切换地图的时候该变量会被置空"))
	void ResetWidgetInfo();
	// UMapSelectionBaseHandler
	virtual void ResetWidgetInfo_CPP() = 0;

	virtual FOnAnyWorldBeginPlay* GetOnAnyWorldBeginPlayDelegate_CPP() = 0;
#pragma endregion 功能函数区域

	
	/*******************************
	 * BaseHandler对象获取函数区域
	 *******************************/
#pragma region BaseHandler对象获取函数区域
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="GameInstance|MenuFramework|Handler Getter")
	TScriptInterface<II_UI_SinglePlayerFunctions> GetSinglePlayerMenuBaseHandler();
	virtual II_UI_SinglePlayerFunctions* GetSinglePlayerMenuBaseHandler_CPP() = 0;
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="GameInstance|MenuFramework|Handler Getter")
	TScriptInterface<II_UI_MapSelectable> GetMapSelector();
	virtual II_UI_MapSelectable* GetMapSelector_CPP() = 0;
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="GameInstance|MenuFramework|Handler Getter")
	TScriptInterface<II_UI_ResumeMenu> GetResumeMenuBaseHandler();
	virtual II_UI_ResumeMenu* GetResumeMenuBaseHandler_CPP() = 0;
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="GameInstance|MenuFramework|Handler Getter")
	TScriptInterface<II_UI_Savable> GetSavableBaseHandler();
	virtual II_UI_Savable* GetSavableBaseHandler_CPP() = 0;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="GameInstance|MenuFramework|Handler Getter")
	TScriptInterface<II_UI_NextLevel> GetNextLevelHandler();
	virtual II_UI_NextLevel* GetNextLevelHandler_CPP() = 0;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="GameInstance|MenuFramework|Handler Getter")
	TScriptInterface<II_UserManager> GetUserManager();
	virtual II_UserManager* GetUserManager_CPP() = 0;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="FunctionHandler", meta=(DisplayName="", Tooltip=""))
	UFunctionHandlerBase* FindHandler_ByName(FName InHandlerName);
	virtual UFunctionHandlerBase* FindHandler_ByName_CPP(FName InHandlerName) = 0;

#pragma endregion BaseHandler对象获取函数区域
};
