// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseClassesAndTypes/FunctionHandlerDef.h"
#include "UObject/Object.h"
#include "LowLevelHandler/SaveLoad/ClassAndStructs/MapInfo.h"
#include "LowLevelHandler/SaveLoad/Interfaces/I_Save.h"
#include "WorldLevelHandler/UI_Modules/UI_Save/Interfaces/I_UI_Savable.h"
#include "WorldLevelHandler/WorldLevelHandlerBase/WorldLevelHandlerBase.h"
#include "MapsInfoHandler.generated.h"

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class BASEUI_MODULE_API UMapsInfoHandler : public UWorldLevelHandlerBase
{
	GENERATED_BODY()

public:
	virtual void AssignDependentHandlerPtr() override;
	virtual void InitHandler(II_GI_MenuFramework* InGameInstancePtr,
		TMap<FName, UFunctionHandlerBase*>& InDependencyHandlerDict) override;
	virtual void OnStart() override;
	
public:
	UFUNCTION(BlueprintCallable, Category="MenuFramework|WorldLevelHandler|MapInfo", meta=(ToolTip="返回主菜单时用"))
	TSoftObjectPtr<UWorld> GetMainMenuMap() { return MainMenuMap; }
	
	UFUNCTION(BlueprintCallable, Category="MenuFramework|WorldLevelHandler|MapInfo")
	UDataTable* GetMapInfoDataTable() { return MapInfoDataTable; }

	UFUNCTION(BlueprintCallable, Category="MenuFramework|WorldLevelHandler|MapInfo")
	bool GetMapInfo(FName InMapIdentifier, FMapInfo& OutMapInfo);
	
	UFUNCTION(BlueprintCallable, Category="MenuFramework|WorldLevelHandler|MapInfo", meta=(ToolTip="通过UserGlobalData来更新GameInstance里面的成员"))
	void UpdateFromUserGlobalData();
	
	UFUNCTION(BlueprintCallable, Category="MenuFramework|WorldLevelHandler|MapInfo")
	FMapInfo GetPlayingMapInfo() { return PlayingMapInfo; }
	
	UFUNCTION(BlueprintCallable, Category="MenuFramework|WorldLevelHandler|MapInfo")
	void SetPlayingMapInfo(FMapInfo InNewMapInfo) { PlayingMapInfo = InNewMapInfo; }
	
	UFUNCTION(BlueprintCallable, Category="MenuFramework|WorldLevelHandler|MapInfo")
	FMapInfo_User GetMapRuntimeInfo(FName InMapIdentifier) { return *GetMapRuntimeInfo_CPP(InMapIdentifier); }
	FMapInfo_User* GetMapRuntimeInfo_CPP(FName InMapIdentifier) { return &SaveHandlePtr->GetUserGlobalData()->UserMapsRuntimeInfo[InMapIdentifier]; }

	UFUNCTION(BlueprintCallable, Category="MenuFramework|WorldLevelHandler|MapInfo")
	void SetMapRuntimeInfo(FName InMapIdentifier, FMapInfo_User InNewMapInfo) { SetMapRuntimeInfo_CPP(InMapIdentifier, InNewMapInfo); }
	void SetMapRuntimeInfo_CPP(FName InMapIdentifier, const FMapInfo_User& InNewMapInfo) const { SaveHandlePtr->GetUserGlobalData()->UserMapsRuntimeInfo[InMapIdentifier] = InNewMapInfo; }

	// UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="MenuFramework|WorldLevelHandler|MapInfo", meta=(ToolTip="重置UDA_WidgetInfo中的WidgetRef变量，切换地图的时候该变量会被置空"))
	// void ResetWidgetInfo();
	
public:
	FMapInfo* FindMapUIInfo_ByUWorld(UWorld* InWorldPtr);

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="GameInfo")
	TSoftObjectPtr<UWorld> MainMenuMap = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="GameInfo")
	UDataTable* MapInfoDataTable = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="GameInfo", meta=(Tooltip=""))
	TMap<FName, FMapInfo_User> Map_MapName_To_MapRuntimeInfo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="GameInfo", meta=(Tooltip=""))
	FMapInfo PlayingMapInfo;

	
	const static FFunctionHandlerDef HandlerDef;

private:
	II_UI_Savable* SaveHandlePtr;
	II_Save* LowLevelSavingHandlePtr;
	II_UserManager* UserManagerPtr;
};
