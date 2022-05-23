// Fill out your copyright notice in the Description page of Project Settings.


#include "GUI_FrameWork/UI_Modules/UI_MapSelection/MapSelectionBaseHandler.h"

#include "Kismet/GameplayStatics.h"


TArray<FName> UMapSelectionBaseHandler::GetMapIDList_Implementation(UDataTable* InMapInfoDataTable)
{
	TArray<FName> RetNameList;
	if (IsValid(InMapInfoDataTable))
	{
		TArray<FName> RowNameList = InMapInfoDataTable->GetRowNames();
		for (FName RowName : RowNameList)
		{
			FMapUIInfo* MapUIInfo = InMapInfoDataTable->FindRow<FMapUIInfo>(RowName, __FUNCTION__);
			RetNameList.Add(MapUIInfo->MapIdentifier);
		}
	}
	return RetNameList;
}

TArray<FMapUIInfo> UMapSelectionBaseHandler::GetMapInfoList_Copy_Implementation(UDataTable* InMapInfoDataTable)
{
	TArray<FMapUIInfo> RetInfoPtrList;
	if (IsValid(InMapInfoDataTable))
	{
		TArray<FName> RowNameList = InMapInfoDataTable->GetRowNames();
		for (FName RowName : RowNameList)
		{
			FMapUIInfo* MapUIInfo = InMapInfoDataTable->FindRow<FMapUIInfo>(RowName, __FUNCTION__);
			RetInfoPtrList.Add(*MapUIInfo);
		}
	}
	return RetInfoPtrList;
}

void UMapSelectionBaseHandler::LoadMap_Implementation(FMapUIInfo InMapInfo)
{
	this->LoadMap_CPP(&InMapInfo);
}

void UMapSelectionBaseHandler::LoadMap_CPP(const FMapUIInfo* InMapInfo)
{
	if (!InMapInfo->MapObject.IsNull())
	{
		UGameplayStatics::OpenLevel(this, FName(InMapInfo->MapObject.GetLongPackageName()));
		GetFrameworkGameInstance_CPP()->ResetWidgetInfo_CPP();
		GetFrameworkGameInstance_CPP()->SetPlayingMapUIInfo_CPP(InMapInfo);
	}
	else
	{
		UE_LOG(LogTemp, Display, TEXT("Function:[%s] Input parameter [InMapInfo.MapObject] is null"), ANSI_TO_TCHAR(__FUNCTION__));
	}
}
