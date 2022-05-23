// Fill out your copyright notice in the Description page of Project Settings.


#include "WorldLevelHandler/UI_Modules/UI_MapSelection/Interfaces/I_UI_MapSelectable.h"

#include "Kismet/GameplayStatics.h"

// Add default functionality here for any IUI_MapSelectable functions that are not pure virtual.

TArray<FMapUIInfo*> II_UI_MapSelectable::GetMapInfoList_CPP(UDataTable* InMapInfoDataTable)
{
	TArray<FMapUIInfo*> RetInfoPtrList;
	if (IsValid(InMapInfoDataTable))
	{
		TArray<FName> RowNameList = InMapInfoDataTable->GetRowNames();
		for (FName RowName : RowNameList)
		{
			FMapUIInfo* MapUIInfo = InMapInfoDataTable->FindRow<FMapUIInfo>(RowName, __FUNCTION__);
			RetInfoPtrList.Add(MapUIInfo);
		}
	}
	return RetInfoPtrList;
}


