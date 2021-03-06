// Fill out your copyright notice in the Description page of Project Settings.


#include "WorldLevelHandler/UI_Modules/UI_MapSelection/MapSelectionBaseHandler.h"

#include "Kismet/GameplayStatics.h"


const FFunctionHandlerDef UMapSelectionBaseHandler::HandlerDef(StaticClass(), {
   HandlerDependentPair("MapsInfoHandler", new FFunctionHandlerDependent(UMapsInfoHandler::StaticClass(),
		NSLOCTEXT("UMapSelectionBaseHandler", "MapsInfoHandler_Tooltip", "本依赖Handler用来获取用户的名称UID等信息用来分用户保存各种存档。"))),
   HandlerDependentPair("WidgetHandler", new FFunctionHandlerDependent(UWidgetHandler::StaticClass(),
		NSLOCTEXT("UMapSelectionBaseHandler", "WidgetHandler_Tooltip", "本依赖Handler用来获取用户的名称UID等信息用来分用户保存各种存档。"))),
});

void UMapSelectionBaseHandler::AssignDependentHandlerPtr()
{
	Super::AssignDependentHandlerPtr();
	MapsInfoHandler = dynamic_cast<UMapsInfoHandler*>(Map_Purpose_To_HandlerInstance["MapsInfoHandler"]);
	WidgetHandler = dynamic_cast<UWidgetHandler*>(Map_Purpose_To_HandlerInstance["WidgetHandler"]);
}

TArray<FName> UMapSelectionBaseHandler::GetMapIDList_CPP(UDataTable* InMapInfoDataTable)
{
	TArray<FName> RetNameList;
	if (IsValid(InMapInfoDataTable))
	{
		TArray<FName> RowNameList = InMapInfoDataTable->GetRowNames();
		for (const FName RowName : RowNameList)
		{
			const FMapUIInfo* MapUIInfo = InMapInfoDataTable->FindRow<FMapUIInfo>(RowName, __FUNCTION__);
			RetNameList.Add(MapUIInfo->MapIdentifier);
		}
	}
	return RetNameList;
}

TArray<FMapInfo> UMapSelectionBaseHandler::GetMapInfoList_Implementation(UDataTable* InMapInfoDataTable)
{
	TArray<FMapInfo> RetInfoPtrList;
	if (IsValid(InMapInfoDataTable))
	{
		TArray<FName> RowNameList = InMapInfoDataTable->GetRowNames();
		for (const FName RowName : RowNameList)
		{
			const FMapInfo* MapUIInfo = InMapInfoDataTable->FindRow<FMapInfo>(RowName, __FUNCTION__);
			RetInfoPtrList.Add(*MapUIInfo);
		}
	}
	return RetInfoPtrList;
}

void UMapSelectionBaseHandler::LoadMap_Implementation(FMapInfo InMapInfo)
{
	this->LoadMap_CPP(&InMapInfo);
}

void UMapSelectionBaseHandler::LoadMap_CPP(const FMapInfo* InMapInfo)
{
	if (!InMapInfo->MapObject.IsNull())
	{
		UGameplayStatics::OpenLevel(this, FName(InMapInfo->MapObject.GetLongPackageName()));
		WidgetHandler->ResetWidgetInfo();
		MapsInfoHandler->SetPlayingMapInfo_CPP(*InMapInfo);
	}
	else
	{
		UE_LOG(LogTemp, Display, TEXT("Function:[%s] Input parameter [InMapInfo.MapObject] is null"), ANSI_TO_TCHAR(__FUNCTION__));
	}
}
