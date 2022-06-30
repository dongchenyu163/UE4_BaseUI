// Fill out your copyright notice in the Description page of Project Settings.


#include "WorldLevelHandler/Examples/TestUI/MapSelectionWidget.h"

#include "BaseClassesAndTypes/BaseUI_Static.h"
#include "WorldLevelHandler/NormalHandlers/MapsInfoHandler/MapsInfoHandler.h"
#include "WorldLevelHandler/UI_Modules/UI_MapSelection/MapSelectionBaseHandler.h"

TArray<FName> UMapSelectionWidget::GetMapIDList_CPP(UDataTable* InMapInfoDataTable)
{
	UFunctionHandlerBase* FoundHandler;
	if (UBaseUI_Static::FindHandlerByName("MapSelector", FoundHandler))
	{
		UMapSelectionBaseHandler* MapSelectionHandlerPtr = dynamic_cast<UMapSelectionBaseHandler*>(FoundHandler);
		return MapSelectionHandlerPtr->GetMapIDList_CPP(InMapInfoDataTable);
	}
	else
	{
		UE_LOG(LogTemp, Display, TEXT("Function:[%s] No handler named [MapSelector]!!!"), ANSI_TO_TCHAR(__FUNCTION__));
		return TArray<FName>();
	}
}

void UMapSelectionWidget::LoadMap_CPP(const FMapInfo* InMapInfo)
{
	UFunctionHandlerBase* FoundHandler;
	if (UBaseUI_Static::FindHandlerByName("MapSelector", FoundHandler))
	{
		UMapSelectionBaseHandler* MapSelectionHandlerPtr = dynamic_cast<UMapSelectionBaseHandler*>(FoundHandler);
		return MapSelectionHandlerPtr->LoadMap_CPP(InMapInfo);
	}
	else
	{
		UE_LOG(LogTemp, Display, TEXT("Function:[%s] No handler named [MapSelector]!!!"), ANSI_TO_TCHAR(__FUNCTION__));
		return;
	}
}

void UMapSelectionWidget::LoadMap_Implementation(FMapInfo InMapInfo)
{
	LoadMap_CPP(&InMapInfo);
}
