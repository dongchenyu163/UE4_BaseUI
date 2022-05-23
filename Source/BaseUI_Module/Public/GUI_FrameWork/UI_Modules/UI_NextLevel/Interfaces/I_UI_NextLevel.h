// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "I_UI_NextLevel.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UI_UI_NextLevel : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class BASEUI_MODULE_API II_UI_NextLevel
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="NextLevel")
	bool HasNextLevel();
	virtual bool HasNextLevel_CPP() = 0;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="NextLevel")
	void LoadNextLevel();
	virtual void LoadNextLevel_CPP() = 0;
};
