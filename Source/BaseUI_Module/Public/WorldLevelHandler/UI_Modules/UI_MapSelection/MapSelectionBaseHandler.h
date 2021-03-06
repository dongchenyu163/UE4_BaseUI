// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WorldLevelHandler/UI_Modules/BaseHandler/UIHandlerBase.h"
#include "Interfaces/I_UI_MapSelectable.h"
#include "UObject/Object.h"
#include "WorldLevelHandler/NormalHandlers/MapsInfoHandler/MapsInfoHandler.h"
#include "WorldLevelHandler/NormalHandlers/WidgetHandler/WidgetHandler.h"
#include "MapSelectionBaseHandler.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class BASEUI_MODULE_API UMapSelectionBaseHandler : public UUIHandlerBase, public II_UI_MapSelectable
{
	GENERATED_BODY()

protected:
	virtual void AssignDependentHandlerPtr() override;

protected:
	virtual ~UMapSelectionBaseHandler() override {}
public:
	virtual TArray<FName> GetMapIDList_Implementation(UDataTable* InMapInfoDataTable) override { return GetMapIDList_CPP(InMapInfoDataTable); }
	virtual TArray<FName> GetMapIDList_CPP(UDataTable* InMapInfoDataTable) override;
	virtual TArray<FMapInfo> GetMapInfoList_Implementation(UDataTable* InMapInfoDataTable) override;
	virtual void LoadMap_Implementation(FMapInfo InMapInfo) override;
	virtual void LoadMap_CPP(const FMapInfo* InMapInfo) override;

protected:
	UPROPERTY()
	UMapsInfoHandler* MapsInfoHandler;
	
	UPROPERTY()
	UWidgetHandler* WidgetHandler;
public:
	const static FFunctionHandlerDef HandlerDef;

};
