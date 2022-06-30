// Fill out your copyright notice in the Description page of Project Settings.


#include "WorldLevelHandler/Examples/TestUI/MapSelectionWidget.h"

TArray<FName> UMapSelectionWidget::GetMapIDList_Implementation(UDataTable* InMapInfoDataTable)
{
	II_GI_MenuFramework* MainMenuGameInstance = GetFrameworkGameInstance_CPP();
	if (MainMenuGameInstance)
	{
		return MainMenuGameInstance->GetMapSelector_CPP()->GetMapIDList_CPP(InMapInfoDataTable);
	}
	return TArray<FName>();
}

TArray<FMapUIInfo> UMapSelectionWidget::GetMapInfoList_Copy_Implementation(UDataTable* InMapInfoDataTable)
{
	II_GI_MenuFramework* MainMenuGameInstance = GetFrameworkGameInstance_CPP();
	if (MainMenuGameInstance)
	{
		return MainMenuGameInstance->GetMapSelector_CPP()->GetMapInfoList_Copy_Implementation(InMapInfoDataTable);
	}
	return TArray<FMapUIInfo>();
}

void UMapSelectionWidget::LoadMap_CPP(const FMapInfo* InMapInfo)
{
	II_GI_MenuFramework* MainMenuGameInstance = GetFrameworkGameInstance_CPP();
	if (MainMenuGameInstance)
	{
		return MainMenuGameInstance->GetMapSelector_CPP()->LoadMap_CPP(InMapInfo);
	}
}

void UMapSelectionWidget::LoadMap_Implementation(FMapUIInfo InMapInfo)
{
	LoadMap_CPP(&InMapInfo);
}
