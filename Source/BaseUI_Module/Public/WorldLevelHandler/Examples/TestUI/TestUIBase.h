// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/WidgetTree.h"
#include "Components/BackgroundBlur.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "GameplayClasses/ControllerComponent/AC_CTRL_MenuBase.h"
#include "GameplayClasses/GameInstanceBase/Interfaces/I_GI_MenuFramework.h"
#include "WorldLevelHandler/UI_Interfaces/I_SetControllerComp.h"
#include "UObject/Object.h"
#include "TestUIBase.generated.h"

/**
 * 提供功能：
 * 	和Controller的组件建立连接：
 * 		结果：包含该组件【MenuComponent】的引用
 * 		方便在UI中切换至其他UI：通过调用
 * 	其他大类：
 *		按需背景虚化：仅在游戏进行的时候，暂停界面的背景会虚化；主菜单等非游戏界面时候背景不会虚化。
 *					需要在蓝图子类创建Blur控件，并变为变量，将改变量赋值到
 *					【InGameMenu_BackgroundBlur】成员变量中。
 *					需要和MapInfo中的【bIsGameWorld】配合使用。
 */
UCLASS(BlueprintType)
class BASEUI_MODULE_API UTestUIBase : public UUserWidget, public II_SetControllerComp
{
	GENERATED_BODY()
protected:
	// virtual void OnWidgetRebuilt() override
	// {
	// }
	// virtual void NativePreConstruct() override
	// {
	//	
	// 	OnComponentSwitchToNewWidget.AddUniqueDynamic( this, &UTestUIBase::Handle_OnComponentSwitchToNewWidget);
	// 	
	// 	InGameMenu_BackgroundBlur = WidgetTree->ConstructWidget<UBackgroundBlur>(UBackgroundBlur::StaticClass(), "InGame_BackgroundBlur");
	// 	UCanvasPanel* _RootWidget = Cast<UCanvasPanel>(GetRootWidget());
	// 	if (!_RootWidget)
	// 	{
	// 		// this->Root
	// 		WidgetTree->RootWidget = WidgetTree->ConstructWidget<UCanvasPanel>();
	// 		_RootWidget = Cast<UCanvasPanel>(GetRootWidget());
	// 	}
	//
	// 	auto Old_Children = _RootWidget->GetAllChildren();
	// 	if (Old_Children.Num() > 0)
	// 	{
	// 		if (Old_Children.Num() == 1)
	// 		{
	// 			InGameMenu_BackgroundBlur->AddChild(Old_Children[0]);
	// 		}
	// 		else
	// 		{
	// 			auto NewContainer = WidgetTree->ConstructWidget<UCanvasPanel>();
	// 			InGameMenu_BackgroundBlur->AddChild(NewContainer);
	// 			for (auto OldWidget : Old_Children)
	// 			{
	// 				NewContainer->AddChild(OldWidget);
	// 			}
	// 			_RootWidget->ClearChildren();
	// 		}
	// 	}
	//
	// 	// _RootWidget->Slot->
	// 	auto slot = Cast<UCanvasPanelSlot>(_RootWidget->AddChildToCanvas(InGameMenu_BackgroundBlur));
	// 	slot->SetAnchors(FAnchors(0, 0, 1, 1));
	// 	InGameMenu_BackgroundBlur->BlurStrength = 10;
	// 	InGameMenu_BackgroundBlur->BlurRadius = 5;
	// }
	virtual void NativeConstruct() override
	{
		Super::NativeConstruct();
		FOnAnyWorldBeginPlay* OnAnyWorldBeginPlay = GetFrameworkGameInstance_CPP()->GetOnAnyWorldBeginPlayDelegate_CPP();
		OnAnyWorldBeginPlay->AddUniqueDynamic( this, &UTestUIBase::Handle_OnAnyWorldBeginPlay );
		// OnAnyWorldBeginPlay->AddUniqueDynamic
		// InGameMenu_BackgroundBlur = WidgetTree->ConstructWidget<UBackgroundBlur>(UBackgroundBlur::StaticClass(), "InGame_BackgroundBlur");
		// UPanelWidget* aaa = Cast<UPanelWidget>(GetRootWidget());
		// aaa->AddChild(InGameMenu_BackgroundBlur);
		
		// 【OnComponentSwitchToNewWidget】委托通过接口传递到Component，让Comp能够在切换Widget的时候可以通知Widget被调用了。
		OnComponentSwitchToNewWidget.AddUniqueDynamic( this, &UTestUIBase::Handle_OnComponentSwitchToNewWidget);
	}

	// virtual void NativeOnInitialized() override
	// {
	// 	// 不知道什么时候会被调用
	// }

protected:
	UFUNCTION()
	/**
	 * 该函数被动态委托挂载，故需要UFUNCTION宏标记
	 */
	void Handle_OnAnyWorldBeginPlay() { OnBeginPlay(); }
	
	UFUNCTION(BlueprintNativeEvent, Category="")
	void Handle_OnComponentSwitchToNewWidget();
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="", meta=(ToolTip="应当在C++中和GameInstance的委托关联在一起"))
	void OnBeginPlay();
	
	UFUNCTION(BlueprintCallable, Category="DongMenuFramework")
	TScriptInterface<II_GI_MenuFramework> GetFrameworkGameInstance();
	
	II_GI_MenuFramework* GetFrameworkGameInstance_CPP();
	
	virtual void SetUIObjectComponent_CPP(UAC_CTRL_MenuBase* InNewComponentObj) override { MenuComponent = InNewComponentObj; }
	virtual void SetUIObjectComponent_Implementation(UAC_CTRL_MenuBase* InNewComponentObj) override { SetUIObjectComponent_CPP(InNewComponentObj); }
public:
	virtual FOnComponentSwitchToNewWidget* GetOnComponentSwitchToNewWidgetDelegate_CPP() override { return &OnComponentSwitchToNewWidget; }
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="")
	UAC_CTRL_MenuBase* MenuComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="")
	UBackgroundBlur* InGameMenu_BackgroundBlur;

	UPROPERTY(BlueprintAssignable, Category="")
	// 【OnComponentSwitchToNewWidget】委托通过接口传递到Component，让Comp能够在切换Widget的时候可以通知Widget被调用了。
	FOnComponentSwitchToNewWidget OnComponentSwitchToNewWidget;
};
