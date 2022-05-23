// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UObject/Object.h"
#include "USaveSystemLoadingScreenBase.generated.h"

/**
 * 
 */
UCLASS(Abstract, BlueprintType, Blueprintable, meta=(ToolTip="抽象类，用于【保存中/加载中 界面】UI控件的基类，本基类主要用于方便选择子类的时候筛选"))
class BASEUI_MODULE_API UUSaveSystemLoadingScreenBase : public UUserWidget
{
	GENERATED_BODY()

public:
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Save Load System|Loading Screen", meta=(ToolTip="用户点下【确认加载存档】的瞬间被调用，也等价于【开始进行存档】"))
	/**
	 * @brief 用户点下【确认存储/加载存档】的瞬间（等价于【开始进行存档】）被调用
	 * @param InScreenType 可以给任意字符串，不同字符串可代表不同的【加载画面】。方便不同情况下显示不同的【保存中/加载中 界面】。
	 */
	void OnStartingLoadingScreen(const FString& InScreenType = TEXT("Default"));
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Save Load System|Loading Screen", meta=(ToolTip="加载进度被更新的时候被调用。方便显示进度条。"))
	void OnUpdateProgress();
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Save Load System|Loading Screen", meta=(ToolTip="所有加载全部完成，World已经变成了可以游玩的状态的时候被调用。"))
	void OnLoadingComplete();
};
