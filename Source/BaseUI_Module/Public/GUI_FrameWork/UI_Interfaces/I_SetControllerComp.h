// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GUI_FrameWork/ControllerComponent/AC_CTRL_MenuBase.h"
#include "UObject/Interface.h"
#include "I_SetControllerComp.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnComponentSwitchToNewWidget);

// This class does not need to be modified.
UINTERFACE(BlueprintType, MinimalAPI)
class UI_SetControllerComp : public UInterface
{
	GENERATED_BODY()
};

/**
 * 主要包含【Controller的Component】和【UI】之间的交互函数。应当由【UI】继承。
 * 1. 设置【Controller的Component】的对象
 * 		继承的类可以设置当前UMG的ControllerComponent(UAC_CTRL_MenuBase对象)，
 * 		方便UI UMG对象调用ControllerComponent的函数
 * 		主要是SwitchWidgetTo和CreateNewWidget这两个函数，
 *			SwitchWidgetTo是在不同的UMG之间切换
 *			CreateNewWidget是创建新的UI UObject
 * 2. 【Controller的Component】获取UI的【切换Widget】的委托对象
 *		实现Component可以在【SwitchWidgetTo】的时候通知UI。
 */
class BASEUI_MODULE_API II_SetControllerComp
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="GUI Framework")
	void SetUIObjectComponent(UAC_CTRL_MenuBase* InNewComponentObj);
	virtual void SetUIObjectComponent_CPP(UAC_CTRL_MenuBase* InNewComponentObj) = 0;

	// UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="", meta=(Tooltip=""))
	// FOnComponentSwitchToNewWidget GetOnComponentSwitchToNewWidgetDelegate();
	virtual FOnComponentSwitchToNewWidget* GetOnComponentSwitchToNewWidgetDelegate_CPP() = 0;

};
