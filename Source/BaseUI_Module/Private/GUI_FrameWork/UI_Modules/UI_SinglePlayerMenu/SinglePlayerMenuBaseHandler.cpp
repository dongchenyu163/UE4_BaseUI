// Fill out your copyright notice in the Description page of Project Settings.


#include "GUI_FrameWork/UI_Modules/UI_SinglePlayerMenu/SinglePlayerMenuBaseHandler.h"
#include "Kismet/GameplayStatics.h"
#include "GUI_FrameWork/GameInstanceBase/Interfaces/I_GI_MenuFramework.h"

void USinglePlayerMenuBaseHandler::StartNewGame_CPP()
{
	if (IsValid(MapInfoDataTable))
 	{
 		if (MapInfoDataTable->GetRowStruct()->IsChildOf(FMapUIInfo::StaticStruct()))
 		{
 			II_GI_MenuFramework* MainMenuGameInstance = GetFrameworkGameInstance_CPP();
 			if (MainMenuGameInstance)
 			{
 				auto MapInfoList = MainMenuGameInstance->GetMapSelector_CPP()->GetMapInfoList_CPP(MapInfoDataTable);
 				auto MainMenuMap = MapInfoList[0]->MapObject;
 				FString MapName = MainMenuMap->GetName();
 				UGameplayStatics::OpenLevel(this, FName(MainMenuMap.GetLongPackageName()));
 			}
 		}
 		else
 		{
 			UE_LOG(LogTemp, Display, TEXT("Function:[%s] Row structure [%s] of Selected DataTable is not dirived from [FMapUIInfo]"), ANSI_TO_TCHAR(__FUNCTION__), *MapInfoDataTable->GetRowStruct()->GetStructCPPName());
 		}
 	}
}

void USinglePlayerMenuBaseHandler::StartNewGame_Implementation()
{
	StartNewGame_CPP();
}

void USinglePlayerMenuBaseHandler::ContinueFromLast_CPP()
{
}

void USinglePlayerMenuBaseHandler::ContinueFromLast_Implementation()
{
	ContinueFromLast_CPP();
}

