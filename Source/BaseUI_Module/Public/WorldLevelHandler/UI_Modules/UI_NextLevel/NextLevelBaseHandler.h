// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WorldLevelHandler/UI_Modules/BaseHandler/UIHandlerBase.h"
#include "Interfaces/I_UI_NextLevel.h"
#include "UObject/Object.h"
#include "WorldLevelHandler/NormalHandlers/MapsInfoHandler/MapsInfoHandler.h"
#include "WorldLevelHandler/UI_Modules/UI_MapSelection/MapSelectionBaseHandler.h"
#include "NextLevelBaseHandler.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class BASEUI_MODULE_API UNextLevelBaseHandler : public UUIHandlerBase, public II_UI_NextLevel
{
	GENERATED_BODY()

protected:
	virtual void AssignDependentHandlerPtr() override;
	
public:
	virtual bool HasNextLevel_Implementation() override;
	virtual void LoadNextLevel_Implementation() override;
	virtual bool HasNextLevel_CPP() override;
	virtual void LoadNextLevel_CPP() override;

protected:
	UPROPERTY()
	UMapsInfoHandler* MapsInfoHandler;

	UPROPERTY()
	UMapSelectionBaseHandler* MapSelectionHandler;

public:
	static const FFunctionHandlerDef HandlerDef;
};
