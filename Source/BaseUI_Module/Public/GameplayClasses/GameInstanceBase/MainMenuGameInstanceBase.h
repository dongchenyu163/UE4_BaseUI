// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ClassesAndStructs/FunctionHandlerInfo.h"
#include "Interfaces/I_GI_MenuFramework.h"
#include "WorldLevelHandler/UI_Modules/UI_MapSelection/MapSelectionBaseHandler.h"
#include "WorldLevelHandler/UI_Modules/UI_NextLevel/NextLevelBaseHandler.h"
#include "WorldLevelHandler/UI_Modules/UI_ResumeMenu/ResumeMenuBaseHandler.h"
#include "WorldLevelHandler/UI_Modules/UI_Save/SaveBaseHandler.h"
#include "WorldLevelHandler/UI_Modules/UI_SinglePlayerMenu/SinglePlayerMenuBaseHandler.h"
#include "UObject/Object.h"
#include "LowLevelHandler/UserManagement/UserManagerBase.h"
#include "MainMenuGameInstanceBase.generated.h"

/**
 * 
 */
UCLASS()
class BASEUI_MODULE_API UMainMenuGameInstanceBase : public UGameInstance, public II_GI_MenuFramework
{
	GENERATED_BODY()
public:
	virtual void Init() override;
protected:
	virtual ~UMainMenuGameInstanceBase() override {}
public:
	virtual UFunctionHandlerBase* FindHandler_ByName_CPP(FName InHandlerName) override;
	virtual UFunctionHandlerBase* FindHandler_ByName_Implementation(FName InHandlerName) override { return FindHandler_ByName_CPP(InHandlerName); }
	
	// virtual TSoftObjectPtr<UWorld> GetMainMenuMap_Implementation() override { return MainMenuMap; }
	//
	// virtual UDataTable* GetMapInfoDataTable_CPP() override { return MapInfoDataTable; }
	// virtual UDataTable* GetMapInfoDataTable_Implementation() override;
	//
	// virtual TMap<FName, FMapUIRuntimeInfo>* GetUIRuntimeInfoTMap_CPP() override { return &Map_MapName_To_MapRuntimeInfo; }
	// virtual TMap<FName, FMapUIRuntimeInfo> GetUIRuntimeInfoTMap_Implementation() override { return *GetUIRuntimeInfoTMap_CPP(); }
	// virtual FMapUIInfo GetMapUIInfo_Implementation(FName InMapIdentifier) override;
	// virtual FMapUIRuntimeInfo GetMapUIRuntimeInfo_Implementation(FName InMapIdentifier) override;
	// virtual FMapUIInfo* GetMapUIInfo_CPP(FName InMapIdentifier) override;
	// virtual FMapUIRuntimeInfo* GetMapUIRuntimeInfo_CPP(FName InMapIdentifier) override;
	//
	// virtual void UpdateFromUserGlobalData_CPP() override;
	// virtual void UpdateFromUserGlobalData_Implementation() override { UpdateFromUserGlobalData_CPP(); }

	// virtual FMapUIInfo* GetPlayingMapUIInfo_CPP() override;
	// virtual void SetPlayingMapUIInfo_CPP(const FMapUIInfo* InNewMapInfo) override;
	// virtual FMapUIInfo GetPlayingMapUIInfo_Implementation() override;
	// virtual void SetPlayingMapUIInfo_Implementation(FMapUIInfo InNewMapInfo) override;
	// virtual void ResetWidgetInfo_CPP() override;
	// virtual void ResetWidgetInfo_Implementation() override;
protected:
	virtual void OnStart() override;
public:
	virtual void OnWorldChanged(UWorld* OldWorld, UWorld* NewWorld) override;
	// virtual FOnAnyWorldBeginPlay* GetOnAnyWorldBeginPlayDelegate_CPP() override { return &OnAnyWorldBeginPlay; }

	// virtual FMapUIInfo* FindMapUIInfo_ByUWorld(UWorld* InWorldPtr);
	static void InitHandlers(II_GI_MenuFramework* InGameInstance, TMap<FName, FFunctionHandlerInfo>& InHandlerClassDict, TMap<FName, UFunctionHandlerBase*>& InCreatedHandlerDict);
	static void CreateHandlers(TMap<FName, FFunctionHandlerInfo>& InHandlerClassDict, TMap<FName, UFunctionHandlerBase*>& OutCreatedHandlerDict, UObject* InObjectOuter = GetTransientPackage());
protected:

	
	UPROPERTY()  // 只是为了保持该变量不被回收
	TMap<FName, UFunctionHandlerBase*> Map_HandlerName_To_HandlerObj;

	/**
	 *
	 */
public:
	// FOnAnyWorldBeginPlay OnAnyWorldBeginPlay; 

// protected:
// 	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="GameInfo")
// 	TSoftObjectPtr<UWorld> MainMenuMap = nullptr;
//
// 	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="GameInfo")
// 	UDataTable* MapInfoDataTable = nullptr;
//
// 	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="GameInfo", meta=(Tooltip=""))
// 	TMap<FName, FMapUIRuntimeInfo> Map_MapName_To_MapRuntimeInfo;
//
// 	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="GameInfo", meta=(Tooltip=""))
// 	FMapUIInfo PlayingMapInfo;
protected:
	bool bIsInit = true;

};
