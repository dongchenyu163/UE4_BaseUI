// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SaveLoadSystemGlobals.h"
#include "Interfaces/I_Save.h"
#include "Kismet/GameplayStatics.h"
#include "LowLevelHandler/LowLevelHandlerBase/LowLevelFunctionHandlerBase.h"
#include "LowLevelHandler/UserManagement/UserManagerBase.h"
#include "MultiUserSaveSystem/DongSaveSystemStatic.h"
#include "UObject/Object.h"
#include "LowLevelHandler/UserManagement/Interfaces/I_UserManager.h"
#include "SavingBaseHandler.generated.h"


/**
 * 
 */
UCLASS(Blueprintable, BlueprintType, meta=(ToolTip="处理【保存和加载USaveGame】的类，本基类实现：【无用户】的游戏的【普通保存】【自动保存】【快速保存】三项功能"))
class BASEUI_MODULE_API USavingBaseHandler : public ULowLevelFunctionHandlerBase, public II_Save
{
	GENERATED_BODY()

public:
	// const static TMap<FString, UClass*> Map_Purpose_To_DependenceHandlerClass;
	// const static TMap<FString, FText> Map_Purpose_To_PurposeTooltip;
	const static FFunctionHandlerDef HandlerDef;

protected:
	virtual ~USavingBaseHandler() override {}

#pragma region 从ULowLevelFunctionHandlerBase继承
public:
	// virtual FName GetHandlerFName() override { return FName("LowLevelSaveHandler"); }
	// virtual TSet<UClass*> GetDependenceHandlerInterfaceCollection() override;
	virtual EFunctionHandlerType GetHandlerType() override { return EFunctionHandlerType::LowLevelHandler; }
	virtual void AssignInterfacePtr(UObject* MatchedObjectPtr, UClass* MatchedInterfaceClassPtr) override;
	virtual void AssignDependentHandlerPtr() override;
	virtual void InitHandler(II_GI_MenuFramework* InGameInstancePtr, TMap<FName, UFunctionHandlerBase*>& InDependencyHandlerDict) override;
protected:
	virtual void OnStart() override {}
#pragma endregion 从ULowLevelFunctionHandlerBase继承
	
public:
	// 核心函数：【保存】【读取】【检测存档存在性】【读取存档文件头】
	virtual void SaveGameToArea_Implementation(FDongSaveSystemSavingParam& InParams) override { SaveGameToArea_CPP(InParams); }
	virtual void SaveGameToArea_CPP(FDongSaveSystemSavingParam& InParams) override;
	
	virtual bool LoadGameFromSaveArea_Implementation(FDongSaveSystemSavingParam& InOutParams) override { return LoadGameFromSaveArea_CPP(InOutParams); }
	virtual bool LoadGameFromSaveArea_CPP(FDongSaveSystemSavingParam& InOutParams) override;
	
	virtual bool DoesSlotExistInSaveArea_Implementation(FDongSaveSystemSavingParam& InParams) override { return DoesSlotExistInSaveArea_CPP(InParams); }
	virtual bool DoesSlotExistInSaveArea_CPP(FDongSaveSystemSavingParam& InParams) override { return UDongSaveSystemStatic_Simple::DoesSaveGameExist(InParams); }

	virtual void GetSaveDataHeaderList_Implementation(ESaveArea_BP InSaveAreaID_Enum, TArray<USaveGameObjectHeaderBase*>& OutHeaderList) override;
	virtual void GetSaveDataHeaderList_CPP(FName InSaveAreaID, TArray<USaveGameObjectHeaderBase*>& OutHeaderList) override;

	// 外围函数
	/// 下面4个函数：获取SaveArea的信息的函数
	/**
	 * @brief 
	 * @param InSaveAreaID_Enum 
	 * @param OutSaveAreaInfo 
	 */
	virtual void GetSaveAreaInfo_Implementation(ESaveArea_BP InSaveAreaID_Enum, FSaveLoadSystemSaveArea& OutSaveAreaInfo) override;
	virtual FSaveLoadSystemSaveArea* GetSaveAreaInfo_CPP(FName InSaveAreaID) override;
	virtual void GetSaveAreaRuntimeInfo_Implementation(ESaveArea_BP InSaveAreaID_Enum, FSaveSystemSavingFeature& OutSaveAreaInfo) override;
	virtual int GetSaveAreaRollingBufferHeaderPos_Implementation(ESaveArea_BP InSaveAreaID_Enum) override;
	virtual void GetSaveAreaRuntimeInfo_CPP(FName InSaveAreaID, FSaveSystemSavingFeature& OutSaveAreaInfo) override;
	virtual int GetSaveAreaRollingBufferHeaderPos_CPP(FName InSaveAreaID) override;

	virtual void SetUseAsyncOperation_CPP(bool InNewUseAsync) override { bUserAsyncSaveLoad = InNewUseAsync; }
	virtual void SetUseAsyncOperation_Implementation(bool InNewUseAsync) override { bUserAsyncSaveLoad = InNewUseAsync; }

	virtual ECannotSaveReason CanSave_CPP() override;
	virtual ECannotSaveReason CanSave_Implementation() override { return CanSave_CPP();}

	virtual FOnGameSaveFinished GetGameSaveFinishDelegate_Implementation() override { return OnSaveFinished; }
	virtual FOnGameLoadFinished GetGameLoadFinishDelegate_Implementation() override { return OnLoadFinished; }
	virtual FOnGameSaveFinish_Event* GetGameSaveFinishDelegate_CPP() override { return &OnSaveFinish_Event_CPP; }
	virtual FOnGameLoadFinish_Event* GetGameLoadFinishDelegate_CPP() override { return &OnLoadFinish_Event_CPP; }
	
	virtual void Init(UObject* InUserManagerObj);

protected:	
	virtual void Handle_AsyncSaveGameToSlotDelegate(const FString& InSlotName, const int32 InUserIndex, bool bInIsSucceed);
	virtual void Handle_AsyncLoadGameFromSlotDelegate(const FString& InLoadedSlotName, const int32 InLoadedUserIndex, FDongSaveSystemSavingParam* InLoadedObjStruct);

public:
	UPROPERTY(BlueprintReadWrite, Category="Save", meta=(Tooltip=""))
	TScriptInterface<II_UserManager> UserManagerObj;
protected:
	II_UserManager* UserManagerPtr;

public:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="SaveGame", meta=(Tooltip="使用异步加载、保存。外部的类通过OnSaveFinished、OnLoadFinished动态多播委托来等待通知"))
	bool bUserAsyncSaveLoad = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="SaveGame|Delegate", meta=(Tooltip=""))
	TArray<USaveGameObjectHeaderBase*> SaveHeaderList;

	// 由外部绑定的委托
	// 暴露给蓝图用的委托
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="SaveGame|Delegate", meta=(Tooltip=""))
	FOnGameSaveFinished OnSaveFinished;
	
	// 暴露给蓝图用的委托
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="SaveGame|Delegate", meta=(Tooltip=""))
	FOnGameLoadFinished OnLoadFinished;

	FOnGameSaveFinish_Event OnSaveFinish_Event_CPP;
	FOnGameLoadFinish_Event OnLoadFinish_Event_CPP;

protected:
	// 完全内部使用的委托
	// First two parameters are passed in SlotName and UserIndex, third parameter is a bool indicating success (true)
	FAsyncSaveGameToSlotDelegate OnSaveFinished_CPP;

	// First two parameters are passed in SlotName and UserIndex, third parameter is the returned SaveGame
	FOnAsyncLoadFromSaveArea OnLoadFinished_CPP;
};