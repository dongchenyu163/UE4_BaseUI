#pragma once

#include "CoreMinimal.h"


UENUM(BlueprintType)
enum ESaveGameType
{
	NormalSave,
	AutoSave,
	QuickSave,
	CustomLevelSave UMETA(Tooltip=""),
	CustomSave UMETA(Tooltip=""),
	EndEnum UMETA(Hidden),
	InvalidType UMETA(Hidden)
};
ENUM_RANGE_BY_COUNT(ESaveGameType, ESaveGameType::EndEnum);  // 可以迭代Enum的值


// TODO：再次优化存档方式。
UENUM(BlueprintType)
enum ESaveGameType_Candidate
{
	NumberSave UMETA(Tooltip="以数字作为存档名称；本存储方式是需要给定USaveGame USaveGameObjectHeaderBase，并存到同一个文件"),
	RollingSave UMETA(Tooltip="使用滚动Buffer循环覆盖的方式存档，并以数字作为存档名称；本存储方式是需要给定USaveGame USaveGameObjectHeaderBase，并存到同一个文件"),
	CustomNameSave UMETA(Tooltip="完全自定义存档名称；本存储方式是需要给定USaveGame USaveGameObjectHeaderBase，并存到同一个文件"),
	OfficialSave UMETA(Tooltip="自定义存档名称，并且使用 官方的 UGameplayStatic系列函数进行存档的保存；本存储方式 只保存USaveGame"),
	EndEnum_Candidate UMETA(Hidden),
	InvalidType_Candidate UMETA(Hidden)
};
ENUM_RANGE_BY_COUNT(ESaveGameType_Candidate, ESaveGameType_Candidate::EndEnum_Candidate);

UENUM(BlueprintType)
enum ESaveArea_BP
{
	SaveArea_Max = 0 UMETA(Hidden)  // , Tooltip = "防止ESaveArea_BP没有值的时候会报错"
};