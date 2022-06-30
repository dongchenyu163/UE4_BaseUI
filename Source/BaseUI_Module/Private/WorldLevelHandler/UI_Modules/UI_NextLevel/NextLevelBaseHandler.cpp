// Fill out your copyright notice in the Description page of Project Settings.


#include "WorldLevelHandler/UI_Modules/UI_NextLevel/NextLevelBaseHandler.h"

const FFunctionHandlerDef UNextLevelBaseHandler::HandlerDef(StaticClass(), {
   HandlerDependentPair("MapsInfoHandler", new FFunctionHandlerDependent(UMapsInfoHandler::StaticClass(),
	   NSLOCTEXT("UNextLevelBaseHandler", "MapsInfoHandler_Tooltip", "本依赖Handler用来获取用户的名称UID等信息用来分用户保存各种存档。"))),
	HandlerDependentPair("MapSelectionHandler", new FFunctionHandlerDependent(UMapSelectionBaseHandler::StaticClass(),
	   NSLOCTEXT("UNextLevelBaseHandler", "MapSelectionHandler_Tooltip", "本依赖Handler用来获取用户的名称UID等信息用来分用户保存各种存档。"))),
});

void UNextLevelBaseHandler::AssignDependentHandlerPtr()
{
	Super::AssignDependentHandlerPtr();
	MapsInfoHandler = dynamic_cast<UMapsInfoHandler*>(Map_Purpose_To_HandlerInstance["MapsInfoHandler"]);
	MapSelectionHandler = dynamic_cast<UMapSelectionBaseHandler*>(Map_Purpose_To_HandlerInstance["MapSelectionHandler"]);
}

bool UNextLevelBaseHandler::HasNextLevel_Implementation()
{
	return HasNextLevel_CPP();
}

void UNextLevelBaseHandler::LoadNextLevel_Implementation()
{
	LoadNextLevel_CPP();
}

bool UNextLevelBaseHandler::HasNextLevel_CPP()
{
	auto PlayingMapInfo = MapsInfoHandler->GetPlayingMapInfo();
	if (PlayingMapInfo.NextLevel_MapIdentifierList.Num() > 0)
	{
		return true;
	}
	return false;
}

void UNextLevelBaseHandler::LoadNextLevel_CPP()
{
	if (HasNextLevel_CPP())
	{
		auto PlayingMapInfo = MapsInfoHandler->GetPlayingMapInfo();
		FName LoadMapIdentifier = PlayingMapInfo.NextLevel_MapIdentifierList[0];
		FMapInfo LoadMapInfo;
		MapsInfoHandler->GetMapInfo(LoadMapIdentifier, LoadMapInfo);
		MapSelectionHandler->LoadMap_CPP(LoadMapInfo);
	}
}
