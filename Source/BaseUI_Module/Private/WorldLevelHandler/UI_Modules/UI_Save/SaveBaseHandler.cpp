// Fill out your copyright notice in the Description page of Project Settings.


#include "WorldLevelHandler/UI_Modules/UI_Save/SaveBaseHandler.h"

#include "ImageUtils.h"
#include "LowLevelHandler/SaveLoad/ClassAndStructs/SaveGameObjBase.h"
#include "WorldLevelHandler/UI_Modules/UI_UserManagement/UserManagementBaseHandler.h"
#include "WorldLevelHandler/UI_Modules/UI_Save/Interfaces/I_CustomSave.h"
#include "Misc/OutputDeviceNull.h"
#include "LowLevelHandler/SaveLoad/MultiUserSaveSystem/DongSaveSystemStatic.h"

// const TMap<FString, UClass*> USaveBaseHandler::Map_Purpose_To_DependenceHandlerClass = {
// 	TPair<FString, UClass*>("LowLevelSaveHandler", USavingBaseHandler::StaticClass()),
// 	TPair<FString, UClass*>("UserManager", UUserManagerBase::StaticClass()),
// };
// const TMap<FString, FText> USaveBaseHandler::Map_Purpose_To_PurposeTooltip = {
// 	TPair<FString, FText>("LowLevelSaveHandler", NSLOCTEXT("USaveBaseHandler", "LowLevelSaveHandler_Tooltip", "本依赖Handler用来获取用户的名称UID等信息用来分用户保存各种存档。")),
// 	TPair<FString, FText>("UserManager", NSLOCTEXT("USaveBaseHandler", "UserManager_Tooltip", "本依赖Handler用来获取用户的名称UID等信息用来分用户保存各种存档。")),
// };
const FFunctionHandlerDef USaveBaseHandler::HandlerDef(USaveBaseHandler::StaticClass(), {
	HandlerDependentPair("LowLevelSaveHandler", new FFunctionHandlerDependent(USavingBaseHandler::StaticClass(),
		NSLOCTEXT("USavingBaseHandler", "UserManager_Tooltip", "本依赖Handler用来获取用户的名称UID等信息用来分用户保存各种存档。"))),
	HandlerDependentPair("UserManager", new FFunctionHandlerDependent(UUserManagerBase::StaticClass(),
		NSLOCTEXT("USavingBaseHandler", "UserManager_Tooltip", "本依赖Handler用来获取用户的名称UID等信息用来分用户保存各种存档。"))),
});

TSet<UClass*> USaveBaseHandler::GetDependenceHandlerInterfaceCollection()
{
	static const TSet<UClass*> DependenceHandlerClassCollection = {
		UI_Save::StaticClass(),
		UI_UserManager::StaticClass()
	};
	return DependenceHandlerClassCollection;
}

void USaveBaseHandler::AssignInterfacePtr(UObject* MatchedObjectPtr, UClass* MatchedInterfaceClassPtr)
{
	Super::AssignInterfacePtr(MatchedObjectPtr, MatchedInterfaceClassPtr);
	if (UI_Save::StaticClass())
	{
		SavingHandlePtr = dynamic_cast<II_Save*>(MatchedObjectPtr);
	}
	else if (UI_UserManager::StaticClass())
	{
		UserManagerPtr = dynamic_cast<II_UserManager*>(MatchedObjectPtr);
	}
}

void USaveBaseHandler::AssignDependentHandlerPtr()
{
	Super::AssignDependentHandlerPtr();
	SavingHandlePtr = dynamic_cast<II_Save*>(Map_Purpose_To_HandlerInstance["LowLevelSaveHandler"]);
	UserManagerPtr = dynamic_cast<II_UserManager*>(Map_Purpose_To_HandlerInstance["UserManager"]);
}

void USaveBaseHandler::InitHandler(II_GI_MenuFramework* InGameInstancePtr,
	TMap<FName, UFunctionHandlerBase*>& InDependencyHandlerDict)
{
	Super::InitHandler(InGameInstancePtr, InDependencyHandlerDict);
}

void USaveBaseHandler::OnStart()
{
	Super::OnStart();

	SavingHandlePtr->GetGameLoadFinishDelegate_CPP()->AddUObject(this, &USaveBaseHandler::Handle_AsyncLoadComplete);
	SavingHandlePtr->GetGameSaveFinishDelegate_CPP()->AddUObject(this, &USaveBaseHandler::Handle_AsyncSaveComplete);
	
	if (SavingHandlePtr->CanSave_CPP() == ECannotSaveReason::CanSave)
	{
		LoadUserGlobalData_CPP();
	}
	
	UserManagerPtr->GetOnUserChangedDelegate_CPP()->BindDynamic(this, &USaveBaseHandler::Handle_OnUserChanged);
}

void USaveBaseHandler::OnWorldChanged(UWorld* OldWorld, UWorld* NewWorld)
{
	Super::OnWorldChanged(OldWorld, NewWorld);
}

void USaveBaseHandler::OnNewWorldBeginPlay()
{
	Super::OnNewWorldBeginPlay();
	Handle_OnAnyWorldChanged();
}

void USaveBaseHandler::SaveUserGlobalData_CPP()
{
#if WITH_EDITOR
	ensure(SavingHandlePtr);
#else
	check(SavingHandlePtr);
#endif
	// UserGlobalData_SaveGameObj->DoUpdateUserGlobalData();
	// SavingHandlePtr->SaveGame_Custom_CPP(UserGlobalData_SaveGameObj, UserGlobalData_SlotNameString);
	const FName SaveAreaID(UReflectionOperations::GetEnumOptionNameString("ESaveArea_BP", UUISaveLoadSystemConfig::Get()->UserGlobalDataSaveAreaID));
	FDongSaveSystemSavingParam Param;
	Param.AsOfficialSave(SaveAreaID, UserGlobalData_SaveGameObj, UUISaveLoadSystemConfig::Get()->UserGlobalData_SlotNameString);
	SavingHandlePtr->SaveGameToArea_CPP(Param);
}

void USaveBaseHandler::LoadUserGlobalData_CPP()
{
#if WITH_EDITOR
	ensure(SavingHandlePtr);
#else
	check(SavingHandlePtr);
#endif

	const FName SaveAreaID(UReflectionOperations::GetEnumOptionNameString("ESaveArea_BP", UUISaveLoadSystemConfig::Get()->UserGlobalDataSaveAreaID));
	FDongSaveSystemSavingParam Param;
	Param.AsIsSlotNameExist(SaveAreaID, INDEX_NONE, UUISaveLoadSystemConfig::Get()->UserGlobalData_SlotNameString);
	const bool bHasGlobalSaveData = SavingHandlePtr->DoesSlotExistInSaveArea_CPP(Param);
	// const bool bHasGlobalSaveData = SavingHandlePtr->DoesSlotExist_ForCustomSlotName_CPP(UserGlobalData_SlotNameString);
	if (bHasGlobalSaveData)
	{
		// USaveGame* TempObj = Cast<USaveGame>(UserGlobalData_SaveGameObj);

		// 同步加载用户进度信息。
		SavingHandlePtr->SetUseAsyncOperation_CPP(false);
		FDongSaveSystemSavingParam Param1;
		Param1.AsOfficialSave(SaveAreaID, nullptr, UUISaveLoadSystemConfig::Get()->UserGlobalData_SlotNameString);
		SavingHandlePtr->LoadGameFromSaveArea_CPP(Param1);
		// SavingHandlePtr->LoadGame_Custom_CPP(UserGlobalData_SlotNameString, TempObj);
		SavingHandlePtr->SetUseAsyncOperation_CPP(true);

		// UserGlobalData_SaveGameObj = Cast<UUserGlobalDataSaveGameObjBase>(TempObj);
		UserGlobalData_SaveGameObj = Cast<UUserGlobalDataSaveGameObjBase>(Param1.MainSaveObj);
		UserGlobalData_SaveGameObj->Rename(nullptr, this);
	}
	else
	{
		if (CreateNewUserGlobalData())
		{
			SaveUserGlobalData_CPP();
		}
	}
}

bool USaveBaseHandler::CreateNewUserGlobalData_Implementation()
{
	if (UserGlobalData_SaveGameObjClass == UUserGlobalDataSaveGameObjBase::StaticClass())
	{
		UE_LOG(LogTemp, Warning, TEXT("Function:[%s] Class you selected should NOT be [UUserGlobalDataSaveGameObjBase], select child class instead!"), ANSI_TO_TCHAR(__FUNCTION__));
		return false;
	}

	if (IsValid(UserGlobalData_SaveGameObj))
	{
		UserGlobalData_SaveGameObj->MarkPendingKill();
	}
	UserGlobalData_SaveGameObj = NewObject<UUserGlobalDataSaveGameObjBase>(this, UserGlobalData_SaveGameObjClass);
	return true;
}

void USaveBaseHandler::GetSaveDataHeaderList_CPP(FName InSaveAreaID, TArray<USaveGameObjectHeaderBase*>& OutHeaderList)
{
#if WITH_EDITOR
	ensure(SavingHandlePtr);
#else
	check(SavingHandlePtr);
#endif
	
	SavingHandlePtr->GetSaveDataHeaderList_CPP(InSaveAreaID, OutHeaderList);
}

void USaveBaseHandler::GetSaveDataHeaderList_Implementation(ESaveArea_BP InSaveAreaID_Enum,
	TArray<USaveGameObjectHeaderBase*>& OutHeaderList)
{
#if WITH_EDITOR
	ensure(SavingHandlePtr);
#else
	check(SavingHandlePtr);
#endif
	
	const auto SaveAreaID = UReflectionOperations::GetEnumOptionNameString("ESaveArea_BP", InSaveAreaID_Enum);
	SavingHandlePtr->GetSaveDataHeaderList_CPP(FName(SaveAreaID), OutHeaderList);
}

void USaveBaseHandler::SaveGame_CPP(int32 InSlotNum)
{
#if WITH_EDITOR
	ensure(SavingHandlePtr);
#else
	check(SavingHandlePtr);
#endif
	const FName SaveAreaID(UReflectionOperations::GetEnumOptionNameString("ESaveArea_BP", UUISaveLoadSystemConfig::Get()->NormalSaveAreaID));
	PrepareSaveData(InSlotNum);

	FDongSaveSystemSavingParam Param;
	Param.AsNumberSave(SaveAreaID, LevelSavingData, LevelSavingData->PlayingInfo, InSlotNum);
	SavingHandlePtr->SaveGameToArea_CPP(Param);
	// SavingHandlePtr->SaveGame_CPP(LevelSavingData, LevelSavingData->PlayingInfo, InSlotNum);
}

void USaveBaseHandler::SaveGame_Auto_CPP()
{
#if WITH_EDITOR
	ensure(SavingHandlePtr);
#else
	check(SavingHandlePtr);
#endif
	const FName SaveAreaID(UReflectionOperations::GetEnumOptionNameString("ESaveArea_BP", UUISaveLoadSystemConfig::Get()->AutoSaveAreaID));
	const int SaveSlotIndex = SavingHandlePtr->GetSaveAreaRollingBufferHeaderPos_CPP(SaveAreaID);
	PrepareSaveData(SaveSlotIndex);
	
	FDongSaveSystemSavingParam Param;
	Param.AsNumberSave(SaveAreaID, LevelSavingData, LevelSavingData->PlayingInfo, SaveSlotIndex);
	SavingHandlePtr->SaveGameToArea_CPP(Param);

	// PrepareSaveData(SavingHandlePtr->GetAutoSaveSlotIndex_CPP());
	// SavingHandlePtr->SaveGame_Auto_CPP(LevelSavingData, LevelSavingData->PlayingInfo);
}

void USaveBaseHandler::SaveGame_Quick_CPP()
{
#if WITH_EDITOR
	ensure(SavingHandlePtr);
#else
	check(SavingHandlePtr);
#endif
	const FName SaveAreaID(UReflectionOperations::GetEnumOptionNameString("ESaveArea_BP", UUISaveLoadSystemConfig::Get()->QuickSaveAreaID));
	const int SaveSlotIndex = SavingHandlePtr->GetSaveAreaRollingBufferHeaderPos_CPP(SaveAreaID);
	PrepareSaveData(SaveSlotIndex);
	
	FDongSaveSystemSavingParam Param;
	Param.AsNumberSave(SaveAreaID, LevelSavingData, LevelSavingData->PlayingInfo, SaveSlotIndex);
	SavingHandlePtr->SaveGameToArea_CPP(Param);
	
	// PrepareSaveData(SavingHandlePtr->GetQuickSaveSlotIndex_CPP());
	// SavingHandlePtr->SaveGame_Auto_CPP(LevelSavingData, LevelSavingData->PlayingInfo);
}

void USaveBaseHandler::LoadGame_CPP(int32 InSlotNum)
{
#if WITH_EDITOR
	ensure(SavingHandlePtr);
#else
	check(SavingHandlePtr);
#endif
	const FName SaveAreaID(UReflectionOperations::GetEnumOptionNameString("ESaveArea_BP", UUISaveLoadSystemConfig::Get()->NormalSaveAreaID));
	PrepareForLoading();
	
	FDongSaveSystemSavingParam Param;
	Param.AsNumberSave(SaveAreaID, LevelSavingData, LevelSavingData->PlayingInfo, InSlotNum);
	SavingHandlePtr->LoadGameFromSaveArea_CPP(Param);
	
	// PrepareForLoading();
	// SavingHandlePtr->LoadGame_CPP(InSlotNum, (USaveGame*&)LevelSavingData);  // 异步加载，可以不用传LevelSavingData
}

void USaveBaseHandler::LoadGame_Auto_CPP()
{
#if WITH_EDITOR
	ensure(SavingHandlePtr);
#else
	check(SavingHandlePtr);
#endif
	const FName SaveAreaID(UReflectionOperations::GetEnumOptionNameString("ESaveArea_BP", UUISaveLoadSystemConfig::Get()->AutoSaveAreaID));
	const int SaveSlotIndex = SavingHandlePtr->GetSaveAreaRollingBufferHeaderPos_CPP(SaveAreaID);
	PrepareForLoading();
	
	FDongSaveSystemSavingParam Param;
	Param.AsNumberSave(SaveAreaID, LevelSavingData, LevelSavingData->PlayingInfo, SaveSlotIndex);
	SavingHandlePtr->LoadGameFromSaveArea_CPP(Param);
	
	// PrepareForLoading();
	// SavingHandlePtr->LoadGame_Auto_CPP((USaveGame*&)LevelSavingData);  // 异步加载，可以不用传LevelSavingData，该参数无意义。
}

void USaveBaseHandler::LoadGame_Quick_CPP()
{
#if WITH_EDITOR
	ensure(SavingHandlePtr);
#else
	check(SavingHandlePtr);
#endif
	const FName SaveAreaID(UReflectionOperations::GetEnumOptionNameString("ESaveArea_BP", UUISaveLoadSystemConfig::Get()->QuickSaveAreaID));
	const int SaveSlotIndex = SavingHandlePtr->GetSaveAreaRollingBufferHeaderPos_CPP(SaveAreaID);
	PrepareForLoading();
	
	FDongSaveSystemSavingParam Param;
	Param.AsNumberSave(SaveAreaID, LevelSavingData, LevelSavingData->PlayingInfo, SaveSlotIndex);
	SavingHandlePtr->LoadGameFromSaveArea_CPP(Param);
	
	// PrepareForLoading();
	// SavingHandlePtr->LoadGame_Quick_CPP((USaveGame*&)LevelSavingData);  // 异步加载，可以不用传LevelSavingData
}

void USaveBaseHandler::Init(UObject* InSavingHandleObj, UObject* InUserManagerObj)
{
	SavingHandleObj = InSavingHandleObj;
	TWeakObjectPtr<UObject> Viewer = InSavingHandleObj;

	CreateNewLevelSavingData();

	if (Viewer.IsValid() && IsValid(InSavingHandleObj))
	{
		SavingHandlePtr = static_cast<II_Save*>(SavingHandleObj.GetInterface());

		SavingHandlePtr->GetGameLoadFinishDelegate_CPP()->AddUObject(this, &USaveBaseHandler::Handle_AsyncLoadComplete);
		SavingHandlePtr->GetGameSaveFinishDelegate_CPP()->AddUObject(this, &USaveBaseHandler::Handle_AsyncSaveComplete);
		
		if (SavingHandlePtr->CanSave_CPP() == ECannotSaveReason::CanSave)
		{
			LoadUserGlobalData_CPP();
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Function:[%s] InSavingHandleObj is not a valid [II_Save] Interface UObject"), ANSI_TO_TCHAR(__FUNCTION__));
#if WITH_EDITOR
		ensure(false);
#else
		check(false);
#endif
	}

	Viewer = InUserManagerObj;
	if (Viewer.IsValid() && IsValid(InUserManagerObj))
	{
		UserManagerPtr = Cast<II_UserManager>(InUserManagerObj);
		UserManagerPtr->GetOnUserChangedDelegate_CPP()->BindDynamic(this, &USaveBaseHandler::Handle_OnUserChanged);
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

void USaveBaseHandler::PrepareSaveData(int32 InSlotNum)
{
	// 准备关卡缩略图，并缩小到384*384
	TArray<FColor> OutImageData_Org;
	TArray<FColor> OutImageData;
	GetViewportScreenShot(GetWorld()->GetGameViewport()->Viewport, OutImageData_Org);
	FVector2D OutSize_Org;
	const FVector2D& OutSize = UUISaveLoadSystemConfig::Get()->ThumbnailSize;
	GetWorld()->GetGameViewport()->GetViewportSize(OutSize_Org);
	FImageUtils::ImageResize(OutSize_Org.X,  OutSize_Org.Y, OutImageData_Org, OutSize.X,  OutSize.Y, OutImageData, true);

	ensure(OutSize.X * OutSize.Y == OutImageData.Num());

	FCreateTexture2DParameters Param;
	Param.bUseAlpha = true;
	Param.bSRGB = false;
	Param.bDeferCompression = false;
	Param.CompressionSettings = TextureCompressionSettings::TC_VectorDisplacementmap;
	Param.MipGenSettings = TextureMipGenSettings::TMGS_NoMipmaps;
	UTexture2D* Texture = FImageUtils::CreateTexture2D(OutSize.X, OutSize.Y, OutImageData, LevelSavingData, "LevelScreenShot", EObjectFlags::RF_Public, Param);
	LevelSavingData->PlayingInfo = NewObject<USaveGameObjectHeaderBase>(this);
	LevelSavingData->PlayingInfo->MapThumbnail = Texture;

	// 准备其他信息
	LevelSavingData->PlayingInfo->SavedEngineVersion = FEngineVersion::Current();
	LevelSavingData->PlayingInfo->SlotIndex = InSlotNum;
	LevelSavingData->PlayingInfo->PlayingMapIdentifier = GetFrameworkGameInstance_CPP()->GetPlayingMapUIInfo_CPP()->MapIdentifier;
	LevelSavingData->PlayingInfo->SaveTime = FDateTime::Now();
	
	// 将关卡中标记要保存的Actor进行序列化。
	WriteLevelActorsToSaveObject();
}

void USaveBaseHandler::Handle_AsyncSaveComplete(const FString& InSlotName, int InUserIndex, bool bInIsSucceed)
{
	if (bInIsSucceed)
	{
		LevelSavingData->MarkPendingKill();
		LevelSavingData = nullptr;
		CreateNewLevelSavingData_Implementation();
	}
}

void USaveBaseHandler::Handle_OnUserChanged(const FUserInfo& InOldUser, const FUserInfo& InNewUser)
{
	LoadUserGlobalData_CPP();
}

void USaveBaseHandler::PrepareForLoading()
{
	// 显示【加载界面】的UI。
	if (IsValid(SaveSystemLoadingScreen))
	{
		// 如果当前【加载界面对象】的类型 和 【加载界面类型】不一致，代表人为切换了【加载界面类型】
		// 此时就需要删除并重新创建【加载界面对象】。
		const auto ConfigClass = UUISaveLoadSystemConfig::Get()->SaveSystemLoadingScreenWidgetClass;
		if (SaveSystemLoadingScreen->GetClass() != ConfigClass)
		{
			// 删除旧对象
			SaveSystemLoadingScreen->MarkPendingKill();
			// 使用SaveSystemLoadingScreenWidgetClass创建新对象
			SaveSystemLoadingScreen = NewObject<UUSaveSystemLoadingScreenBase>(this, ConfigClass);
		}
		
		SaveSystemLoadingScreen->AddToViewport();
		SaveSystemLoadingScreen->OnStartingLoadingScreen();
	}
}

/** Call from: [USavingBaseHandler::Handle_AsyncLoadGameFromSlotDelegate]
 *  
 */
void USaveBaseHandler::Handle_AsyncLoadComplete(const FString& InLoadedSlotName, int InLoadedUserIndex, FDongSaveSystemSavingParam* InLoadedObjStruct)
{
	// 读取存档本身
	LevelSavingData = Cast<ULevelActorsSaveGameObjBase>(InLoadedObjStruct->MainSaveObj);
	LevelSavingData->Rename(nullptr, this);

	// 获取World信息
	auto MapInfoDataTable = GetFrameworkGameInstance_CPP()->GetMapInfoDataTable_CPP();
	auto MapSelector = GetFrameworkGameInstance_CPP()->GetMapSelector_CPP();
	auto& MapID = LevelSavingData->PlayingInfo->PlayingMapIdentifier;
	auto MapInfo = MapInfoDataTable->FindRow<FMapUIInfo>(MapID, TEXT("Load another level save data"));

#if WITH_EDITOR
	ensure(MapInfo);
#else
	check(MapInfo);
#endif

	// 首先加载World，等待World加载完毕后再进行Actor的恢复。
	MapSelector->LoadMap_CPP(MapInfo);
	// 待World加载完毕后跳转到【USaveBaseHandler::Handle_OnAnyWorldChanged()】函数
	// 就是下面的函数。
	bIsWaitingWorldChanged = true;
}

void USaveBaseHandler::Handle_OnAnyWorldChanged()
{
	if (!bIsWaitingWorldChanged)
	{
		return;
	}
	bIsWaitingWorldChanged = false;
	
	// 切换到新的World后，可能UI也跟着之前的World被移除显示；需要重新显示。
	// SaveSystemLoadingScreen->AddToViewport();

	//
	LoadLevelActorsFromSaveObject();

	//
	if (IsValid(SaveSystemLoadingScreen))
	{
		SaveSystemLoadingScreen->OnLoadingComplete();
	}
}

bool USaveBaseHandler::CreateNewLevelSavingData_Implementation()
{
	// if (LevelSavingData_SaveGameObjClass == ULevelActorsSaveGameObjBase::StaticClass())
	// {
	// 	UE_LOG(LogTemp, Warning, TEXT("Function:[%s] Class you selected should NOT be [UUserGlobalDataSaveGameObjBase], select child class instead!"), ANSI_TO_TCHAR(__FUNCTION__));
	// 	return false;
	// }

	if (IsValid(LevelSavingData))
	{
		// LevelSavingData->MarkPendingKill();
		return false;
	}
	LevelSavingData = NewObject<ULevelActorsSaveGameObjBase>(this, LevelSavingData_SaveGameObjClass);
	return true;
}

void USaveBaseHandler::LoadLevelActorsFromSaveObject() const
{
	if (IsValid(LevelSavingData))
	{
		// 制作【Actor名称】->【Actor对象】的字典
		TArray<AActor*> ActorsInLevel = GetWorld()->GetLevel(0)->Actors;
		TMap<FName, AActor*> Map_ActorName_To_Actor;
		for (auto ActorInLevel : ActorsInLevel)
		{
			if (IsValid(ActorInLevel))
			{
				Map_ActorName_To_Actor.Add(ActorInLevel->GetFName(), ActorInLevel);
			}
		}
		
		// 对存档中保存的每一个【Actor信息】
		for (int i = 0; i < LevelSavingData->LevelObjectInfoList.Num(); i++)
		{
			auto ActorInfo = LevelSavingData->LevelObjectInfoList[i];
			const bool bHasImplementedCustomSaveInterface = ActorInfo.ActorClass->ImplementsInterface(UI_CustomSave::StaticClass());
			if (!bHasImplementedCustomSaveInterface)
			{
				continue;
			}

			// 在当前关卡中寻找同名的Actor，存在的话则直接对该Actor进行反序列化
			AActor* OperatingActor = nullptr;
			AActor** FoundRes = Map_ActorName_To_Actor.Find(ActorInfo.ActorName);
			// 不存在的话就创建一个新的Actor
			if (FoundRes == nullptr)
			{
				FActorSpawnParameters ActorSpawnParam;
				ActorSpawnParam.Name = ActorInfo.ActorName;
				OperatingActor = GetWorld()->SpawnActor<AActor>(ActorInfo.ActorClass, ActorSpawnParam);
			}
			else
			{
				OperatingActor = *FoundRes;
			}

			// 该Actor必须继承II_CustomSave接口。调用函数进行反序列化
			II_CustomSave* CustomSavePtr = Cast<II_CustomSave>(OperatingActor);
			const bool bActorHasCustomSaveInterface = CustomSavePtr != nullptr;
			if (bActorHasCustomSaveInterface)
			{
				CustomSavePtr->LoadFromByteArray_CPP(ActorInfo.ObjectByteArray);
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("Function:[%s] Actor [%s] with class [%s] \
					is not implement [II_CustomSave] interface, \
					or not inherited in C++"), 
					ANSI_TO_TCHAR(__FUNCTION__),
					*OperatingActor->GetName(),
					*ActorInfo.ActorClass->GetName()
					);
			}
		}
	}
}

void USaveBaseHandler::WriteLevelActorsToSaveObject()
{
	TArray<AActor*> ActorsInLevel = GetWorld()->GetLevel(0)->Actors;
	if (ActorsInLevel.Num() == 0)
	{
		UE_LOG(LogTemp, Display, TEXT("Function:[%s] No actors in this level!"), ANSI_TO_TCHAR(__FUNCTION__));
		return;
	}

	TArray<AActor*> ActorsWithSaveTag;
	UGameplayStatics::GetAllActorsWithTag(ActorsInLevel[0], FilterActorTagName, ActorsWithSaveTag);
 	for (const auto Actor : ActorsWithSaveTag)
 	{
 		II_CustomSave* CustomSavePtr = Cast<II_CustomSave>(Actor);

 		const bool bActorHasCustomSaveInterface = CustomSavePtr != nullptr;
 		if (bActorHasCustomSaveInterface)
 		{
 			FActorSavingInfo ActorSavingInfo;
 			CustomSavePtr->GetSaveByteArray_CPP(ActorSavingInfo.ObjectByteArray);
 			ActorSavingInfo.ActorClass = Actor->GetClass();
 			ActorSavingInfo.ActorName = Actor->GetFName();
 			LevelSavingData->LevelObjectInfoList.Add(ActorSavingInfo);
 		}
 	}
}
