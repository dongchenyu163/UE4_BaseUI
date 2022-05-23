// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
// #include "GUI_FrameWork/UI_Modules/UI_Save/SaveGameObjs/LevelActorsSaveGameObjBase.h"
#include "ImageUtils.h"
#include "SaveLoad/SaveLoadSystemGlobals.h"
#include "UObject/Interface.h"
#include "TextureUtilities.h"
#include "SaveLoad/SaveGameObjectHeaderBase.h"
#include "Serialization/ObjectWriter.h"
#include "I_Save.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogDongSavingSystem, All, All);


/** Delegate called from AsyncLoadGameFromSlot. First two parameters are passed in SlotName and UserIndex, third parameter is a bool indicating success (true) or failure (false). */
DECLARE_EVENT_ThreeParams(USavingBaseHandler, FOnGameSaveFinish_Event, const FString&, const int32, bool);

/** Delegate called from AsyncLoadGameFromSlot. First two parameters are passed in SlotName and UserIndex, third parameter is the returned SaveGame, or null if it failed to load. */
DECLARE_EVENT_ThreeParams(USavingBaseHandler, FOnGameLoadFinish_Event, const FString&, const int32, FDongSaveSystemSavingParam*);

DECLARE_DYNAMIC_DELEGATE_ThreeParams(FOnGameSaveFinished, const FString&, InSlotName, const int32, InUserIndex, bool, bInIsSucceed);

/**
 * @brief C++中不使用，仅暴露给蓝图使用，通知存档已经加载完毕。
 *		绑定：USavingBaseHandler::GetGameLoadFinishDelegate()获取委托对象。；
 *		调用时机：[UDongSaveSystemStatic_Simple::AsyncLoadGameFromSlot]->【USavingBaseHandler::Handle_AsyncLoadGameFromSlotDelegate】
 * @param InLoadedSlotName 加载的存档文件名
 * @param InLoadedUserIndex 用户的ID
 * @param InLoadedObjStruct 加载的存档UObject，存储关卡信息（Actor信息等）的 和 存储文件头
 */
DECLARE_DYNAMIC_DELEGATE_ThreeParams(FOnGameLoadFinished, const FString&, InLoadedSlotName, const int32, InLoadedUserIndex, FDongSaveSystemSavingParam&, InLoadedObjStruct);

// 底层->SavingHandler的委托
DECLARE_DELEGATE_ThreeParams(FOnAsyncLoadFromSaveArea, const FString&, const int32, FDongSaveSystemSavingParam*);


UENUM(BlueprintType)
enum ECannotSaveReason
{
	CanSave,
	NoUser
};

USTRUCT(BlueprintType, meta=(ToolTip="游戏本身的进度，例如：象棋残局游戏：哪些关卡通关了，获得那些成就；一个游戏的一个用户只需要一份的存档"))
struct FSaveGameInfo
{
	GENERATED_BODY()
		
	// UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame, Category="SaveData|BaseInfo", meta=(Tooltip=""))
	// FName PlayingMapIdentifier;
	// 		
	// UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame, Category="SaveData|BaseInfo", meta=(Tooltip=""))
	// UTexture2D* SaveDataImage;
};



// This class does not need to be modified.
UINTERFACE(BlueprintType)
class UI_Save : public UInterface
{
	GENERATED_BODY()
};

/**
 * 主要和UserManager配合，将存档（USaveGame对象）存入某个用户下，或者从某个用户下读取一个存档（USaveGame对象）
 */
class BASEUI_MODULE_API II_Save
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Save", meta=(Tooltip="通过本函数在保存前检查是否会覆盖某个存档"))
	bool DoesSlotExistInSaveArea(UPARAM(ref) FDongSaveSystemSavingParam& InParams);
	virtual bool DoesSlotExistInSaveArea_CPP(FDongSaveSystemSavingParam& InParams) = 0;
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Save", meta=(ToolTip=""))
	ECannotSaveReason CanSave();
	virtual ECannotSaveReason CanSave_CPP() = 0;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Save|Delegate", meta=(ToolTip="获取异步保存结束时候的通知委托"))
	FOnGameSaveFinished GetGameSaveFinishDelegate();
	virtual FOnGameSaveFinish_Event* GetGameSaveFinishDelegate_CPP() = 0;
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Save|Delegate", meta=(ToolTip="获取异步读取结束时候的通知委托"))
	FOnGameLoadFinished GetGameLoadFinishDelegate();
	virtual FOnGameLoadFinish_Event* GetGameLoadFinishDelegate_CPP() = 0;
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Save|Delegate", meta=(ToolTip="设置 是否使用异步加载存档， 默认开启。"))
	void SetUseAsyncOperation(bool InNewUseAsync);
	virtual void SetUseAsyncOperation_CPP(bool InNewUseAsync) = 0;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Save", meta=(Tooltip=""))
	void GetSaveDataHeaderList(ESaveArea_BP InSaveAreaID_Enum, TArray<USaveGameObjectHeaderBase*>& OutHeaderList);
	virtual void GetSaveDataHeaderList_CPP(FName InSaveAreaID, TArray<USaveGameObjectHeaderBase*>& OutHeaderList) = 0;

	// 获取存储区域的配置信息
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Save", meta=(Tooltip="获取某个存储区域的完整信息；大部分在工程设置的【】中配置"))
	void GetSaveAreaInfo(ESaveArea_BP InSaveAreaID_Enum, FSaveLoadSystemSaveArea& OutSaveAreaInfo);
	virtual FSaveLoadSystemSaveArea* GetSaveAreaInfo_CPP(FName InSaveAreaID) = 0;
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Save", meta=(Tooltip="获取某个存储区域的运行时信息；本函数只是方便蓝图少使用结构体的break节点；"))
	void GetSaveAreaRuntimeInfo(ESaveArea_BP InSaveAreaID_Enum, FSaveSystemSavingFeature& OutSaveAreaInfo);
	virtual void GetSaveAreaRuntimeInfo_CPP(FName InSaveAreaID, FSaveSystemSavingFeature& OutSaveAreaInfo) = 0;
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Save", meta=(Tooltip="获取某个存储区域的滚动缓冲区的头指针，即 发生自动保存的话 会保存在滚动缓冲区的哪个位置上"))
	int GetSaveAreaRollingBufferHeaderPos(ESaveArea_BP InSaveAreaID_Enum);
	virtual int GetSaveAreaRollingBufferHeaderPos_CPP(FName InSaveAreaID) = 0;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Save", meta=(DisplayName="", Tooltip=""))
	void SaveGameToArea(UPARAM(ref) FDongSaveSystemSavingParam& InParams);
	virtual void SaveGameToArea_CPP(FDongSaveSystemSavingParam& InParams) = 0;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Save", meta=(DisplayName="", Tooltip=""))
	/**
	 * @brief 从指定的SaveAreaID中读取关卡存档，会检查存档是否存在
	 * @param InOutParams
	 * @return bool 返回要加载的存档是否存在。存在->true；不存在->false
	 */
	bool LoadGameFromSaveArea(UPARAM(ref) FDongSaveSystemSavingParam& InOutParams);
	/**
	 * @brief 从指定的SaveAreaID中读取关卡存档，会检查存档是否存在
	 * @param InOutParams
	 * @return bool 返回要加载的存档是否存在。存在->true；不存在->false
	 */
	virtual bool LoadGameFromSaveArea_CPP(FDongSaveSystemSavingParam& InOutParams) = 0;
};
