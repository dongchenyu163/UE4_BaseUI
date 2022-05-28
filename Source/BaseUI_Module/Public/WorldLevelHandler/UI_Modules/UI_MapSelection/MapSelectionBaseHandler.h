﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WorldLevelHandler/UI_Modules/BaseHandler/UIHandlerBase.h"
#include "Interfaces/I_UI_MapSelectable.h"
#include "UObject/Object.h"
#include "MapSelectionBaseHandler.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class BASEUI_MODULE_API UMapSelectionBaseHandler : public UUIHandlerBase, public II_UI_MapSelectable
{
	GENERATED_BODY()

protected:
	virtual ~UMapSelectionBaseHandler() override {}
public:
	virtual TArray<FName> GetMapIDList_Implementation(UDataTable* InMapInfoDataTable) override;
	virtual TArray<FMapUIInfo> GetMapInfoList_Copy_Implementation(UDataTable* InMapInfoDataTable) override;
	virtual void LoadMap_Implementation(FMapUIInfo InMapInfo) override;
	virtual void LoadMap_CPP(const FMapUIInfo* InMapInfo) override;
};