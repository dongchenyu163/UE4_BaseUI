// Fill out your copyright notice in the Description page of Project Settings.


#include "WorldLevelHandler/UI_Modules/UI_SinglePlayerMenu/SinglePlayerMenuBaseHandler.h"
#include "Kismet/GameplayStatics.h"
#include "GameplayClasses/GameInstanceBase/Interfaces/I_GI_MenuFramework.h"

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

TSet<UClass*> USinglePlayerMenuBaseHandler::GetDependenceHandlerInterfaceCollection()
{
	static const TSet<UClass*> DependenceHandlerClassCollection = {
		UI_UI_MapSelectable::StaticClass(),
		UI_UI_Savable::StaticClass()
	};
	return DependenceHandlerClassCollection;
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

