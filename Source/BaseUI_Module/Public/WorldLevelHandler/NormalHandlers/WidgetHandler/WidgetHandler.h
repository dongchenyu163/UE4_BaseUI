// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "WorldLevelHandler/WorldLevelHandlerBase/WorldLevelHandlerBase.h"
#include "WidgetHandler.generated.h"

/**
 * 
 */
UCLASS()
class BASEUI_MODULE_API UWidgetHandler : public UWorldLevelHandlerBase
{
	GENERATED_BODY()

public:
	virtual EFunctionHandlerType GetHandlerType() override { return EFunctionHandlerType::WorldHandler; }
	virtual void OnStart() override;
	void ResetWidgetInfo();
};
