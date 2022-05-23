// Fill out your copyright notice in the Description page of Project Settings.


#include "WorldLevelHandler/UI_Modules/UI_NextLevel/NextLevelBaseHandler.h"

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
	auto PlayingMapInfo = GetFrameworkGameInstance_CPP()->GetPlayingMapUIInfo_CPP();
	if (PlayingMapInfo->NextLevel_MapIdentifierList.Num() > 0)
	{
		return true;
	}
	return false;
}

void UNextLevelBaseHandler::LoadNextLevel_CPP()
{
	if (HasNextLevel_CPP())
	{
		II_GI_MenuFramework* MainGameInstance = GetFrameworkGameInstance_CPP();
		auto PlayingMapInfo = MainGameInstance->GetPlayingMapUIInfo_CPP();
		FName LoadMapIdentifier = PlayingMapInfo->NextLevel_MapIdentifierList[0];
		FMapUIInfo* LoadMapInfo = MainGameInstance->GetMapUIInfo_CPP(LoadMapIdentifier);
		auto MapLoader = MainGameInstance->GetMapSelector_CPP();
		MapLoader->LoadMap_CPP(LoadMapInfo);
	}
}
