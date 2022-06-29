// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FunctionHandlerBase.h"
#include "GameplayClasses/GameInstanceBase/Interfaces/I_GI_MenuFramework.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/Object.h"
#include "BaseUI_Static.generated.h"

/**
 * 
 */
UCLASS()
class BASEUI_MODULE_API UBaseUI_Static : public UObject
{
	GENERATED_BODY()
	
public:
// 完成K2Node
	UFUNCTION(BlueprintCallable, Category="DongBaseUI")
	static bool FindHandlerByName(FName InHandlerName, UFunctionHandlerBase*& OutFoundedHandler)
	{
		const UWorld* PlayingWorld = GEngine->GetCurrentPlayWorld();
		if (PlayingWorld)
		{
			UE_LOG(LogTemp, Display, TEXT("Function:[%s] Find handler."), ANSI_TO_TCHAR(__FUNCTION__));
			II_GI_MenuFramework* GameInstancePtr = PlayingWorld->GetGameInstanceChecked<II_GI_MenuFramework>();
			if (GameInstancePtr)
			{
				OutFoundedHandler = GameInstancePtr->FindHandler_ByName_CPP(InHandlerName);
				return true;
			}
		}
		UE_LOG(LogTemp, Error, TEXT("Function:[%s] This function and node MUST be called at the RUNTIME!!!"), ANSI_TO_TCHAR(__FUNCTION__));
		return false;
	}
};
