// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayClasses/GameInstanceBase/MainMenuGameInstanceBase.h"

#include "SaveGameSystem.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "WorldLevelHandler/DA_WidgetInfo.h"
#include "WorldLevelHandler/Examples/TestUI/TestUserGlobalSaveGameBaseObj.h"
#include "WorldLevelHandler/UI_Modules/UI_MapSelection/MapSelectionBaseHandler.h"
#include "WorldLevelHandler/UI_Modules/UI_ResumeMenu/ResumeMenuBaseHandler.h"
#include "WorldLevelHandler/UI_Modules/UI_Save/SaveBaseHandler.h"
#include "WorldLevelHandler/UI_Modules/UI_SinglePlayerMenu/SinglePlayerMenuBaseHandler.h"


void UMainMenuGameInstanceBase::Init()
{
	Super::Init();

	SinglePlayerFunctionsHandlerObj = NewObject<USinglePlayerMenuBaseHandler>(this, SinglePlayerMenuHandlerClass);
	MapSelectableHandlerObj = NewObject<UMapSelectionBaseHandler>(this, MapSelectableHandlerClass);
	ResumeMenuHandlerObj = NewObject<UResumeMenuBaseHandler>(this, ResumeMenuHandlerClass);
	SavableHandlerObj = NewObject<USaveBaseHandler>(this, SavableHandlerClass);
	NextLevelHandlerObj = NewObject<UNextLevelBaseHandler>(this, NextLevelHandlerClass);

	SinglePlayerFunctionsHandlerObj->SetMapSelectableObj_CPP(MapSelectableHandlerObj);
	ensure(MapInfoDataTable);
	SinglePlayerFunctionsHandlerObj->SetMapInfoDataTable(MapInfoDataTable);
	SinglePlayerFunctionsHandlerObj->SetSaveBaseHandler(SavableHandlerObj);

	UserManager = NewObject<UUserManagerBase>(this, UserManagerClass);
	SavingBaseHandler = NewObject<USavingBaseHandler>(this, SavingBaseHandlerClass);
	UserManager->Init();

	// auto UserList = UserManager->GetAllUserInfo_CPP();
	// FUserInfo UserInfo;
	// if (UserList.Num() == 0)
	// {
	// 	UserManager->CreateNewUser_ByName_CPP(FName("DefaultUser"), FText::FromString("DefaultUser"), nullptr);
	// }
	// UserInfo = UserManager->GetAllUserInfo_CPP()[0];
	// UserManager->SwitchCurrentUser_CPP(UserInfo.BaseInfo.UserName);
	
	SavingBaseHandler->Init(UserManager);
	

}

UFunctionHandlerBase* UMainMenuGameInstanceBase::FindHandler_ByName_CPP(FName InHandlerName)
{
	return nullptr;
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

FMapUIInfo* UMainMenuGameInstanceBase::FindMapUIInfo_ByUWorld(UWorld* InWorldPtr)
{
	// MapInfoDataTable->GetAllRows()
	auto name = FName(FPaths::GetBaseFilename(InWorldPtr->GetPackage()->FileName.ToString()));
	auto res = MapInfoDataTable->FindRow<FMapUIInfo>(name, TEXT("FindCurrentWorld"));
	return (res);
}

#pragma endregion Get & Set PlayingMapUIInfo

