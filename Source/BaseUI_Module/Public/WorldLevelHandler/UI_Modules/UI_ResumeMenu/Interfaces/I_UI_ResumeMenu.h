// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Widget.h"
#include "UObject/Interface.h"
#include "I_UI_ResumeMenu.generated.h"

// This class does not need to be modified.
UINTERFACE(BlueprintType)
class UI_UI_ResumeMenu : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class BASEUI_MODULE_API II_UI_ResumeMenu
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="ResumeMenu")
	void ResumeGame(UWidget* InControlledWidget);
	virtual void ResumeGame_CPP(UWidget* InControlledWidget) = 0;
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="ResumeMenu")
	void ExitToMainMenu();
	virtual void ExitToMainMenu_CPP() = 0;
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="ResumeMenu")
	void ExitToDesktop();
	virtual void ExitToDesktop_CPP() = 0;
};
