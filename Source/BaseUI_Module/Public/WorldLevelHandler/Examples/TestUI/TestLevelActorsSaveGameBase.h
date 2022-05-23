// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "GameFramework/SaveGame.h"
#include "WorldLevelHandler/UI_Modules/UI_MapSelection/Interfaces/I_UI_MapSelectable.h"
#include "WorldLevelHandler/UI_Modules/UI_Save/Interfaces/I_UI_Savable.h"
#include "TestLevelActorsSaveGameBase.generated.h"

/**
 * 
 */
UCLASS(BlueprintType, Deprecated, meta=(DeprecationMessage="本类被ULevelActorsSaveGameObjBase类代替"))
// class BASEUI_MODULE_API UTestLevelActorsSaveGameBase : public USaveGame
class BASEUI_MODULE_API UDEPRECATED_TestLevelActorsSaveGameBase : public USaveGame
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="SaveGame|MapInfo", meta=(Tooltip="游戏的地图进度"))
	TArray<FMapUIRuntimeInfo> MapUIRuntimeInfoList;

	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="SaveGame|PlayingInfo", meta=(Tooltip="游玩的某一关的进度"))
	// USaveGameObjectHeaderBase* PlayingInfo;
};
