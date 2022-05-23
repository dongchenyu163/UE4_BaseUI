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
	virtual TArray<FName> GetMapIDList_Implementation(UDataTable* InMapInfoDataTable) override;
	virtual TArray<FMapUIInfo> GetMapInfoList_Copy_Implementation(UDataTable* InMapInfoDataTable) override;
	virtual void LoadMap_CPP(const FMapUIInfo* InMapInfo) override;
	virtual void LoadMap_Implementation(FMapUIInfo InMapInfo) override;
};
