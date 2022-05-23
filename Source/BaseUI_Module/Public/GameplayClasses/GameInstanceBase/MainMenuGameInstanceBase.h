// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
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
	virtual TScriptInterface<II_UI_SinglePlayerFunctions> GetSinglePlayerMenuBaseHandler_Implementation() override { return SinglePlayerFunctionsHandlerObj; }
	virtual TScriptInterface<II_UI_MapSelectable> GetMapSelector_Implementation() override { return MapSelectableHandlerObj; }
	virtual TScriptInterface<II_UI_ResumeMenu> GetResumeMenuBaseHandler_Implementation() override { return ResumeMenuHandlerObj; }
	virtual TScriptInterface<II_UI_Savable> GetSavableBaseHandler_Implementation() override { return SavableHandlerObj; }
	virtual TScriptInterface<II_UI_NextLevel> GetNextLevelHandler_Implementation() override { return NextLevelHandlerObj; }
	virtual TScriptInterface<II_UserManager> GetUserManager_Implementation() override { return UserManager; }
	
	virtual II_UI_SinglePlayerFunctions* GetSinglePlayerMenuBaseHandler_CPP() override { return SinglePlayerFunctionsHandlerObj; }
	virtual II_UI_MapSelectable* GetMapSelector_CPP() override { return MapSelectableHandlerObj; }
	virtual II_UI_ResumeMenu* GetResumeMenuBaseHandler_CPP() override { return ResumeMenuHandlerObj; }
	virtual II_UI_Savable* GetSavableBaseHandler_CPP() override { return SavableHandlerObj; }
	virtual II_UI_NextLevel* GetNextLevelHandler_CPP() override { return NextLevelHandlerObj; }
	virtual II_UserManager* GetUserManager_CPP() override { return UserManager; }
	
	virtual TSoftObjectPtr<UWorld> GetMainMenuMap_Implementation() override { return MainMenuMap; }

	virtual UDataTable* GetMapInfoDataTable_CPP() override { return MapInfoDataTable; }
	virtual UDataTable* GetMapInfoDataTable_Implementation() override;

	virtual TMap<FName, FMapUIRuntimeInfo>* GetUIRuntimeInfoTMap_CPP() override { return &Map_MapName_To_MapRuntimeInfo; }
	virtual TMap<FName, FMapUIRuntimeInfo> GetUIRuntimeInfoTMap_Implementation() override { return *GetUIRuntimeInfoTMap_CPP(); }
	virtual FMapUIInfo GetMapUIInfo_Implementation(FName InMapIdentifier) override;
	virtual FMapUIRuntimeInfo GetMapUIRuntimeInfo_Implementation(FName InMapIdentifier) override;
	virtual FMapUIInfo* GetMapUIInfo_CPP(FName InMapIdentifier) override;
	virtual FMapUIRuntimeInfo* GetMapUIRuntimeInfo_CPP(FName InMapIdentifier) override;

	virtual void UpdateFromUserGlobalData_CPP() override;
	virtual void UpdateFromUserGlobalData_Implementation() override { UpdateFromUserGlobalData_CPP(); }

	virtual FMapUIInfo* GetPlayingMapUIInfo_CPP() override;
	virtual void SetPlayingMapUIInfo_CPP(const FMapUIInfo* InNewMapInfo) override;
	virtual FMapUIInfo GetPlayingMapUIInfo_Implementation() override;
	virtual void SetPlayingMapUIInfo_Implementation(FMapUIInfo InNewMapInfo) override;
	virtual void ResetWidgetInfo_CPP() override;
	virtual void ResetWidgetInfo_Implementation() override;
protected:
	virtual void OnStart() override;
	virtual void OnFirstWorldChanged(UWorld* NewWorld);
public:
	virtual void OnWorldChanged(UWorld* OldWorld, UWorld* NewWorld) override;
	virtual FOnAnyWorldBeginPlay* GetOnAnyWorldBeginPlayDelegate_CPP() override { return &OnAnyWorldBeginPlay; }

	virtual FMapUIInfo* FindMapUIInfo_ByUWorld(UWorld* InWorldPtr);
protected:
	UPROPERTY()  // 只是为了保持该变量不被回收
	USinglePlayerMenuBaseHandler* SinglePlayerFunctionsHandlerObj = nullptr;
	UPROPERTY()
	UMapSelectionBaseHandler* MapSelectableHandlerObj = nullptr;
	UPROPERTY()
	UResumeMenuBaseHandler* ResumeMenuHandlerObj = nullptr;
	UPROPERTY()
	USaveBaseHandler* SavableHandlerObj = nullptr;
	UPROPERTY()
	UNextLevelBaseHandler* NextLevelHandlerObj = nullptr;

	UPROPERTY()
	UUserManagerBase* UserManager = nullptr;
	UPROPERTY()
	USavingBaseHandler* SavingBaseHandler = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Handler Class", meta=(Tooltip=""))
	TSubclassOf<USinglePlayerMenuBaseHandler> SinglePlayerMenuHandlerClass = USinglePlayerMenuBaseHandler::StaticClass();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Handler Class", meta=(Tooltip=""))
	TSubclassOf<UMapSelectionBaseHandler> MapSelectableHandlerClass = UMapSelectionBaseHandler::StaticClass();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Handler Class", meta=(Tooltip=""))
	TSubclassOf<UResumeMenuBaseHandler> ResumeMenuHandlerClass = UResumeMenuBaseHandler::StaticClass();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Handler Class", meta=(Tooltip=""))
	TSubclassOf<USaveBaseHandler> SavableHandlerClass = USaveBaseHandler::StaticClass();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Handler Class", meta=(Tooltip=""))
	TSubclassOf<UNextLevelBaseHandler> NextLevelHandlerClass = UNextLevelBaseHandler::StaticClass();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Handler Class", meta=(Tooltip=""))
	TSubclassOf<UUserManagerBase> UserManagerClass = UUserManagerBase::StaticClass();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Handler Class", meta=(Tooltip=""))
	TSubclassOf<USavingBaseHandler> SavingBaseHandlerClass = USavingBaseHandler::StaticClass();


	/**
	 *
	 */
public:
	FOnAnyWorldBeginPlay OnAnyWorldBeginPlay; 

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="GameInfo")
	TSoftObjectPtr<UWorld> MainMenuMap = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="GameInfo")
	UDataTable* MapInfoDataTable = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="GameInfo", meta=(Tooltip=""))
	TMap<FName, FMapUIRuntimeInfo> Map_MapName_To_MapRuntimeInfo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="GameInfo", meta=(Tooltip=""))
	FMapUIInfo PlayingMapInfo;

protected:
	bool bIsInit = true;
};
