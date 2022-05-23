// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GUI_FrameWork/UI_Modules/BaseHandler/MenuBaseHandler.h"
#include "UObject/Object.h"
#include "MainMenuGameInstanceConfig.generated.h"

/**
 * 
 */
UCLASS()
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
	
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override
	{
		
	}
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="UI Handler Settings", meta=(DisplayName="UI handler use in game", ToolTip="程序中使用FName来查找并引用对应模块"))
	TMap<FName, TSubclassOf<UMenuBaseHandler>> UsingHandlerClassCollection;

	
};
