// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TestUIBase.h"
#include "UObject/Object.h"
#include "WinGameWidget.generated.h"

/**
 * 
 */
UCLASS()
class BASEUI_MODULE_API UWinGameWidget : public UTestUIBase, public II_UI_NextLevel, public II_UI_ResumeMenu
{
	GENERATED_BODY()

public:
	virtual bool HasNextLevel_CPP() override { return false; }  // 用不到该函数。
	virtual void LoadNextLevel_CPP() override;
	virtual void ResumeGame_CPP(UWidget* InControlledWidget) override {}  // 用不到该函数。
	virtual void ExitToMainMenu_CPP() override;
	virtual void ExitToDesktop_CPP() override;
};
