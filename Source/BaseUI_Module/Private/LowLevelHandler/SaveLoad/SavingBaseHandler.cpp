// Fill out your copyright notice in the Description page of Project Settings.


#include "LowLevelHandler/SaveLoad/SavingBaseHandler.h"

#include "PlatformFeatures.h"
#include "HAL/FileManagerGeneric.h"
#include "Kismet/GameplayStatics.h"
#include "LowLevelHandler/SaveLoad/SaveLoadSystemGlobals.h"
#include "LowLevelHandler/SaveLoad/MultiUserSaveSystem/DongSaveSystemStatic.h"

const FFunctionHandlerDef USavingBaseHandler::HandlerDef(USavingBaseHandler::StaticClass(), {
	HandlerDependentPair("UserManager", new FFunctionHandlerDependent(UUserManagerBase::StaticClass(),
		NSLOCTEXT("USavingBaseHandler", "UserManager_Tooltip", "本依赖Handler用来获取用户的名称UID等信息用来分用户保存各种存档。")))
});

void USavingBaseHandler::AssignInterfacePtr(UObject* MatchedObjectPtr, UClass* MatchedInterfaceClassPtr)
{
	if (UI_UserManager::StaticClass())
	{
		UserManagerObj = MatchedObjectPtr;
		UserManagerPtr = dynamic_cast<II_UserManager*>(MatchedObjectPtr);
	}
}

void USavingBaseHandler::AssignDependentHandlerPtr()
{
	Super::AssignDependentHandlerPtr();
	UserManagerPtr = dynamic_cast<II_UserManager*>(Map_Purpose_To_HandlerInstance["UserManager"]);
}

void USavingBaseHandler::InitHandler(II_GI_MenuFramework* InGameInstancePtr, TMap<FName, UFunctionHandlerBase*>& InDependencyHandlerDict)
{
	Super::InitHandler(InGameInstancePtr, InDependencyHandlerDict);
}

void USavingBaseHandler::SaveGameToArea_CPP(FDongSaveSystemSavingParam& InParams)
{
	if (UDongSaveSystemStatic_Simple::DoesSaveGameExist(InParams))
	{
		FString&& SlotName = UDongSaveSystemStatic_Simple::GetSaveDataFilePath(InParams);
		UE_LOG(LogTemp, Warning, TEXT("Function:[%s] Save slot: [%s] will be OVERRIDE!"), ANSI_TO_TCHAR(__FUNCTION__), *SlotName);
	}

	if (bUserAsyncSaveLoad)
	{
		UDongSaveSystemStatic_Simple::AsyncSaveGameToSlot(InParams, FAsyncSaveGameToSlotDelegate::CreateUObject(this, &USavingBaseHandler::Handle_AsyncSaveGameToSlotDelegate));
	}
	else
	{
		UDongSaveSystemStatic_Simple::SaveGameToSlot(InParams);
	}
}

bool USavingBaseHandler::LoadGameFromSaveArea_CPP(FDongSaveSystemSavingParam& InOutParams)
{
	if (UDongSaveSystemStatic_Simple::DoesSaveGameExist(InOutParams))
	{
		if (bUserAsyncSaveLoad)
		{
			UDongSaveSystemStatic_Simple::AsyncLoadGameFromSlot(InOutParams, FOnAsyncLoadFromSaveArea::CreateUObject(this, &USavingBaseHandler::Handle_AsyncLoadGameFromSlotDelegate));
		}
		else
		{
			InOutParams.MainSaveObj = UDongSaveSystemStatic_Simple::LoadGameFromSlot(InOutParams);
		}
		return true;
	}
	else
	{
		FString&& SlotName = UDongSaveSystemStatic_Simple::GetSaveDataFilePath(InOutParams);
		UE_LOG(LogTemp, Error, TEXT("Function:[%s] Save slot: [%s] is NOT exist!"), ANSI_TO_TCHAR(__FUNCTION__), *SlotName);
		return false;
	}
}

ECannotSaveReason USavingBaseHandler::CanSave_CPP()
{
	const int UserCount = UserManagerPtr->GetAllUserInfo_CPP().Num();
	if (UserCount == 0)
	{
		return ECannotSaveReason::NoUser;
	}
	else
	{
		return ECannotSaveReason::CanSave;
	}
}

void USavingBaseHandler::GetSaveDataHeaderList_Implementation(
	ESaveArea_BP InSaveAreaID_Enum, TArray<USaveGameObjectHeaderBase*>& OutHeaderList)
{
	const auto SaveAreaID = UReflectionOperations::GetEnumOptionNameString("ESaveArea_BP", InSaveAreaID_Enum);
	GetSaveDataHeaderList_CPP(FName(SaveAreaID), OutHeaderList);
	// return GetSaveDataHeaderList_CPP(FName(SaveAreaID));
}

void USavingBaseHandler::GetSaveDataHeaderList_CPP(FName InSaveAreaID, TArray<USaveGameObjectHeaderBase*>& OutHeaderList)
{
	for (USaveGameObjectHeaderBase* SaveHeader : SaveHeaderList)
	{
		SaveHeader->MarkPendingKill();
	}
	SaveHeaderList.Empty();
	
	TArray<FString> FileList;

	IFileManager& Handler = IFileManager::Get();

	FDongSaveSystemSavingParam Param;
	Param.SaveAreaID = InSaveAreaID;
	const FString BasePath = USaveLoadSystemConfig::GetSavePath(Param);

	Handler.FindFiles(FileList, *(BasePath));

	TArray<uint8> ReadingBuffer;
	for ( const FString FilePath : FileList )
	{
		SaveHeaderList.Add(NewObject<USaveGameObjectHeaderBase>(this));
		UDongSaveSystemStatic_Simple::ReadFileHeaderToMemoryBytes(BasePath / FilePath, ReadingBuffer);
		UDongSaveSystemStatic_Simple::LoadHeaderObjFromMemoryBytes(ReadingBuffer, SaveHeaderList.Last());
	}
	OutHeaderList = SaveHeaderList;
	// return SaveHeaderList;
}

void USavingBaseHandler::GetSaveAreaInfo_Implementation(ESaveArea_BP InSaveAreaID_Enum,
	FSaveLoadSystemSaveArea& OutSaveAreaInfo)
{
	const auto SaveAreaID = UReflectionOperations::GetEnumOptionNameString("ESaveArea_BP", InSaveAreaID_Enum);
	OutSaveAreaInfo = *GetSaveAreaInfo_CPP(FName(SaveAreaID));
}

FSaveLoadSystemSaveArea* USavingBaseHandler::GetSaveAreaInfo_CPP(FName InSaveAreaID)
{
	return &(*USaveLoadSystemConfig::Get()->GetSaveAreaInfo_Map())[InSaveAreaID];
}

void USavingBaseHandler::GetSaveAreaRuntimeInfo_Implementation(ESaveArea_BP InSaveAreaID_Enum,
	FSaveSystemSavingFeature& OutSaveAreaInfo)
{
	const auto SaveAreaID = UReflectionOperations::GetEnumOptionNameString("ESaveArea_BP", InSaveAreaID_Enum);
	OutSaveAreaInfo = (*GetSaveAreaInfo_CPP(FName(SaveAreaID))).SaveGameFeatureRuntimeInfo;
}

int USavingBaseHandler::GetSaveAreaRollingBufferHeaderPos_Implementation(ESaveArea_BP InSaveAreaID_Enum)
{
	const auto SaveAreaID = UReflectionOperations::GetEnumOptionNameString("ESaveArea_BP", InSaveAreaID_Enum);
	return (*GetSaveAreaInfo_CPP(FName(SaveAreaID))).SaveGameFeatureRuntimeInfo.RollingBufferPos;
}

void USavingBaseHandler::GetSaveAreaRuntimeInfo_CPP(FName InSaveAreaID, FSaveSystemSavingFeature& OutSaveAreaInfo)
{
	OutSaveAreaInfo = (*GetSaveAreaInfo_CPP(InSaveAreaID)).SaveGameFeatureRuntimeInfo;
}

int USavingBaseHandler::GetSaveAreaRollingBufferHeaderPos_CPP(FName InSaveAreaID)
{
	return (*GetSaveAreaInfo_CPP(InSaveAreaID)).SaveGameFeatureRuntimeInfo.RollingBufferPos;
}

void USavingBaseHandler::Init(UObject* InUserManagerObj)
{
	UserManagerObj = InUserManagerObj;

	// auto GameInstance = UGameplayStatics::GetGameInstance(this);
	// II_GI_MenuFramework* MainMenuGameInstance(Cast<II_GI_MenuFramework>(GameInstance));
	// USaveLoadSystemConfig::Get()->
	
	if (IsValid(InUserManagerObj))
	{
		UserManagerPtr = static_cast<II_UserManager*>(UserManagerObj.GetInterface());
		USaveLoadSystemConfig::SetUserManagerInstance(UserManagerPtr);
		// FUserInfo UserInfo;
		// if (!UserManagerPtr->FindUser_ByName_CPP(DEFAULT_USER_NAME, UserInfo))
		// {
		// 	UserManagerPtr->CreateNewUser_ByName_CPP(DEFAULT_USER_NAME, FText::GetEmpty(), nullptr);
		// 	UserManagerPtr->FindUser_ByName_CPP(DEFAULT_USER_NAME, UserInfo);
		// 	UserManagerPtr->SwitchCurrentUser_CPP(DEFAULT_USER_NAME);
		// }
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Function:[%s] InUserManagerObj is not a valid [II_UserManager] Interface UObject"), ANSI_TO_TCHAR(__FUNCTION__));
#if WITH_EDITOR
		ensure(false);
#else
		check(false);
#endif
	}
}

void USavingBaseHandler::Handle_AsyncSaveGameToSlotDelegate(const FString& InSlotName, const int32 InUserIndex,
                                                            bool bInIsSucceed)
{
	if (OnSaveFinish_Event_CPP.IsBound())
	{
		OnSaveFinish_Event_CPP.Broadcast(InSlotName, InUserIndex, bInIsSucceed);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Function:[%s] OnSaveFinish_Event_CPP NOT Bind any functions"), ANSI_TO_TCHAR(__FUNCTION__));
	}

	if (OnSaveFinished.IsBound())
	{
		OnSaveFinished.Execute(InSlotName, InUserIndex, bInIsSucceed);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Function:[%s] OnSaveFinished NOT Bind (Maybe bind in Blueprints) any functions"), ANSI_TO_TCHAR(__FUNCTION__));
	}
}

/** Call From:[UDongSaveSystemStatic_Simple::AsyncLoadGameFromSlot]
 *	Call Next:[USaveBaseHandler::Handle_AsyncLoadComplete]
 *
 */
void USavingBaseHandler::Handle_AsyncLoadGameFromSlotDelegate(const FString& InLoadedSlotName,
                                                              const int32 InLoadedUserIndex, FDongSaveSystemSavingParam* InLoadedObjStruct)
{
	if (OnLoadFinish_Event_CPP.IsBound())
	{
		OnLoadFinish_Event_CPP.Broadcast(InLoadedSlotName, InLoadedUserIndex, InLoadedObjStruct);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Function:[%s] OnLoadFinish_Event_CPP NOT Bind any functions"), ANSI_TO_TCHAR(__FUNCTION__));
	}

	if (OnLoadFinished.IsBound())
	{
		OnLoadFinished.Execute(InLoadedSlotName, InLoadedUserIndex, *InLoadedObjStruct);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Function:[%s] OnLoadFinished NOT Bind (Maybe bind in Blueprints) any functions"), ANSI_TO_TCHAR(__FUNCTION__));
	}
}
