// Fill out your copyright notice in the Description page of Project Settings.

#include "GameplayClasses/GameInstanceBase/MainMenuGameInstanceConfig.h"
#include "WorldLevelHandler/UI_Modules/BaseHandler/UIHandlerBase.h"
#include "WorldLevelHandler/UI_Modules/UI_MapSelection/MapSelectionBaseHandler.h"
#include "WorldLevelHandler/UI_Modules/UI_NextLevel/NextLevelBaseHandler.h"
#include "WorldLevelHandler/UI_Modules/UI_ResumeMenu/ResumeMenuBaseHandler.h"
#include "WorldLevelHandler/UI_Modules/UI_Save/SaveBaseHandler.h"
#include "WorldLevelHandler/UI_Modules/UI_SinglePlayerMenu/SinglePlayerMenuBaseHandler.h"


UMainMenuGameInstanceConfig::UMainMenuGameInstanceConfig(): Super()
{
	// UI Handlers
	HandlerClassDict.Add(FName("LowLevelSaveHandler"),
         FFunctionHandlerInfo(USavingBaseHandler::StaticClass(),
              TMap<FString, FName>(
                  {
                      TPair<FString, FName>("UserManager", "UserManager"),
                  }
              )
         )
	);
	
	HandlerClassDict.Add(FName("UserManager"), FFunctionHandlerInfo(UUserManagerBase::StaticClass()));

	HandlerClassDict.Add(FName("MapsInfoHandler"),
		 FFunctionHandlerInfo(UMapsInfoHandler::StaticClass(),
			  TMap<FString, FName>(
				  {
					  TPair<FString, FName>("SaveHandler", "SaveHandler"),
					  TPair<FString, FName>("LowLevelSaveHandler", "LowLevelSaveHandler"),
					  TPair<FString, FName>("UserManager", "UserManager"),
				  }
			  )
		 )
	);
	
	HandlerClassDict.Add(FName("WidgetHandler"), FFunctionHandlerInfo(UWidgetHandler::StaticClass()));

	HandlerClassDict.Add(FName("MapSelector"),
		 FFunctionHandlerInfo(UMapSelectionBaseHandler::StaticClass(),
			  TMap<FString, FName>(
				  {
					  TPair<FString, FName>("MapsInfoHandler", "MapsInfoHandler"),
					  TPair<FString, FName>("WidgetHandler", "WidgetHandler"),
				  }
			  )
		 )
	);

	HandlerClassDict.Add(FName("NextLevelHandler"),
		 FFunctionHandlerInfo(UNextLevelBaseHandler::StaticClass(),
			  TMap<FString, FName>(
				  {
					  TPair<FString, FName>("MapsInfoHandler", "MapsInfoHandler"),
					  TPair<FString, FName>("MapSelectionHandler", "MapSelector"),
				  }
			  )
		 )
	);
	
	HandlerClassDict.Add(FName("ResumeMenuHandle"),
		 FFunctionHandlerInfo(UResumeMenuBaseHandler::StaticClass(),
			  TMap<FString, FName>(
				  {
					  TPair<FString, FName>("MapsInfoHandler", "MapsInfoHandler"),
				  }
			  )
		 )
	);

	HandlerClassDict.Add(FName("SaveHandler"),
		FFunctionHandlerInfo(USaveBaseHandler::StaticClass(),
			TMap<FString, FName>({
			  TPair<FString, FName>("LowLevelSaveHandler", "LowLevelSaveHandler"),
			  TPair<FString, FName>("UserManager", "UserManager"),
			  TPair<FString, FName>("MapsInfoHandler", "MapsInfoHandler"),
			  TPair<FString, FName>("WidgetHandler", "WidgetHandler"),
			  TPair<FString, FName>("MapSelectHandler", "MapSelector"),
			})
		));

	HandlerClassDict.Add(FName("SinglePlayerMenuHandle"),
         FFunctionHandlerInfo(USinglePlayerMenuBaseHandler::StaticClass(),
              TMap<FString, FName>({
                  TPair<FString, FName>("MapSelector", "MapSelector"),
                  TPair<FString, FName>("SaveHandler", "SaveHandler"),
              })
         ));
	ReloadConfig();
}

