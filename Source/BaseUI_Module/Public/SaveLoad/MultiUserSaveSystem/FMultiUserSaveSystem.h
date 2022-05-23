// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ObjectEditorUtils.h"
#include "SaveGameSystem.h"
#include "SaveLoad/SaveLoadSystemGlobals.h"

/**
 * 
 */
class BASEUI_MODULE_API FMultiUserSaveSystem : public FGenericSaveGameSystem
{
public:
	FMultiUserSaveSystem() {}
	virtual ~FMultiUserSaveSystem() {}

	static ISaveGameSystem* Get()
	{
		static FMultiUserSaveSystem SaveSystem;
		return &SaveSystem;
	}

	virtual bool SaveGame(bool bAttemptToUseUI, const TCHAR* Name, const int32 UserIndex,
	                      const TArray<uint8>& Data) override
	{
		FMultiUserSaveSystem::CurrentOperatingSlotIndex = UserIndex;
		return FGenericSaveGameSystem::SaveGame(bAttemptToUseUI, Name, UserIndex, Data);
	}

	virtual bool LoadGame(bool bAttemptToUseUI, const TCHAR* Name, const int32 UserIndex, TArray<uint8>& Data) override
	{
		FMultiUserSaveSystem::CurrentOperatingSlotIndex = UserIndex;
		return FGenericSaveGameSystem::LoadGame(bAttemptToUseUI, Name, UserIndex, Data);
	}

	virtual ESaveExistsResult DoesSaveGameExistWithResult(const TCHAR* Name, const int32 UserIndex) override
	{
		if (IFileManager::Get().FileSize(*GetSaveGamePath(Name)) >= 0)
		{
			return ESaveExistsResult::OK;
		}
		return ESaveExistsResult::DoesNotExist;
	}
protected:
	/**
	 * @brief 需要通过【】【】函数传递
	 * @param Name 
	 * @return 
	 */
	virtual FString GetSaveGamePath(const TCHAR* Name) override
	{
		const auto SaveLoadSystemConfig = USaveLoadSystemConfig::Get();
		const ESaveGameType& SaveType = FMultiUserSaveSystem::CurrentSaveType;

		FString SavePath = SaveLoadSystemConfig->GetSavePath(SaveType);
		
		return FString::Printf(TEXT("%s/%s.sav"), *SavePath, Name);
	}

	static int32 CurrentOperatingSlotIndex;
	static ESaveGameType CurrentSaveType;
	static TMap<int32, FName> Map_UserIndex_To_SaveFolderName;
public:
	static TMap<int32, FName>& GetCustomIndexMap()
	{
		return FMultiUserSaveSystem::Map_UserIndex_To_SaveFolderName;
	}

	static void SetCustomIndexMap(const TMap<int32, FName>& InNewMap)
	{
		Map_UserIndex_To_SaveFolderName = InNewMap;
	}

	static void SetCurrentSaveType(ESaveGameType InNewSaveType)
	{
		CurrentSaveType = InNewSaveType;
	}
};
