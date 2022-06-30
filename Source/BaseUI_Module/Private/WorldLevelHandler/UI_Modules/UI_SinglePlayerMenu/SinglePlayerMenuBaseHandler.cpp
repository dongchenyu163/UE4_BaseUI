// Fill out your copyright notice in the Description page of Project Settings.


#include "WorldLevelHandler/UI_Modules/UI_SinglePlayerMenu/SinglePlayerMenuBaseHandler.h"
#include "Kismet/GameplayStatics.h"
#include "GameplayClasses/GameInstanceBase/Interfaces/I_GI_MenuFramework.h"
#include "WorldLevelHandler/UI_Modules/UI_MapSelection/MapSelectionBaseHandler.h"
#include "WorldLevelHandler/UI_Modules/UI_Save/SaveBaseHandler.h"

const FFunctionHandlerDef USinglePlayerMenuBaseHandler::HandlerDef(USinglePlayerMenuBaseHandler::StaticClass(), {
	HandlerDependentPair("MapSelector", new FFunctionHandlerDependent(UMapSelectionBaseHandler::StaticClass(),
		NSLOCTEXT("USinglePlayerMenuBaseHandler", "MapSelector_Tooltip", "主菜单需要选择以及切换地图。本依赖Handler主要读取地图的信息以及加载对应地图。"))),
	HandlerDependentPair("SaveHandler", new FFunctionHandlerDependent(USaveBaseHandler::StaticClass(),
		NSLOCTEXT("USinglePlayerMenuBaseHandler", "SaveHandler_Tooltip", "主菜单需要保存的功能；实际进行文件操作以及保存读取的Handler"))),
});

void USinglePlayerMenuBaseHandler::StartNewGame_CPP()
{
	if (IsValid(MapInfoDataTable))
 	{
 		if (MapInfoDataTable->GetRowStruct()->IsChildOf(FMapUIInfo::StaticStruct()))
 		{
 			II_GI_MenuFramework* MainMenuGameInstance = GetFrameworkGameInstance_CPP();
 			if (MainMenuGameInstance)
 			{
 				auto MapInfoList = MapSelectableObj->GetMapInfoList_CPP(MapInfoDataTable);
 				const auto MainMenuMap = MapInfoList[0]->MapObject;
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

void USinglePlayerMenuBaseHandler::AssignInterfacePtr(UObject* MatchedObjectPtr, UClass* MatchedInterfaceClassPtr)
{
	if (MatchedInterfaceClassPtr == UI_UI_MapSelectable::StaticClass())
	{
		MapSelectableObj = MatchedObjectPtr;
	}
	else if (MatchedInterfaceClassPtr == UI_UI_Savable::StaticClass())
	{
		SaveBaseHandler = MatchedObjectPtr;
	}
}

void USinglePlayerMenuBaseHandler::AssignDependentHandlerPtr()
{
	Super::AssignDependentHandlerPtr();
	MapSelectableObj = Map_Purpose_To_HandlerInstance["MapSelector"];
	SaveBaseHandler = Map_Purpose_To_HandlerInstance["SaveHandler"];
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

