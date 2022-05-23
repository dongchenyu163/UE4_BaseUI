// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
// #include "I_UI_MapSelectable.h"
#include "Components/Widget.h"
#include "SaveLoad/ClassAndStructs/UserGlobalDataSaveGameObjBase.h"
#include "GUI_FrameWork/UI_Modules/UI_Save/UI/USaveSystemLoadingScreenBase.h"
#include "SaveLoad/Interfaces/I_Save.h"
#include "UObject/Interface.h"
#include "I_UI_Savable.generated.h"


// This class does not need to be modified.
UINTERFACE(Blueprintable, MinimalAPI)
class UI_UI_Savable : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class BASEUI_MODULE_API II_UI_Savable
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Save Load System|UI Functions|User Global", meta=(ToolTip="保存游戏本身的进度，例如：象棋残局游戏：哪些关卡通关了，获得那些成就；一个游戏的一个用户只需要一份的存档"))
	void SaveUserGlobalData();
	virtual void SaveUserGlobalData_CPP() = 0;
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Save Load System|UI Functions|User Global", meta=(ToolTip="读取游戏本身的进度"))
	void LoadUserGlobalData();
	virtual void LoadUserGlobalData_CPP() = 0;
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Save Load System|UI Functions|User Global", meta=(ToolTip="其他类获取游戏本身的进度的信息的函数，并使用该信息完成其他操作"))
	UUserGlobalDataSaveGameObjBase* GetUserGlobalData();
	virtual UUserGlobalDataSaveGameObjBase* GetUserGlobalData_CPP() = 0;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Save", meta=(Tooltip=""))
	void GetSaveDataHeaderList(ESaveArea_BP InSaveAreaID_Enum, TArray<USaveGameObjectHeaderBase*>& OutHeaderList);
	virtual void GetSaveDataHeaderList_CPP(FName InSaveAreaID, TArray<USaveGameObjectHeaderBase*>& OutHeaderList) = 0;
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="", meta=(Tooltip=""))
	UWidget* GetSaveSystemLoadingScreenWidget();
	virtual UWidget* GetSaveSystemLoadingScreenWidget_CPP() = 0;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="", meta=(Tooltip=""))
	void SetSaveSystemLoadingScreenWidgetClass(TSubclassOf<UUSaveSystemLoadingScreenBase> InLoadingScreenWidgetClass);
	virtual void SetSaveSystemLoadingScreenWidgetClass_CPP(TSubclassOf<UUSaveSystemLoadingScreenBase> InLoadingScreenWidgetClass) = 0;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Save Load System|UI Functions|Save Game")
	void SaveGame(int32 InSlotNum);
	virtual void SaveGame_CPP(int32 InSlotNum) = 0;
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Save Load System|UI Functions|Save Game")
	void SaveGame_Auto();
	virtual void SaveGame_Auto_CPP() = 0;
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Save Load System|UI Functions|Save Game")
	void SaveGame_Quick();
	virtual void SaveGame_Quick_CPP() = 0;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Save Load System|UI Functions|Save Game")
	void LoadGame(int32 InSlotNum);
	virtual void LoadGame_CPP(int32 InSlotNum) = 0;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Save Load System|UI Functions|Save Game")
	void LoadGame_Auto();
	virtual void LoadGame_Auto_CPP() = 0;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Save Load System|UI Functions|Save Game")
	void LoadGame_Quick();
	virtual void LoadGame_Quick_CPP() = 0;
};
