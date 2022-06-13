#pragma once

#include "CoreMinimal.h"
#include "ReflectionOperations.h"
#include "SaveGameObjectHeaderBase.h"
#include "LowLevelHandler/UserManagement/Interfaces/I_UserManager.h"
#include "Widgets/SSaveLoadSystemDetailsWidget.h"
#include "ClassAndStructs/ESaveGameType.h"
#include "ClassAndStructs/FDongSaveSystemSavingParam.h"
#include "ClassAndStructs/FSaveLoadSystemSaveArea.h"
#include "Misc/FileHelper.h"
#include "SaveLoadSystemGlobals.generated.h"


UCLASS(config = SaveLoadSystemConfig)
class USaveLoadSystemConfig : public UDeveloperSettings
{
	GENERATED_BODY()
public:
	USaveLoadSystemConfig()
		: Super()
	{
		// 没有配置文件的时候的初始值。（有的话这里的值会被覆盖）
		Map_SaveAreaID_To_SaveAreaInfo.Add("NormalSave", FSaveLoadSystemSaveArea("{UserID}/Save Data", NumberSave));
		Map_SaveAreaID_To_SaveAreaInfo.Add("AutoSave", FSaveLoadSystemSaveArea("{UserID}/Auto Save", RollingSave));
		Map_SaveAreaID_To_SaveAreaInfo.Add("QuickSave", FSaveLoadSystemSaveArea("{UserID}/Quick Save", RollingSave));
		Map_SaveAreaID_To_SaveAreaInfo.Add("ConfigArea", FSaveLoadSystemSaveArea("{UserID}/", OfficialSave, false));
		Map_SaveAreaID_To_SaveAreaInfo.Add("CustomLevelSaveArea", FSaveLoadSystemSaveArea("{UserID}/Custom Level Saving", CustomNameSave));

		// 手动触发Config的读取，如果有SaveLoadSystemConfig.ini的话。
		ReloadConfig();
		// 更新OLD_Map_SaveAreaID_To_SaveAreaInfo，方便后续检查被更改的值（为了实时更新ESaveArea_BP的枚举选项）
		OLD_Map_SaveAreaID_To_SaveAreaInfo = Map_SaveAreaID_To_SaveAreaInfo;

		// 自定义Config的界面。关联【USaveLoadSystemConfig】和【自定义的设置界面】
		static FName PropertyEditor("PropertyEditor");
		FPropertyEditorModule& PropertyModule = FModuleManager::GetModuleChecked<FPropertyEditorModule>(PropertyEditor);
		PropertyModule.RegisterCustomClassLayout("SaveLoadSystemConfig", FOnGetDetailCustomizationInstance::CreateStatic(&FSaveLoadSystemDetailsCustomization::MakeInstance) );
	}

public:
	virtual FName GetContainerName() const override { return TEXT("Project"); }
	virtual FName GetCategoryName() const override { return TEXT("Dong UI Framework Modules Config"); }  // Setting的左侧栏的大分类
	virtual FName GetSectionName() const override { return TEXT("Save & Load System Config"); }  // Setting的左侧栏的子分类

	// Setting的左侧栏的子分类
	virtual FText GetSectionText() const override
	{
		return NSLOCTEXT("SaveLoadSystemConfig", "SectionText", "Dong Save Load System Plugin Config");
	}

	virtual FText GetSectionDescription() const override
	{
		return NSLOCTEXT("SaveLoadSystemConfig", "SectionDescriptionText", "设置存档系统的相关参数");
	}


	TMap<FName, FSaveLoadSystemSaveArea>* GetSaveAreaInfo_Map()
	{
		return &Map_SaveAreaID_To_SaveAreaInfo;
	}
	
	/**
	 * @brief 
	 * @param InSlotID 0~2^32 传入INDEX_NONE代表使用SaveSlotNameFormat_HasCustom作为Slot的名称InCustomName作为名称中的变量。
	 * @param InCustomName 自定义名称
	 * @param InSaveType 保存的类型【普通】【自动】【快速】
	 * @return 
	 */
	static FString GetSlotName(FDongSaveSystemSavingParam& InParam)
	{
		return USaveLoadSystemConfig::Get()->GetSlotName_Internal(InParam);
	}

	/**
	 * @brief 
	 * @param InParam 只需要InParam.SaveAreaID即可
	 * @return 
	 */
	static FString GetSavePath(FDongSaveSystemSavingParam& InParam)
	{
		return USaveLoadSystemConfig::Get()->GetSavePath_Internal(InParam);
	}

	static bool CheckSaveSystemSavingParam(FDongSaveSystemSavingParam& InParam, bool bInCheckForLoading = false)
	{
		return USaveLoadSystemConfig::Get()->CheckSaveSystemSavingParam_Internal(InParam, bInCheckForLoading);
	}

	static void SetUserManagerInstance(II_UserManager* InNewGameInstance) { Get()->UserManagerPtr = InNewGameInstance; Get()->SetOnUserChange(); }

	void SetOnUserChange() { UserManagerPtr->GetOnUserChangedDelegate_CPP()->BindDynamic(this, &USaveLoadSystemConfig::Handle_OnUserChange); }

	UFUNCTION(BlueprintPure, Category="Save Load System Config Object", DisplayName="GetSaveLoadSystemConfig")
	static USaveLoadSystemConfig* Get() { return GetMutableDefault<USaveLoadSystemConfig>(); }
	
	UFUNCTION(BlueprintPure, Category="Save Load System Config Object")
	static FString GetSaveFileExtension() { return Get()->SaveFileExtension; }

	UFUNCTION(BlueprintPure, Category="Save Load System Config Object")
	static UClass* GetHeaderClass() { return Get()->SaveHeaderClass; }
	
protected:
	virtual FString GetSavePath_Internal(FDongSaveSystemSavingParam& InParam)
	{
		const FUserInfo* UserInfo = GetCurrentUserInfo();
		const int32 UserID = UserInfo->BaseInfo.UserID;
		
		const TMap<FName, FString>* FoundUserSavePathMap = Map_UserID_To_SavePathMap.Find(UserID);
		if (!FoundUserSavePathMap)
		{
			RefreshUserSavePathCache(UserInfo);
			FoundUserSavePathMap = &Map_UserID_To_SavePathMap[UserID];
		}
		return (*FoundUserSavePathMap)[InParam.SaveAreaID];
	}
	
	virtual FString GetSlotName_Internal(FDongSaveSystemSavingParam InParam)
	{
		// 获取当前操作的User信息
		const FUserInfo* UserInfo = GetCurrentUserInfo();
		const int32& UserID = UserInfo->BaseInfo.UserID;
		
		FSaveLoadSystemSaveArea SaveAreaInfo = Map_SaveAreaID_To_SaveAreaInfo[InParam.SaveAreaID];
		FStringFormatNamedArguments Args;
		FString FinalSlotName;

		/* DONG_SEARCH_TAG: SaveSlotNameFormat 存储路径 添加 字符串 变量 */
		Args.Add("UserID", FString::FromInt(UserID));
		// Args.Add("Prefix", SaveAreaInfo.SaveSlotPrefix);  // Prefix已弃用，因为前缀可以写死在SaveSlotNameFormat里面。
		Args.Add("SlotID", FString::FromInt(InParam.SlotNum));
		Args.Add("CustomSlotName", InParam.CustomSlotName);
		if (SaveAreaInfo.SaveGameType == NumberSave)
		{
		}
		else if (SaveAreaInfo.SaveGameType == RollingSave)
		{
			int& SaveSlotHeaderID = SaveAreaInfo.SaveGameFeatureRuntimeInfo.RollingBufferPos;
			const int& MaxSize = SaveAreaInfo.SaveGameFeatureRuntimeInfo.RollingBufferSize;

			// 如果SaveSlotHeaderID处于初始状态：即INDEX_NONE，滚动缓冲区里面为空，头指针不指向任何合法的地址。
			//		这时自增1的话刚好是0号位置。
			// 如果SaveSlotHeaderID处于普通状态，滚动缓冲区里面不为空，
			//		SaveSlotHeaderID记录的是头地址（即：最后存储的存档的序号）。
			//		这时需要自增1来获得本册应当存储的头地址。
			SaveSlotHeaderID++;
			SaveSlotHeaderID = SaveSlotHeaderID % MaxSize;
			
			/* DONG_SEARCH_TAG: 存储路径 添加 字符串 变量 */
			Args.Remove("SlotID");
			Args.Add("SlotID", FString::FromInt(SaveSlotHeaderID));

		}
		else if (SaveAreaInfo.SaveGameType == CustomNameSave)
		{
			/* DONG_SEARCH_TAG: 存储路径 添加 字符串 变量 */
			Args.Remove("SlotID");  // 本模式不需要响应SlotID参数。
		}
		else if (SaveAreaInfo.SaveGameType == OfficialSave)
		{
		}
		else
		{
			UE_LOG(LogTemp, Display, TEXT("Function:[%s] Invalid SaveType"), ANSI_TO_TCHAR(__FUNCTION__));
			return "";
		}
		
		FinalSlotName = FString::Format(*SaveAreaInfo.SaveSlotNameFormat, Args);
		FinalSlotName = InParam.SaveAdditionalPath / FinalSlotName;

		return FinalSlotName;
	}

	FUserInfo* GetCurrentUserInfo()
	{
		return UserManagerPtr->GetCurrentUser_CPP();
	}

	// 为了更新【【UserID】【SaveAreaID】】缓存，存储路径基本上不变，所以可以直接存储到哈希表中，方便快速查询取用。
	virtual void RefreshUserSavePathCache(const FUserInfo* UserInfo)
	{
		const auto UserID = UserInfo->BaseInfo.UserID;
		const auto UserName = UserInfo->BaseInfo.UserName;
		/* DONG_SEARCH_TAG: SaveLocationFormatStr 存储路径 添加 字符串 变量 */
		FStringFormatNamedArguments Args;
		Args.Add("UserID", (FString::FromInt(UserID)));
		Args.Add("UserName", (UserName.ToString()));

		// 制作 可以通过【UserID】【SaveAreaID】进行存储路径查询的二重TMap
		for (auto SaveAreaTuple : Map_SaveAreaID_To_SaveAreaInfo)
		{
			const auto& SaveAreaInfo = SaveAreaTuple.Value;
			FString&& FolderName = FString::Format(*SaveAreaInfo.SaveLocationFormatStr, Args);
			FString FolderNameString = SaveBaseFolder / FolderName;

			// 寻找用户的路径Map
			auto FoundUserSavePathMapPtr = Map_UserID_To_SavePathMap.Find(UserID);
			if (!FoundUserSavePathMapPtr)
			{
				Map_UserID_To_SavePathMap.Add(UserID, TMap<FName, FString>());
				FoundUserSavePathMapPtr = &Map_UserID_To_SavePathMap[UserID];
			}

			// 寻找路径Map中是否有当前的 SaveType
			auto FoundSaveFolderStringPtr = FoundUserSavePathMapPtr->Find(SaveAreaTuple.Key);
			if (!FoundSaveFolderStringPtr)
			{
				// 没有的话添加
				FoundUserSavePathMapPtr->Add(SaveAreaTuple.Key, FolderNameString);
			}
			else
			{
				// 有的话覆盖
				(*FoundUserSavePathMapPtr)[SaveAreaTuple.Key] = FolderNameString;
			}
		}
	}

	virtual bool CheckSaveSystemSavingParam_Internal(FDongSaveSystemSavingParam& InParam, bool bInCheckForLoading = false)
	{
		// 检查 传入的【区域名称】是否存在
		const FSaveLoadSystemSaveArea* AreaInfo = Map_SaveAreaID_To_SaveAreaInfo.Find(InParam.SaveAreaID);
		if (AreaInfo == nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("Function:[%s] SaveAreaID: [%s] NOT FOUND!"), ANSI_TO_TCHAR(__FUNCTION__), *InParam.SaveAreaID.ToString());
			return false;
		}

		// 检查 传入的【区域存储类型】是否和设置的相同
		if (AreaInfo->SaveGameType != InParam.SaveType)
		{
			const UEnum* EnumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("ESaveGameType_Candidate"), true);
			const FString ConfigSaveTypeStr = *EnumPtr->GetNameStringByValue(AreaInfo->SaveGameType);
			const FString GivenSaveTypeStr = *EnumPtr->GetNameStringByValue(InParam.SaveType);
			UE_LOG(LogTemp, Warning, TEXT("Function:[%s] SaveType in config: [%s], BUT you give the type: [%s]! 检查你使用的用于创建InParam的函数，是否和Config中设置的一样。"), 
					ANSI_TO_TCHAR(__FUNCTION__),
					*ConfigSaveTypeStr,
					*GivenSaveTypeStr);
			return false;
		}

		if (!bInCheckForLoading)
		{
			// 保存的时候 检查 要保存的Obj是否合法。
			if (!IsValid(InParam.MainSaveObj))
			{
				UE_LOG(LogTemp, Warning, TEXT("Function:[%s] MainSaveObj is not valid!"), ANSI_TO_TCHAR(__FUNCTION__));
				return false;
			}

			// 保存的时候  检查 要保存的 文件头 是否合法。
			FSaveSystemSavingFeature SaveFeature;
			SaveFeature.FromSaveType(InParam.SaveType);
			const bool& bCheckHeaderObj = !SaveFeature.bUseOfficial;
			if(bCheckHeaderObj)
			{
				if (!IsValid(InParam.SaveObjHeader))
				{
					UE_LOG(LogTemp, Warning, TEXT("Function:[%s] SaveObjHeader is not valid!"), ANSI_TO_TCHAR(__FUNCTION__));
					return false;
				}
			}
		}

		return true;
	}
	
	UFUNCTION()
	void Handle_OnUserChange(const FUserInfo& InOldUser, const FUserInfo& InNewUser)
	{
		LoadRuntimeData();
	}

	virtual void PostReloadConfig(FProperty* PropertyThatWasLoaded) override
	{
		TArray<FName> SaveAreaIDList;
		Map_SaveAreaID_To_SaveAreaInfo.GenerateKeyArray(SaveAreaIDList);
		UReflectionOperations::SetNewOptionsToEnum("ESaveArea_BP", SaveAreaIDList);
	}

	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override
	{
		if (PropertyChangedEvent.Property->GetName() == "Map_SaveAreaID_To_SaveAreaInfo")
		{
			if (PropertyChangedEvent.ChangeType == EPropertyChangeType::ArrayAdd)
			{
				// ArrayAdd：Property和MemberProperty是一样的；无法取得新元素的Index
				TArray<FName> SaveAreaIDList;
				Map_SaveAreaID_To_SaveAreaInfo.GenerateKeyArray(SaveAreaIDList);
				UReflectionOperations::SetNewOptionsToEnum("ESaveArea_BP", SaveAreaIDList);
			}
			else if (PropertyChangedEvent.ChangeType == EPropertyChangeType::ArrayRemove)
			{
				TArray<FName> SaveAreaIDList;
				Map_SaveAreaID_To_SaveAreaInfo.GenerateKeyArray(SaveAreaIDList);
				UReflectionOperations::SetNewOptionsToEnum("ESaveArea_BP", SaveAreaIDList);
			}
			else if (PropertyChangedEvent.ChangeType == EPropertyChangeType::ArrayClear)
			{
				TArray<FName> SaveAreaIDList;
				SaveAreaIDList.Add(FName("Empty"));
				OLD_Map_SaveAreaID_To_SaveAreaInfo.GenerateKeyArray(SaveAreaIDList);
				for (const FName SaveAreaID : SaveAreaIDList)
				{
					UReflectionOperations::RemoveOptionFromEnum("ESaveArea_BP", SaveAreaID);
				}
			}
			else
			{
				TArray<FName> CurSaveAreaIDList;
				Map_SaveAreaID_To_SaveAreaInfo.GenerateKeyArray(CurSaveAreaIDList);
				TArray<FName> OldSaveAreaIDList;
				OLD_Map_SaveAreaID_To_SaveAreaInfo.GenerateKeyArray(OldSaveAreaIDList);
				TSet<FName> CurSaveAreaIDCollection(CurSaveAreaIDList);
				TSet<FName> OldSaveAreaIDCollection(OldSaveAreaIDList);
				const TSet<FName> Diff = CurSaveAreaIDCollection.Difference(OldSaveAreaIDCollection);
				
				const bool bIsKeyNameChanged = Diff.Num() > 0;
				if (bIsKeyNameChanged)
				{
					UReflectionOperations::SetNewOptionsToEnum("ESaveArea_BP", CurSaveAreaIDList);
				}
			}
				
			OLD_Map_SaveAreaID_To_SaveAreaInfo = Map_SaveAreaID_To_SaveAreaInfo;
			
			// Map_SaveAreaID_To_SaveAreaInfo 发生了改变，需要删除并重新创建RuntimeData。
			// 只会发生在【编辑器时】，不会在游戏运行时发生。
			DeleteSavedRuntimeDataFile(false);  // 删除原来的文件。
			ResetRuntimeData();  // 重置RuntimeData
		}
		else if (PropertyChangedEvent.Property->GetName() == "RuntimeDataSaveFullPath")
		{
			DeleteSavedRuntimeDataFile(true);  // RuntimeDataSaveFullPath的改变只需要移动文件即可，无需删除
			Old_RuntimeDataSaveFullPath = RuntimeDataSaveFullPath;
		}
		else if (PropertyChangedEvent.Property->GetName() == "SaveLocationFormatStr")
		{
			DeleteSavedRuntimeDataFile(true);  // RuntimeDataSaveFullPath的改变只需要移动文件即可，无需删除
			Old_RuntimeDataSaveFullPath = RuntimeDataSaveFullPath;
		}
		else
		{
			UE_LOG(LogTemp, Display, TEXT("Function:[%s] Property Name: [%s]"), ANSI_TO_TCHAR(__FUNCTION__), *PropertyChangedEvent.Property->GetName());
		}
	}
	
	/**
	 * @brief 编辑器时 当前用户 使用反射系统递归遍历FSaveLoadSystemSaveArea的属性树，找出由SaveGame标记的所有属性，
	 *			再对于【所有的FSaveLoadSystemSaveArea对象】进行 清零【刚才找到的满足条件的属性】
	 * 目前只有一个RuntimeData；
	 * Map_SaveAreaID_To_SaveAreaInfo里面大部分都是在配置中设置的，所以对于所有用户都一样
	 */
	void ResetRuntimeData()
	{
		TArray<FName> KeyList;
		Map_SaveAreaID_To_SaveAreaInfo.GetKeys(KeyList);

		if (LIKELY(KeyList.Num() > 0))
		{
			FPropertyValueIterator It(FProperty::StaticClass(),
					FSaveLoadSystemSaveArea::StaticStruct(),
					&Map_SaveAreaID_To_SaveAreaInfo[KeyList[0]],
					EPropertyValueIteratorFlags::FullRecursion);
			TMap<const FProperty*, int> Map_ResetProperty_To_PropertyOffset;  // 

			// 递归循环查找具有SaveGame标志位的Property，并保存
			// 以【FSaveLoadSystemSaveArea】为基地址的偏移量。
			/// FPropertyValueIterator中的GetPropertyChain不好使。
			/// 所以自己记录 从Map_SaveAreaID_To_SaveAreaInfo[KeyList[0]]开始的偏移量。
			for (; It; ++It)
			{
				if (UNLIKELY(It.Key()->HasAllPropertyFlags(CPF_SaveGame)))
				{
					const int PropertyOffset = (uint8*)(It->Value) - (uint8*)&Map_SaveAreaID_To_SaveAreaInfo[KeyList[0]];
					Map_ResetProperty_To_PropertyOffset.Add(It.Key(), PropertyOffset);
				}
			}

			// 循环 Map_SaveAreaID_To_SaveAreaInfo 中每一个【FSaveLoadSystemSaveArea】值。
			for (int i = 0; i < Map_SaveAreaID_To_SaveAreaInfo.Num(); i++)
			{
				// 对每一个【FSaveLoadSystemSaveArea】，循环 有SaveGame标志位的Property列表。
				for (const auto MapIt : Map_ResetProperty_To_PropertyOffset)
				{
					// 计算被更改的Property的值的地址：ValuePtr。基地址+偏移
					void* PropertyValuePtr = (uint8*)&(Map_SaveAreaID_To_SaveAreaInfo[KeyList[i]]) + MapIt.Value;
					if (MapIt.Key->GetName() == "RollingBufferPos")
					{
						*((int*)PropertyValuePtr) = INDEX_NONE;
					}
					else  // 剩余的变量就通过ClearValue函数进行 重置
					{
						MapIt.Key->ClearValue(PropertyValuePtr);
					}
				}
			}
		}
	}

	class FSimpleDirVisitor : public IPlatformFile::FDirectoryVisitor
	{
	public:
		FSimpleDirVisitor(TArray<FString> *InDirectoryNames)
			: DirectoryNames(InDirectoryNames)
		{}

		void SetResultPath(TArray<FString> *InDirectoryNames) { DirectoryNames = InDirectoryNames; }

		virtual bool Visit(const TCHAR* FilenameOrDirectory, bool bIsDirectory) override
		{
			if (bIsDirectory)
			{
				DirectoryNames->Add(FString(FilenameOrDirectory));
			}
			return true;
		}
	
	private:
		TArray<FString> *DirectoryNames;
	};

	class FSimpleRuntimeDataVisitor : public IPlatformFile::FDirectoryVisitor
	{
	public:
		FSimpleRuntimeDataVisitor(TArray<FString> *InOutputNames)
			: RuntimeDataFileNames(InOutputNames)
		{
			FString NoUse;
			USaveLoadSystemConfig::Get()->RuntimeDataSaveFullPath.Split("/", &NoUse, &RuntimeDataFileName, ESearchCase::IgnoreCase, ESearchDir::FromEnd);
		}

		void SetResultPath(TArray<FString> *InDirectoryNames) { RuntimeDataFileNames = InDirectoryNames; }

		virtual bool Visit(const TCHAR* FilenameOrDirectory, bool bIsDirectory) override
		{
			
			if (bIsDirectory)
			{
				return true;
			}
			FString FileName, NoUse;
			USaveLoadSystemConfig::Get()->RuntimeDataSaveFullPath.Split("/", &NoUse, &FileName, ESearchCase::IgnoreCase, ESearchDir::FromEnd);
			if (FileName == RuntimeDataFileName)
			{
				RuntimeDataFileNames->Add(FilenameOrDirectory);
			}
			return true;
		}
	
	private:
		FString RuntimeDataFileName;
		TArray<FString> *RuntimeDataFileNames;
	};
	/**
	 * @brief 作用：编辑器时 所有用户 删除所有用户的RuntimeData文件
	 *		  时机：通过更改配置而导致Map_SaveAreaID_To_SaveAreaInfo变量发生改变
	 *		  
	 * Map_SaveAreaID_To_SaveAreaInfo里面对于所有用户都一样，如果配置发生改变，需要删除所有用户用于存放RuntimeData的文件.
	 * 并且里面存储的数据也一并失效，删除。
	 *
	 * @param bInJustMoveFile 仅仅移动文件，而不是删除。仅发生在RuntimeDataSaveFullPath变量发生改变时。
	 */
	void DeleteSavedRuntimeDataFile(bool bInJustMoveFile)
	{
		TArray<FString> DeleteFilePathsList;
		FSimpleRuntimeDataVisitor Visitor(&DeleteFilePathsList);
		IFileManager::Get().IterateDirectoryRecursively(*SaveBaseFolder, Visitor);

		for (FString DeletePathName : DeleteFilePathsList)
		{
			IFileManager::Get().Delete(*DeletePathName);  // 该函数可自动处理路径文件不存在。
		}
	}

	/**
	 * @brief 作用：运行时 当前用户 保存RuntimeData到文件中。
	 * 时机：目前由每回滚动缓冲区被使用的时候就保存一次
	 */
	void SaveRuntimeData()
	{
		TArray<uint8> SaveData;
		FMemoryWriter Ar(SaveData, true);
		
		Ar << Map_SaveAreaID_To_SaveAreaInfo;
		FFileHelper::SaveArrayToFile(SaveData, *MakeRuntimeDataSaveFullPath());
	}
	
	FString MakeRuntimeDataSaveFullPath()
	{
		// 获取当前操作的User信息
		const FUserInfo* UserInfo = GetCurrentUserInfo();
		const int32& UserID = UserInfo->BaseInfo.UserID;
		
		FStringFormatNamedArguments Args;
		Args.Add("UserID", FString::FromInt(UserID));
		FString FinalPath = SaveBaseFolder / FString::Format(*RuntimeDataSaveFullPath, Args);
		return FinalPath;
	}

	/**
	 * @brief 作用：运行时 当前用户 读取RuntimeData文件到Map_SaveAreaID_To_SaveAreaInfo中。
	 *				文件不存在的话，初始化Map_SaveAreaID_To_SaveAreaInfo变量，并
	 *		  时机：用户切换的时候调用
	 */
	void LoadRuntimeData()
	{
		FString OperatingPath = SaveBaseFolder / MakeRuntimeDataSaveFullPath();
		if (IFileManager::Get().FileSize(*OperatingPath) >= 0)
		{
			TArray<uint8> SaveData;
			FMemoryReader Ar(SaveData, true);
			
			FFileHelper::LoadFileToArray(SaveData, *OperatingPath);
			Ar << Map_SaveAreaID_To_SaveAreaInfo;
		}
		else
		{
			ResetRuntimeData();
			SaveRuntimeData();  // 重置后再保存到文件。
		}
	}

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Basic Config", meta=(ToolTip="文件头类的选择。"))
	TSubclassOf<USaveGameObjectHeaderBase> SaveHeaderClass = USaveGameObjectHeaderBase::StaticClass();
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Config, Category="Basic Config", AdvancedDisplay, DisplayName="(Deprecated) Slot Name Normal", meta=(ToolTip="使用SlotID存储存档时使用的命名规则"))
	FString SaveSlotNameFormat = "{Prefix}_{UserID}_{SlotID}";
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Config, Category="Basic Config", AdvancedDisplay, DisplayName="(Deprecated) Slot Name When Custom Slot Name", meta=(ToolTip="使用【自定义存档名称】存储存档时使用的命名规则"))
	FString SaveSlotNameFormat_HasCustom = "{CustomSlotName}";
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Config, Category="Basic Config", AdvancedDisplay, meta=(ToolTip="(Deprecated) 存档文件扩展名"))
	FString SaveFileExtension = ".sav";

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Config, Category="Basic Config", meta=(ToolTip="保存运行时的数据（比如：滚动缓冲区的头指针）的文件名以及路径。"))
	FString RuntimeDataSaveFullPath = "{UserID}/Options.sav";
	FString Old_RuntimeDataSaveFullPath = "{UserID}/Options.sav";
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Config, Category="Save Area", meta=(ToolTip))
	TMap<FName, FSaveLoadSystemSaveArea> Map_SaveAreaID_To_SaveAreaInfo;
	TMap<FName, FSaveLoadSystemSaveArea> OLD_Map_SaveAreaID_To_SaveAreaInfo;  // PostPropertyEdit，方便检查是哪个Index的数据发生了改动

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Config, Category="Basic Config", meta=(ToolTip))
	TEnumAsByte<ESaveArea_BP> TestVar = TEnumAsByte<ESaveArea_BP>(0);

protected:
	TMap<int32, TMap<FName, FString>> Map_UserID_To_SavePathMap;
	II_UserManager* UserManagerPtr = nullptr;
	
	FString SaveBaseFolder = FPaths::ProjectSavedDir() / "SaveGames";
};




