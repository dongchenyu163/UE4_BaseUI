// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LowLevelHandler/SaveLoad/ClassAndStructs/SaveGameObjBase.h"
#include "WorldLevelHandler/UI_Modules/BaseHandler/UIHandlerBase.h"
#include "Interfaces/I_UI_Savable.h"
#include "UObject/Object.h"
#include "Kismet/GameplayStatics.h"
#include "LowLevelHandler/SaveLoad/ClassAndStructs/LevelActorsSaveGameObjBase.h"
#include "LowLevelHandler/SaveLoad/ClassAndStructs/UserGlobalDataSaveGameObjBase.h"
#include "LowLevelHandler/SaveLoad/SavingBaseHandler.h"
#include "UI/USaveSystemLoadingScreenBase.h"
#include "SaveBaseHandler.generated.h"


/**
 * 存储没什么特别的；读取阶段，在获取到存档（USaveGame对象）后，读取存档内部的关卡相关数据，并在关卡内恢复存档数据。
 */
UCLASS(BlueprintType)
class BASEUI_MODULE_API USaveBaseHandler : public UUIHandlerBase, public II_UI_Savable
{
	GENERATED_BODY()
public:
	USaveBaseHandler();
protected:
	virtual ~USaveBaseHandler() override {}
public:
	
	virtual void SaveGame_CPP(int32 InSlotNum) override;
	virtual void SaveGame_Auto_CPP() override;
	virtual void SaveGame_Quick_CPP() override;
	virtual void LoadGame_CPP(int32 InSlotNum) override;
	virtual void LoadGame_Auto_CPP() override;
	virtual void LoadGame_Quick_CPP() override;

	virtual UUserGlobalDataSaveGameObjBase* GetUserGlobalData_CPP() override { return UserGlobalData_SaveGameObj; }
	virtual UUserGlobalDataSaveGameObjBase* GetUserGlobalData_Implementation() override { return UserGlobalData_SaveGameObj; }
	virtual void SaveUserGlobalData_CPP() override;
	virtual void SaveUserGlobalData_Implementation() override { SaveUserGlobalData_CPP(); }
	virtual void LoadUserGlobalData_CPP() override;
	virtual void LoadUserGlobalData_Implementation() override { LoadUserGlobalData_CPP(); }

	virtual void GetSaveDataHeaderList_CPP(FName InSaveAreaID, TArray<USaveGameObjectHeaderBase*>& OutHeaderList) override;
	virtual void GetSaveDataHeaderList_Implementation(ESaveArea_BP InSaveAreaID_Enum, TArray<USaveGameObjectHeaderBase*>& OutHeaderList) override;

	virtual void SaveGame_Implementation(int32 InSlotNum) override { SaveGame_CPP(InSlotNum); }
	virtual void SaveGame_Auto_Implementation() override { SaveGame_Auto_CPP(); }
	virtual void SaveGame_Quick_Implementation() override { SaveGame_Quick_CPP(); }
	virtual void LoadGame_Implementation(int32 InSlotNum) override { LoadGame_CPP(InSlotNum); }
	virtual void LoadGame_Auto_Implementation() override { LoadGame_Auto_CPP(); }
	virtual void LoadGame_Quick_Implementation() override { LoadGame_Quick_CPP(); }

	virtual UWidget* GetSaveSystemLoadingScreenWidget_Implementation() override { return GetSaveSystemLoadingScreenWidget_CPP(); }
	virtual void SetSaveSystemLoadingScreenWidgetClass_Implementation(TSubclassOf<UUSaveSystemLoadingScreenBase> InLoadingScreenWidgetClass) override { SetSaveSystemLoadingScreenWidgetClass_CPP(InLoadingScreenWidgetClass); }
	virtual UWidget* GetSaveSystemLoadingScreenWidget_CPP() override { return SaveSystemLoadingScreen; }
	virtual void SetSaveSystemLoadingScreenWidgetClass_CPP(TSubclassOf<UUSaveSystemLoadingScreenBase> InLoadingScreenWidgetClass) override { UUISaveLoadSystemConfig::Get()->SaveSystemLoadingScreenWidgetClass = InLoadingScreenWidgetClass; }
	
	void Handle_AsyncLoadComplete(const FString& InLoadedSlotName, int InLoadedUserIndex, FDongSaveSystemSavingParam* InLoadedObjStruct);
	void Handle_AsyncSaveComplete(const FString& InSlotName, int InUserIndex, bool bInIsSucceed);
	UFUNCTION()
	void Handle_OnUserChanged(const FUserInfo& InOldUser, const FUserInfo& InNewUser);
	void Init(UObject* InSavingHandleObj, UObject* InUserManagerObj);

	/**
	 * @brief 准备要保存的数据；关卡缩略图，关卡Actors，额外信息（存档序号，关卡名称等）
	 * @param InSlotNum 存档的序号，对于UI的意义就是，各个存档的显示顺序
	 */
	void PrepareSaveData(int32 InSlotNum);
	void WriteLevelActorsToSaveObject();
	void PrepareForLoading();
	/**
	 * @brief 进行关卡中Actor的状态的恢复；前提：World已经加载完毕，且存档也加载完毕后。
	 */
	void LoadLevelActorsFromSaveObject() const;

	UFUNCTION()
	/**
	 * @brief 进行关卡加载的时候，在GameInstance切换完World时调用OnWorldChanged的时候转发到本函数；
	 *			通知本存储系统新的World已经加载完毕，可以继续进行后续的Actor恢复工作了
	 *			该函数被动态委托挂载，故需要UFUNCTION宏标记
	 */
	void Handle_OnAnyWorldChanged();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Save Load System|UI Handler", meta=(ToolTip=""))
	bool CreateNewUserGlobalData();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Save Load System|UI Handler", meta=(ToolTip=""))
	bool CreateNewLevelSavingData();

	/**
	 * 变量区域
	 */
public:
	UPROPERTY(BlueprintReadWrite, Category="Save Low Level Handler", meta=(Tooltip=""))
	TScriptInterface<II_Save> SavingHandleObj;
protected:
	II_Save* SavingHandlePtr;
	II_UserManager* UserManagerPtr;

	// Config：描述用户的存档类，一个游戏应该只有一种。
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Save Object Class", meta=(DisplayName="User Global Data Class", Tooltip="用户存档"))
	TSubclassOf<UUserGlobalDataSaveGameObjBase> UserGlobalData_SaveGameObjClass = UUISaveLoadSystemConfig::Get()->UserGlobalData_SaveGameObjClass;

	UPROPERTY(BlueprintReadWrite, Category="Save Object", meta=(DisplayName="", Tooltip=""))
	UUserGlobalDataSaveGameObjBase* UserGlobalData_SaveGameObj;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Save Object Class", meta=(DisplayName="Game Save Data Class", Tooltip="指定【关卡存档】使用的类，默认基类保存了【存档的显示信息】【保存的Actor的序列化信息】，可通过创建子类来自定义【关卡存档】需要保存的额外信息"))
	TSubclassOf<ULevelActorsSaveGameObjBase> LevelSavingData_SaveGameObjClass = ULevelActorsSaveGameObjBase::StaticClass();

	UPROPERTY(BlueprintReadWrite, Category="Save Object", meta=(DisplayName="", Tooltip="【关卡存档】的对象，用于保存的时候进行临时存储；用户通过【Save Game】按钮进行当前关卡的保存操作而产生的存档，即【关卡存档】"))
	ULevelActorsSaveGameObjBase* LevelSavingData;
	
	UPROPERTY(BlueprintReadOnly, Category="Save Object", meta=(ToolTip="读取的存档的信息都在这里，之初的目的是：通过这个对象蓝图可以在 当前关卡进行的时候 读取 存档中的【关卡全局信息】"))
	ULevelActorsSaveGameObjBase* LevelInformationObj = nullptr;

	UPROPERTY(BlueprintReadWrite, Category="Save|Actor Saving", meta=(DisplayName="", Tooltip="只有具有以下名称的Tag（不是GameplayTag），才会进行保存"))
	FName FilterActorTagName = "SaveGame";
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Save Load System|Loading Screen", meta=(Tooltip="存储当前加载界面的对象【GetSaveSystemLoadingScreenWidget】"))
	UUSaveSystemLoadingScreenBase* SaveSystemLoadingScreen;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Save Load System|Loading Screen", meta=(Tooltip="产生加载动作后才 置位 本标志位，过滤掉非法的【】事件"))
	bool bIsWaitingWorldChanged = false;
};

