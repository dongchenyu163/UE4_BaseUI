// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayClasses/GameInstanceBase/MainMenuGameInstanceBase.h"

#include "SaveGameSystem.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "GameplayClasses/GameInstanceBase/MainMenuGameInstanceConfig.h"
#include "WorldLevelHandler/DA_WidgetInfo.h"
#include "WorldLevelHandler/Examples/TestUI/TestUserGlobalSaveGameBaseObj.h"
#include "WorldLevelHandler/UI_Modules/UI_MapSelection/MapSelectionBaseHandler.h"
#include "WorldLevelHandler/UI_Modules/UI_ResumeMenu/ResumeMenuBaseHandler.h"
#include "WorldLevelHandler/UI_Modules/UI_Save/SaveBaseHandler.h"
#include "WorldLevelHandler/UI_Modules/UI_SinglePlayerMenu/SinglePlayerMenuBaseHandler.h"


void UMainMenuGameInstanceBase::Init()
{
	Super::Init();

	// SinglePlayerFunctionsHandlerObj = NewObject<USinglePlayerMenuBaseHandler>(this, SinglePlayerMenuHandlerClass);
	// MapSelectableHandlerObj = NewObject<UMapSelectionBaseHandler>(this, MapSelectableHandlerClass);
	// ResumeMenuHandlerObj = NewObject<UResumeMenuBaseHandler>(this, ResumeMenuHandlerClass);
	// SavableHandlerObj = NewObject<USaveBaseHandler>(this, SavableHandlerClass);
	// NextLevelHandlerObj = NewObject<UNextLevelBaseHandler>(this, NextLevelHandlerClass);
	//
	// SinglePlayerFunctionsHandlerObj->SetMapSelectableObj_CPP(MapSelectableHandlerObj);
	// ensure(MapInfoDataTable);
	// SinglePlayerFunctionsHandlerObj->SetMapInfoDataTable(MapInfoDataTable);
	// SinglePlayerFunctionsHandlerObj->SetSaveBaseHandler(SavableHandlerObj);
	//
	// UserManager = NewObject<UUserManagerBase>(this, UserManagerClass);
	// SavingBaseHandler = NewObject<USavingBaseHandler>(this, SavingBaseHandlerClass);
	// UserManager->Init();

	// auto UserList = UserManager->GetAllUserInfo_CPP();
	// FUserInfo UserInfo;
	// if (UserList.Num() == 0)
	// {
	// 	UserManager->CreateNewUser_ByName_CPP(FName("DefaultUser"), FText::FromString("DefaultUser"), nullptr);
	// }
	// UserInfo = UserManager->GetAllUserInfo_CPP()[0];
	// UserManager->SwitchCurrentUser_CPP(UserInfo.BaseInfo.UserName);
	
	// SavingBaseHandler->Init(UserManager);

	CreateHandlers(UMainMenuGameInstanceConfig::Get()->HandlerClassDict, Map_HandlerName_To_HandlerObj, this);
	InitHandlers(this, UMainMenuGameInstanceConfig::Get()->HandlerClassDict, Map_HandlerName_To_HandlerObj);
}

void UMainMenuGameInstanceBase::InitHandlers(II_GI_MenuFramework* InGameInstance, TMap<FName, FFunctionHandlerInfo>& InHandlerClassDict, TMap<FName, UFunctionHandlerBase*>& InCreatedHandlerDict)
{
	for (auto Pair : InHandlerClassDict)
	{
		TMap<FName, UFunctionHandlerBase*> DependentHandlerDict;
		for (auto DependentHandlerName : Pair.Value.DependencyHandlerNameCollection)
		{
			DependentHandlerDict.Add(DependentHandlerName, InCreatedHandlerDict[DependentHandlerName]);
		}
		// InHandlerClassDict[Pair.Key].Map_Purpose_To_HandlerName
		InCreatedHandlerDict[Pair.Key]->InitHandler(InGameInstance, DependentHandlerDict);
	}
}

void UMainMenuGameInstanceBase::CreateHandlers(TMap<FName, FFunctionHandlerInfo>& InHandlerClassDict, TMap<FName, UFunctionHandlerBase*>& OutCreatedHandlerDict, UObject* InObjectOuter)
{
	auto& HandlerToCreateInfoDict = InHandlerClassDict;
	TArray<FName> CurrentCreateHandleList;
	HandlerToCreateInfoDict.GetKeys(CurrentCreateHandleList);
	TSet<FName> HandlerNameCollection(CurrentCreateHandleList);

	// 删除掉 依赖项不在待创建Handler的列表。
	for (FName HandlerName : CurrentCreateHandleList)
	{
		const auto& CurrentCreationHandlerInfo = HandlerToCreateInfoDict[HandlerName];
		if ( ! HandlerNameCollection.Includes(CurrentCreationHandlerInfo.DependencyHandlerNameCollection))
		{
			CurrentCreateHandleList.Remove(HandlerName);
		}
	}

	int PreviousRemainHandlerCount = CurrentCreateHandleList.Num();
	int ScanCount = CurrentCreateHandleList.Num();
	while(CurrentCreateHandleList.Num() > 0)
	{
		// 取出第一个待创建Handler的名称
		const FName CurrentCreationHandlerName = CurrentCreateHandleList[0];
		const auto& CurrentCreationHandlerInfo = HandlerToCreateInfoDict[CurrentCreationHandlerName];

		// 从待创建Handler的名称中移除掉取出元素
		CurrentCreateHandleList.RemoveAt(0);

		// 获取待创建Handler的依赖列表
		bool bNoDependency = false;
		auto DependencyList = CurrentCreationHandlerInfo.DependencyHandlerNameCollection;
		for (auto DependencyName : DependencyList)
		{
		
			auto HandlerObj = OutCreatedHandlerDict.Find(DependencyName);
			// 由于依赖的Handler不存在，退出循环。
			if (!HandlerObj)
			{
				bNoDependency = true;
				break;
			}
		}
		
		// 防止出现不存在依赖
		ScanCount--;
		if (ScanCount == 0)
		{
			ScanCount = CurrentCreateHandleList.Num();
		}
		

		// 依赖不满足，将该Handler的名称重新添加到 【待创建Handler的名称】 列表的末尾。
		if (bNoDependency)
		{
			CurrentCreateHandleList.Add(FName(CurrentCreationHandlerName));
			continue;
		}
		
		// 所有依赖已经被创建的话，才继续创建
		UE_LOG(LogTemp, Warning, TEXT("Function:[%s] Create handler of class [%s] with name [%s]"), ANSI_TO_TCHAR(__FUNCTION__), *CurrentCreationHandlerInfo.HandlerClass->GetName(), *CurrentCreationHandlerName.ToString());
		UFunctionHandlerBase* const& CreatedObj = NewObject<UFunctionHandlerBase>(
			InObjectOuter, CurrentCreationHandlerInfo.HandlerClass, CurrentCreationHandlerName);
		CreatedObj->SetHandlerName(CurrentCreationHandlerName);
		OutCreatedHandlerDict.Add(FName(CurrentCreationHandlerName), CreatedObj);
	}
	UE_LOG(LogTemp, Display, TEXT("Function:[%s] MESSAGE"), ANSI_TO_TCHAR(__FUNCTION__));
}

UFunctionHandlerBase* UMainMenuGameInstanceBase::FindHandler_ByName_CPP(FName InHandlerName)
{
	auto FindResult = Map_HandlerName_To_HandlerObj.Find(InHandlerName);
	if (FindResult)
	{
		return *FindResult;
	}
	else
	{
		return nullptr;
	}	
}

UDataTable* UMainMenuGameInstanceBase::GetMapInfoDataTable_Implementation()
{
	return GetMapInfoDataTable_CPP();
}

#pragma region GetMapUIInfo & GetMapUIRuntimeInfo
FMapUIInfo UMainMenuGameInstanceBase::GetMapUIInfo_Implementation(FName InMapIdentifier)
{
	auto Res = GetMapUIInfo_CPP(InMapIdentifier);
	if (Res != nullptr)
	{
		return FMapUIInfo(*Res);
	}
	else
	{
		ensure(false);  // Assert failed, Check [InMapIdentifier] is valid.
		return FMapUIInfo();
	}
}

FMapUIRuntimeInfo UMainMenuGameInstanceBase::GetMapUIRuntimeInfo_Implementation(FName InMapIdentifier)
{
	auto Res = GetMapUIRuntimeInfo_CPP(InMapIdentifier);
	if (Res != nullptr)
	{
		return FMapUIRuntimeInfo(*Res);
	}
	else
	{
		ensure(false);  // Assert failed, Check [InMapIdentifier] is valid.
		return FMapUIRuntimeInfo();
	}
}

FMapUIInfo* UMainMenuGameInstanceBase::GetMapUIInfo_CPP(FName InMapIdentifier)
{
	if (IsValid(MapInfoDataTable))
	{
		FMapUIInfo* InfoPtr = MapInfoDataTable->FindRow<FMapUIInfo>(InMapIdentifier, "GetMapUIInfo_CPP");
		if (InfoPtr == nullptr)
		{
			UE_LOG(LogTemp, Display, TEXT("Function:[%s] MESSAGE"), ANSI_TO_TCHAR(__FUNCTION__));
		}
		return InfoPtr;
	}
	else
	{
		return nullptr;
	}
}

FMapUIRuntimeInfo* UMainMenuGameInstanceBase::GetMapUIRuntimeInfo_CPP(FName InMapIdentifier)
{
	return Map_MapName_To_MapRuntimeInfo.Find(InMapIdentifier);
}

void UMainMenuGameInstanceBase::UpdateFromUserGlobalData_CPP()
{
	UTestUserGlobalSaveGameBaseObj* UserGlobalData = Cast<UTestUserGlobalSaveGameBaseObj>(GetSavableBaseHandler_CPP()->GetUserGlobalData());
	Map_MapName_To_MapRuntimeInfo = UserGlobalData->UserMapsInfo;
}
#pragma endregion GetMapUIInfo & GetMapUIRuntimeInfo

#pragma region Get & Set PlayingMapUIInfo
FMapUIInfo* UMainMenuGameInstanceBase::GetPlayingMapUIInfo_CPP()
{
	return &PlayingMapInfo;
}

void UMainMenuGameInstanceBase::SetPlayingMapUIInfo_CPP(const FMapUIInfo* InNewMapInfo)
{
	PlayingMapInfo = *InNewMapInfo;
}

FMapUIInfo UMainMenuGameInstanceBase::GetPlayingMapUIInfo_Implementation()
{
	return FMapUIInfo(*GetPlayingMapUIInfo_CPP());
}

void UMainMenuGameInstanceBase::SetPlayingMapUIInfo_Implementation(FMapUIInfo InNewMapInfo)
{
	PlayingMapInfo = InNewMapInfo;
}

FMapUIInfo* UMainMenuGameInstanceBase::FindMapUIInfo_ByUWorld(UWorld* InWorldPtr)
{
	// MapInfoDataTable->GetAllRows()
	auto name = FName(FPaths::GetBaseFilename(InWorldPtr->GetPackage()->FileName.ToString()));
	auto res = MapInfoDataTable->FindRow<FMapUIInfo>(name, TEXT("FindCurrentWorld"));
	return (res);
}

#pragma endregion Get & Set PlayingMapUIInfo



void UMainMenuGameInstanceBase::ResetWidgetInfo_CPP()
{
	FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");
	TArray<FAssetData> AssetDataList;
	FARFilter Filter;
	Filter.bRecursivePaths = true;
	Filter.ClassNames.Add(*UDA_WidgetInfo::StaticClass()->GetName());
	Filter.PackagePaths.Add("/Game/");
	AssetRegistryModule.Get().GetAssets(Filter, AssetDataList);
	for (FAssetData AssetData : AssetDataList)
	{
		auto WidgetInfoObj = Cast<UDA_WidgetInfo>(AssetData.GetAsset());
		WidgetInfoObj->WidgetRef = nullptr;
	}
}

void UMainMenuGameInstanceBase::ResetWidgetInfo_Implementation()
{
	ResetWidgetInfo_CPP();
}

void UMainMenuGameInstanceBase::OnStart()
{
	Super::OnStart();
	SavableHandlerObj->Init(SavingBaseHandler, UserManager);
	OnAnyWorldBeginPlay.AddDynamic(SavableHandlerObj, &USaveBaseHandler::Handle_OnAnyWorldChanged);

	// Reset runtime info for UDA_WidgetInfo Obj.
	ResetWidgetInfo_CPP();

	if (IsValid(MapInfoDataTable))
	{
		TArray<FName> MapInfoNameList = MapInfoDataTable->GetRowNames();
		for (const FName RowName : MapInfoNameList)
		{
			const FMapUIInfo* InfoPtr = MapInfoDataTable->FindRow<FMapUIInfo>(RowName, "Init");
			FMapUIRuntimeInfo NewRuntimeInfo;
			NewRuntimeInfo.MapIdentifier = InfoPtr->MapIdentifier;
			NewRuntimeInfo.bIsPlayable = InfoPtr->bInitialPlayable;
			Map_MapName_To_MapRuntimeInfo.Add(InfoPtr->MapIdentifier, NewRuntimeInfo);
		}

		auto World = GetWorld();
		if (World)
		{
			SetPlayingMapUIInfo_CPP(FindMapUIInfo_ByUWorld(World));
		}
	}
}

void UMainMenuGameInstanceBase::OnFirstWorldChanged(UWorld* NewWorld)
{

}

void UMainMenuGameInstanceBase::OnWorldChanged(UWorld* OldWorld, UWorld* NewWorld)
{
	if (NewWorld)  // NewWorld 他妈的居然会为NULL。。。
		{
		if (!NewWorld->OnWorldBeginPlay.IsBoundToObject(this))
		{
			NewWorld->OnWorldBeginPlay.AddLambda(
			   [this, NewWorld]()
			   {
					   if (bIsInit)
					   {
						   bIsInit = false;
						   OnFirstWorldChanged(NewWorld);
					   }
					  if (OnAnyWorldBeginPlay.IsBound())
					  {
						  OnAnyWorldBeginPlay.Broadcast();
					  }
				  }
				  );
		}
		}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Function:[%s] [NewWorld] is nullptr 呵呵"), ANSI_TO_TCHAR(__FUNCTION__));
	}

}


bool VerifyHandlerCreationResult(TMap<FName, UFunctionHandlerBase*> InHandlerDict, TSet<FName> InShouldCreateNameCollection)
{

	// Check Object is valid.
	for (auto CreatedPair : InHandlerDict)
	{
		if (!IsValid(CreatedPair.Value))
		{
			UE_LOG(LogTemp, Error, TEXT("Function:[%s] Object name [%s] create failed."), ANSI_TO_TCHAR(__FUNCTION__), *FName(CreatedPair.Key).ToString());
			return false;
		}
	}

	// Check the created object name is exactly matching the InShouldCreateNameList.
	TArray<FName> CreatedHandlerNameList;
	InHandlerDict.GetKeys(CreatedHandlerNameList);
	TSet<FName> CreatedHandlerNameCollection(CreatedHandlerNameList);

	TSet<FName> Diff = InShouldCreateNameCollection.Difference(CreatedHandlerNameCollection);
	if (Diff.Num() != 0)
	{
		for (FName DiffElement : Diff)
		{
			if (CreatedHandlerNameCollection.Contains(DiffElement))
			{
				UE_LOG(LogTemp, Error, TEXT("Function:[%s] Object name [%s] should NOT be create."), ANSI_TO_TCHAR(__FUNCTION__), *DiffElement.ToString());
			}
			else if (InShouldCreateNameCollection.Contains(DiffElement))
			{
				UE_LOG(LogTemp, Error, TEXT("Function:[%s] Object name [%s] NOT create!!!"), ANSI_TO_TCHAR(__FUNCTION__), *DiffElement.ToString());
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("Function:[%s] Object name [%s] is not expected!!!"), ANSI_TO_TCHAR(__FUNCTION__), *DiffElement.ToString());
			}
		}
		return false;
	}
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FMainMenuGameInstanceBase_CreateHandlerTest, "BaseUI_Module.GameInstance.CreateHandlerTest", EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::EngineFilter)
bool FMainMenuGameInstanceBase_CreateHandlerTest::RunTest(const FString& Parameters)
{
	TMap<FName, FFunctionHandlerInfo> HandlerClassDict;
	TMap<FName, UFunctionHandlerBase*> CreateResult;
	// Normal case.
	HandlerClassDict.Add(FName("MapSelector"), FFunctionHandlerInfo(UMapSelectionBaseHandler::StaticClass()));
	HandlerClassDict.Add(FName("NextLevelHandler"), FFunctionHandlerInfo(UNextLevelBaseHandler::StaticClass()));
	HandlerClassDict.Add(FName("ResumeMenuHandle"), FFunctionHandlerInfo(UResumeMenuBaseHandler::StaticClass()));
	HandlerClassDict.Add(FName("SaveHandler"), FFunctionHandlerInfo(USaveBaseHandler::StaticClass(), { FName("LowLevelSaveHandler") }));
	HandlerClassDict.Add(FName("SinglePlayerMenuHandle"), FFunctionHandlerInfo(USinglePlayerMenuBaseHandler::StaticClass(), { FName("MapSelector") }));
	HandlerClassDict.Add(FName("LowLevelSaveHandler"), FFunctionHandlerInfo(USinglePlayerMenuBaseHandler::StaticClass(), { FName("UserManager") }));
	HandlerClassDict.Add(FName("UserManager"), FFunctionHandlerInfo(USinglePlayerMenuBaseHandler::StaticClass()));

	UMainMenuGameInstanceBase::CreateHandlers(HandlerClassDict, CreateResult);
	TSet<FName> ShouldBeCreateNameList = {
		"MapSelector",
		"NextLevelHandler",
		"ResumeMenuHandle",
		"SaveHandler",
		"SinglePlayerMenuHandle",
		"LowLevelSaveHandler",
		"UserManager"
	};
	const bool bNormalTestResult = VerifyHandlerCreationResult(CreateResult, ShouldBeCreateNameList);
	if (!bNormalTestResult)
	{
		UE_LOG(LogTemp, Error, TEXT("Function:[%s] [BaseUI_Module.GameInstance.CreateHandlerTest] Normal test failed"), ANSI_TO_TCHAR(__FUNCTION__));
	}
	else
	{
		UE_LOG(LogTemp, Display, TEXT("[BaseUI_Module.GameInstance.CreateHandlerTest] Normal test PASS"));
	}
	
	return true;
}
