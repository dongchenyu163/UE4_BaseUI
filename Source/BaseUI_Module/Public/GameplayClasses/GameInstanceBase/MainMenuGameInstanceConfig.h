// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ClassesAndStructs/FunctionHandlerInfo.h"
#include "WorldLevelHandler/UI_Modules/BaseHandler/UIHandlerBase.h"
#include "UObject/Object.h"
#include "WorldLevelHandler/UI_Modules/UI_MapSelection/MapSelectionBaseHandler.h"
#include "WorldLevelHandler/UI_Modules/UI_NextLevel/NextLevelBaseHandler.h"
#include "WorldLevelHandler/UI_Modules/UI_ResumeMenu/ResumeMenuBaseHandler.h"
#include "WorldLevelHandler/UI_Modules/UI_Save/SaveBaseHandler.h"
#include "WorldLevelHandler/UI_Modules/UI_SinglePlayerMenu/SinglePlayerMenuBaseHandler.h"
#include "MainMenuGameInstanceConfig.generated.h"

/**
 * 
 */
UCLASS(Config="MainMenuGameInstanceConfig")
class BASEUI_MODULE_API UMainMenuGameInstanceConfig : public UDeveloperSettings
{
	GENERATED_BODY()
public:
	// 指定本Config显示在【编辑器设置】还是【工程设置】中；分别对应两种选择【Editor】和【Project】
	virtual FName GetContainerName() const override { return TEXT("Project"); }
	// Setting的左侧栏的大分类
	virtual FName GetCategoryName() const override { return TEXT("Dong UI Framework Modules Config"); }
	// 内部唯一标识符
	virtual FName GetSectionName() const override { return TEXT("Dong UI Framework Modules GameInstance Config"); }
	// Setting的左侧栏的子分类
	virtual FText GetSectionText() const override { return NSLOCTEXT("MainMenuGameInstanceConfig", "SectionText", "Game Instance Config"); }
	// Setting的左侧栏的悬停提示文字
	virtual FText GetSectionDescription() const override { return NSLOCTEXT("MainMenuGameInstanceConfig", "SectionDescriptionText", "配置GameInstance"); }

	UMainMenuGameInstanceConfig()
	{
		// UI Handlers
		HandlerClassDict.Add(FName("MapSelector"), FFunctionHandlerInfo(UMapSelectionBaseHandler::StaticClass()));
		HandlerClassDict.Add(FName("NextLevelHandler"), FFunctionHandlerInfo(UNextLevelBaseHandler::StaticClass()));
		HandlerClassDict.Add(FName("ResumeMenuHandle"), FFunctionHandlerInfo(UResumeMenuBaseHandler::StaticClass()));
		HandlerClassDict.Add(FName("SaveHandler"), FFunctionHandlerInfo(USaveBaseHandler::StaticClass(), { "LowLevelSaveHandler", "UserManager" }));
		HandlerClassDict.Add(FName("SinglePlayerMenuHandle"), FFunctionHandlerInfo(USinglePlayerMenuBaseHandler::StaticClass(), { "MapSelector", "SaveHandler" }));

		// Low level Handlers
		HandlerClassDict.Add(FName("LowLevelSaveHandler"), FFunctionHandlerInfo(USinglePlayerMenuBaseHandler::StaticClass(), { "UserManager" }));
		HandlerClassDict.Add(FName("UserManager"), FFunctionHandlerInfo(USinglePlayerMenuBaseHandler::StaticClass()));

		ReloadConfig();
	}
	virtual void PostReloadConfig(FProperty* PropertyThatWasLoaded) override
	{
		TArray<FName> HandlerNameList;
		HandlerClassDict.GetKeys(HandlerNameList);
		UReflectionOperations::SetNewOptionsToEnum("EFunctionHandlerNames_BP", HandlerNameList);
	}
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override
	{
		TArray<FName> HandlerNameList;
		HandlerClassDict.GetKeys(HandlerNameList);
		UReflectionOperations::SetNewOptionsToEnum("EFunctionHandlerNames_BP", HandlerNameList);
	}

	static UMainMenuGameInstanceConfig* Get() { return GetMutableDefault<UMainMenuGameInstanceConfig>(); }
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="UI Handler Settings", meta=(DisplayName="Handler use in game", ToolTip="程序中使用FName来查找并引用对应模块"))
	TMap<FName, FFunctionHandlerInfo> HandlerClassDict;

	
};
