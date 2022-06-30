// Fill out your copyright notice in the Description page of Project Settings.


#include "WorldLevelHandler/NormalHandlers/MapsInfoHandler/MapsInfoHandler.h"

#include "WorldLevelHandler/UI_Modules/UI_Save/SaveBaseHandler.h"


const FFunctionHandlerDef UMapsInfoHandler::HandlerDef(UMapsInfoHandler::StaticClass(), {
   HandlerDependentPair("SaveHandler", new FFunctionHandlerDependent(USaveBaseHandler::StaticClass(),
		NSLOCTEXT("UMapsInfoHandler", "SaveHandler_Tooltip", "本依赖Handler用来获取用户的名称UID等信息用来分用户保存各种存档。"))),
   HandlerDependentPair("LowLevelSaveHandler", new FFunctionHandlerDependent(USavingBaseHandler::StaticClass(),
        NSLOCTEXT("UMapsInfoHandler", "LowLevelSaveHandler_Tooltip", "本依赖Handler用来获取用户的名称UID等信息用来分用户保存各种存档。"))),
   HandlerDependentPair("UserManager", new FFunctionHandlerDependent(UUserManagerBase::StaticClass(),
        NSLOCTEXT("UMapsInfoHandler", "UserManager_Tooltip", "本依赖Handler用来获取用户的名称UID等信息用来分用户保存各种存档。"))),
});

void UMapsInfoHandler::AssignDependentHandlerPtr()
{
	Super::AssignDependentHandlerPtr();
	SaveHandlePtr = dynamic_cast<II_UI_Savable*>(Map_Purpose_To_HandlerInstance["SaveHandler"]);
	LowLevelSavingHandlePtr = dynamic_cast<II_Save*>(Map_Purpose_To_HandlerInstance["LowLevelSaveHandler"]);
	UserManagerPtr = dynamic_cast<II_UserManager*>(Map_Purpose_To_HandlerInstance["UserManager"]);
}

void UMapsInfoHandler::InitHandler(II_GI_MenuFramework* InGameInstancePtr,
	TMap<FName, UFunctionHandlerBase*>& InDependencyHandlerDict)
{
	Super::InitHandler(InGameInstancePtr, InDependencyHandlerDict);
}

void UMapsInfoHandler::OnStart()
{
	Super::OnStart();
	if (IsValid(MapInfoDataTable))
	{
		TArray<FName> MapInfoNameList = MapInfoDataTable->GetRowNames();
		for (const FName RowName : MapInfoNameList)
		{
			const FMapInfo* InfoPtr = MapInfoDataTable->FindRow<FMapInfo>(RowName, "Init");
			FMapInfo_User NewRuntimeInfo;
			NewRuntimeInfo.MapIdentifier = InfoPtr->MapIdentifier;
			NewRuntimeInfo.bIsPlayable = InfoPtr->bInitialPlayable;
			Map_MapName_To_MapRuntimeInfo.Add(InfoPtr->MapIdentifier, NewRuntimeInfo);
		}

		auto World = GetWorld();
		if (World)
		{
			PlayingMapInfo = *FindMapUIInfo_ByUWorld(World);
		}
	}
}

bool UMapsInfoHandler::GetMapInfo(FName InMapIdentifier, FMapInfo& OutMapInfo)
{
	if (IsValid(MapInfoDataTable))
	{
		FMapInfo* InfoPtr = MapInfoDataTable->FindRow<FMapInfo>(InMapIdentifier, "UMapsInfoHandler::GetMapInfo");
		if (InfoPtr == nullptr)
		{
			UE_LOG(LogTemp, Display, TEXT("Function:[%s] MESSAGE"), ANSI_TO_TCHAR(__FUNCTION__));
		}
		OutMapInfo = *InfoPtr;
		return true;
	}
	else
	{
		return false;
	}
}

void UMapsInfoHandler::UpdateFromUserGlobalData()
{
	SaveHandlePtr->GetUserGlobalData();
	UUserGlobalDataSaveGameObjBase* UserGlobalData = SaveHandlePtr->GetUserGlobalData();
	Map_MapName_To_MapRuntimeInfo = UserGlobalData->UserMapsRuntimeInfo;
}

FMapInfo* UMapsInfoHandler::FindMapUIInfo_ByUWorld(UWorld* InWorldPtr)
{
	auto name = FName(FPaths::GetBaseFilename(InWorldPtr->GetPackage()->FileName.ToString()));
	auto res = MapInfoDataTable->FindRow<FMapInfo>(name, TEXT("FindCurrentWorld"));
	return (res);
}
