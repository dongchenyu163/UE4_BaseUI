// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "Kismet/GameplayStatics.h"
#include "LowLevelHandler/SaveLoad/SaveLoadSystemGlobals.h"
#include "LowLevelHandler/SaveLoad/Interfaces/I_Save.h"
#include "UObject/Object.h"
#include "DongSaveSystemStatic.generated.h"


/**
 * 完全不使用UE4的ISaveSystem
 */
UCLASS(BlueprintType)
class BASEUI_MODULE_API UDongSaveSystemStatic_Simple : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	/*************************
	 * 实际进行 保存存档 的函数
	 *************************/
	//
	UFUNCTION(BlueprintCallable, DisplayName="SaveGameToSlot", Category="DongSaveGameStatic")
	static bool SaveGameToSlot_BP(UPARAM(ref) FDongSaveSystemSavingParam& InParam) { return SaveGameToSlot(InParam); }
	static bool SaveGameToSlot(FDongSaveSystemSavingParam& InParam);
	// 异步保存存档
	static void AsyncSaveGameToSlot(FDongSaveSystemSavingParam& InParam, FAsyncSaveGameToSlotDelegate SavedDelegate);

	/*************************
	 * 实际进行 加载存档 的函数
	 *************************/
	UFUNCTION(BlueprintCallable, DisplayName="LoadGameToSlot", Category="DongSaveGameStatic")
	static USaveGame* LoadGameFromSlot_BP(UPARAM(ref) FDongSaveSystemSavingParam& InParam) { return LoadGameFromSlot(InParam); }
	static USaveGame* LoadGameFromSlot(FDongSaveSystemSavingParam& InOutParam);
	// 异步加载存档
	static void AsyncLoadGameFromSlot(FDongSaveSystemSavingParam& InParam, FOnAsyncLoadFromSaveArea LoadedDelegate);

	/*************************
	 * 删除存档
	 *************************/
	UFUNCTION(BlueprintCallable, DisplayName="DeleteGameInSlot", Category="DongSaveGameStatic")
	static bool DeleteGameInSlot_BP(UPARAM(ref) FDongSaveSystemSavingParam& InParam) { return DeleteGameInSlot(InParam); }
	static bool DeleteGameInSlot(FDongSaveSystemSavingParam& InParam);

	/*************************
	 * 查询存档
	 *************************/
	UFUNCTION(BlueprintCallable, DisplayName="DoesSaveGameExist", Category="DongSaveGameStatic")
	static bool DoesSaveGameExist_BP(UPARAM(ref) FDongSaveSystemSavingParam& InParam) { return DoesSaveGameExist(InParam); }
	static bool DoesSaveGameExist(FDongSaveSystemSavingParam& InParam);

	/*************************
	 * 从存储参数来获得最终的存储路径，用于错误信息提示。
	*************************/
	UFUNCTION(BlueprintCallable, DisplayName="GetSaveDataFilePath", Category="DongSaveGameStatic")
	static FString GetSaveDataFilePath_BP(UPARAM(ref) FDongSaveSystemSavingParam& InParam) { return GetPath(InParam); }
	static FString GetSaveDataFilePath(FDongSaveSystemSavingParam& InParam) { return GetPath(InParam); }

	/*************************
	 * 辅助功能函数
	 ************************/
	UFUNCTION(BlueprintCallable, Category="DongSaveGameStatic")
	static bool SaveActorToMemory(AActor* InSaveActor, TArray<uint8>& OutSaveData);
	UFUNCTION(BlueprintCallable, Category="DongSaveGameStatic")
	static AActor* LoadActorFromMemory(const TArray<uint8>& InSaveData, AActor* InOutLoadActor);

	// 保存Header和SaveGameToMemory()是在一起的
	//
	static bool LoadHeaderObjFromMemoryBytes(const TArray<uint8>& InSaveData, USaveGameObjectHeaderBase*& OutHeaderData);
	static bool ReadFileHeaderToMemoryBytes(const FString InFilePath, TArray<uint8>& OutSaveGameHeader);
	static bool UpdateHeader(const FString InFilePath, USaveGameObjectHeaderBase* OutHeaderData);

	static bool SaveGameToMemory(USaveGame* InSaveGameObject, USaveGameObjectHeaderBase* InSaveHeader, TArray<uint8>& OutSaveData);
	static USaveGame* LoadGameFromMemory(const TArray<uint8>& InSaveData, FDongSaveSystemSavingParam& OutLoadingResult);

	static UDongSaveSystemStatic_Simple* Get() { return GetMutableDefault<UDongSaveSystemStatic_Simple>(); }
	static USaveGameObjectHeaderBase* GetLoadedHeader() { return Get()->LoadedHeader; }
private:
	static FString GetPath(FDongSaveSystemSavingParam& InParam)
	{
		const auto SlotNameString = USaveLoadSystemConfig::GetSlotName(InParam);
		const auto SavePath = USaveLoadSystemConfig::GetSavePath(InParam);
		return SavePath / SlotNameString;
	}

	void CreateNewHeader()
	{
		if (IsValid(LoadedHeader))
		{
			LoadedHeader->MarkPendingKill();
			LoadedHeader = nullptr;
		}
		LoadedHeader = NewObject<USaveGameObjectHeaderBase>(UDongSaveSystemStatic_Simple::Get(), USaveLoadSystemConfig::Get()->GetHeaderClass());
	}

private:
	UPROPERTY()
	USaveGameObjectHeaderBase* LoadedHeader;
};
