// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TestUIBase.h"
#include "Components/Widget.h"
#include "WorldLevelHandler/UI_Modules/UI_MapSelection/Interfaces/I_UI_MapSelectable.h"
#include "WorldLevelHandler/UI_Modules/UI_ResumeMenu/Interfaces/I_UI_ResumeMenu.h"
#include "WorldLevelHandler/UI_Modules/UI_SinglePlayerMenu/Interfaces/I_UI_SinglePlayerFunctions.h"
#include "UObject/Object.h"
#include "MainMenuWidget.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class BASEUI_MODULE_API UMainMenuWidget : public UTestUIBase, /*public II_UI_MapSelectable,*/ public II_UI_SinglePlayerFunctions, public II_UI_ResumeMenu
{
	GENERATED_BODY()

protected:
	virtual ~UMainMenuWidget() override {}
public:
	UFUNCTION(BlueprintCallable, Category="")
	virtual void StartNewGame_Implementation() override;
	UFUNCTION(BlueprintCallable, Category="")
	virtual void ContinueFromLast_Implementation() override;
	UFUNCTION(BlueprintCallable, Category="")
	virtual void ResumeGame_Implementation(UWidget* InControlledWidget) override {}
	UFUNCTION(BlueprintCallable, Category="")
	virtual void ExitToMainMenu_Implementation() override {}
	UFUNCTION(BlueprintCallable, Category="")
	virtual void ExitToDesktop_Implementation() override;

	virtual void ResumeGame_CPP(UWidget* InControlledWidget) override;
	virtual void ExitToMainMenu_CPP() override;
	virtual void ExitToDesktop_CPP() override;
	virtual void StartNewGame_CPP() override;
	virtual void ContinueFromLast_CPP() override;
};
