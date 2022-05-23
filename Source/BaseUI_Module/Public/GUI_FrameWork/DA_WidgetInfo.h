// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "DA_WidgetInfo.generated.h"

/**
 * 方便切换UI的时候指定UI；
 */
UCLASS()
class BASEUI_MODULE_API UDA_WidgetInfo : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="WidgetInfo", meta=(ToolTip=""))
	FName WidgetID;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="WidgetInfo", meta=(ToolTip=""))
	TSubclassOf<UUserWidget> WidgetClassRef;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="WidgetInfo", meta=(ToolTip="请在DT_MapInfo当中寻找相关联的地图的标识符"))
	FName AssociatedMapIdentifier;
	
	UPROPERTY()
	UUserWidget* WidgetRef = nullptr;

};
