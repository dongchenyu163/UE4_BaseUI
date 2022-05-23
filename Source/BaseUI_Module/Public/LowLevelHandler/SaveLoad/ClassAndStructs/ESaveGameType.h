#pragma once

#include "CoreMinimal.h"


// TODO：再次优化存档方式。
UENUM(BlueprintType)
enum ESaveGameType
{
	NumberSave UMETA(Tooltip="以数字作为存档名称；本存储方式是需要给定USaveGame USaveGameObjectHeaderBase，并存到同一个文件"),
	RollingSave UMETA(Tooltip="使用滚动Buffer循环覆盖的方式存档，并以数字作为存档名称；本存储方式是需要给定USaveGame USaveGameObjectHeaderBase，并存到同一个文件"),
	CustomNameSave UMETA(Tooltip="完全自定义存档名称；本存储方式是需要给定USaveGame USaveGameObjectHeaderBase，并存到同一个文件"),
	OfficialSave UMETA(Tooltip="自定义存档名称，并且使用 官方的 UGameplayStatic系列函数进行存档的保存；本存储方式 只保存USaveGame"),
	EndEnum_ESaveGameType UMETA(Hidden),
	InvalidType_ESaveGameType UMETA(Hidden)
};
ENUM_RANGE_BY_COUNT(ESaveGameType, ESaveGameType::EndEnum_ESaveGameType);

UENUM(BlueprintType)
enum ESaveArea_BP
{
	SaveArea_Max = 0 UMETA(Hidden)  // , Tooltip = "防止ESaveArea_BP没有值的时候会报错"
};