// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayClasses/GameInstanceBase/Interfaces/I_GI_MenuFramework.h"
#include "UObject/Object.h"
#include "UIHandlerBase.generated.h"

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class BASEUI_MODULE_API UUIHandlerBase : public UObject
{
	GENERATED_BODY()
public:
	UUIHandlerBase()
	{
		II_GI_MenuFramework* MainMenuGameInstance(Cast<II_GI_MenuFramework>(GetOuter()));
		if (MainMenuGameInstance)
		{
			FrameworkGameInstance_CPP = MainMenuGameInstance;
		}
	}

private:
	void Init(II_GI_MenuFramework* InGameInstancePtr) { FrameworkGameInstance_CPP = InGameInstancePtr; } 

protected:
	UFUNCTION(BlueprintCallable, Category="DongMenuFramework")
	TScriptInterface<II_GI_MenuFramework> GetFrameworkGameInstance() { return (UObject*)GetFrameworkGameInstance_CPP(); }
	II_GI_MenuFramework* GetFrameworkGameInstance_CPP();

protected:
	// UPROPERTY(BlueprintReadOnly, Category="GameInstance")
	// TScriptInterface<II_GI_MenuFramework> FrameworkGameInstance;
	II_GI_MenuFramework* FrameworkGameInstance_CPP;

};
