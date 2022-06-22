// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "ClassesAndStructs/MapInfo.h"
#include "MapsInfoHandler.generated.h"

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class BASEUI_MODULE_API UMapsInfoHandler : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Instanced, BlueprintReadWrite, Category="")
	UMapDescriptor* Descriptor;

	// UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="MenuFramework|WorldLevelHandler|MapInfo", meta=(ToolTip="返回主菜单时用"))
	// TSoftObjectPtr<UWorld> GetMainMenuMap();
	//
	// UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="MenuFramework|WorldLevelHandler|MapInfo")
	// UDataTable* GetMapInfoDataTable();
	//
	// UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="MenuFramework|WorldLevelHandler|MapInfo")
	// FMapInfo GetMapUIInfo(FName InMapIdentifier);
	//
	// UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="MenuFramework|WorldLevelHandler|MapInfo", meta=(ToolTip="通过UserGlobalData来更新GameInstance里面的成员"))
	// void UpdateFromUserGlobalData();
	//
	// UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="MenuFramework|WorldLevelHandler|MapInfo")
	// FMapInfo GetPlayingMapUIInfo();
	//
	// UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="MenuFramework|WorldLevelHandler|MapInfo")
	// void SetPlayingMapUIInfo(FMapInfo InNewMapInfo);
	//
	// UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="MenuFramework|WorldLevelHandler|MapInfo", meta=(ToolTip="重置UDA_WidgetInfo中的WidgetRef变量，切换地图的时候该变量会被置空"))
	// void ResetWidgetInfo();

};
