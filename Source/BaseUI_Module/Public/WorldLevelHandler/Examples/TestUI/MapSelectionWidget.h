// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TestUIBase.h"
#include "UObject/Object.h"
#include "MapSelectionWidget.generated.h"

/**
 * 
 */
UCLASS()
class BASEUI_MODULE_API UMapSelectionWidget : public UTestUIBase, public II_UI_MapSelectable
{
	GENERATED_BODY()
public:
	virtual TArray<FName> GetMapIDList_CPP(UDataTable* InMapInfoDataTable) override;
	virtual TArray<FName> GetMapIDList_Implementation(UDataTable* InMapInfoDataTable) override { return GetMapIDList_CPP(InMapInfoDataTable); }
	virtual void LoadMap_CPP(const FMapInfo* InMapInfo) override;
	virtual void LoadMap_Implementation(FMapInfo InMapInfo) override;
};
