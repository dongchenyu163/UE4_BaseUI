// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "I_UI_SinglePlayerFunctions.generated.h"

// This class does not need to be modified.
UINTERFACE(Blueprintable, MinimalAPI)
class UI_UI_SinglePlayerFunctions : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class BASEUI_MODULE_API II_UI_SinglePlayerFunctions
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="MainMenu|SinglePlayer")
	void StartNewGame();
	virtual void StartNewGame_CPP() = 0;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="MainMenu|SinglePlayer")
	void ContinueFromLast();
	virtual void ContinueFromLast_CPP() = 0;
};
