// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GUI_FrameWork/UI_Modules/BaseHandler/MenuBaseHandler.h"
#include "Components/Widget.h"
#include "Interfaces/I_UI_ResumeMenu.h"
#include "UObject/Object.h"
#include "ResumeMenuBaseHandler.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class BASEUI_MODULE_API UResumeMenuBaseHandler : public UMenuBaseHandler, public II_UI_ResumeMenu
{
	GENERATED_BODY()
public:
	UResumeMenuBaseHandler() {}
	UResumeMenuBaseHandler(UWidget* InControlledResumeMenu) { ControlledResumeMenu = InControlledResumeMenu; }
protected:
	virtual ~UResumeMenuBaseHandler() override {}
public:
	virtual void ResumeGame_Implementation(UWidget* InControlledWidget) override;
	virtual void ExitToMainMenu_Implementation() override;
	virtual void ExitToDesktop_Implementation() override;
	
	virtual void ResumeGame_CPP(UWidget* InControlledWidget) override;
	virtual void ExitToMainMenu_CPP() override;
	virtual void ExitToDesktop_CPP() override;

	UFUNCTION(BlueprintCallable, Category="ResumeMenu")
	void SetControlledMenu(UWidget* InControlledResumeMenu);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="ResumeMenu", meta=(Tooltip="被控制的本体"))
	UWidget* ControlledResumeMenu = nullptr;

};
