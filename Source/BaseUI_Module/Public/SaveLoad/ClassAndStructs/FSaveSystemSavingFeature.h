#pragma once

#include "CoreMinimal.h"
#include "ESaveGameType.h"
#include "FSaveSystemSavingFeature.generated.h"


USTRUCT(BlueprintType)
struct FSaveSystemSavingFeature
{
	GENERATED_BODY()
public:
	FSaveSystemSavingFeature(bool bInUseOfficial, bool bInNumberSlotID, bool bInRollingBuffer, bool bInStringSlotID)
		: bUseOfficial(bInUseOfficial),
		bNumberSlotID(bInNumberSlotID),
		bRollingBuffer(bInRollingBuffer),
		bStringSlotID(bInStringSlotID)
	{}
	FSaveSystemSavingFeature()
		: bUseOfficial(false),
		bNumberSlotID(true),
		bRollingBuffer(false),
		bStringSlotID(false)
	{}

	// NOTE: DO NOT add any properties before！！！
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Config, SkipSerialization, Category="Saving Feature", meta=(ToolTip="与剩下的Feature互斥，使用UGameplayStatic中的系列函数进行保存。"))
	uint8 bUseOfficial:1;  // default member initializers for bit-fields requires at least '/std:c++20'

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Config, SkipSerialization, Category="Saving Feature", meta=(ToolTip="使用数字作为文件名进行保存"))
	uint8 bNumberSlotID:1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Config, SkipSerialization, Category="Saving Feature", meta=(ToolTip="使用滚动缓冲区进行循环覆盖保存，必须和bNumberSlotID配合使用", EditCondition="bNumberSlotID"))
	uint8 bRollingBuffer:1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Config, SkipSerialization, Category="Saving Feature", meta=(ToolTip="使用字符串作为文件名保存"))
	uint8 bStringSlotID:1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Config, SkipSerialization, Category="Saving Feature", meta=(ToolTip="SaveGames文件夹开始的路径", EditCondition="bRollingBuffer"))
	int RollingBufferSize = 10;

	UPROPERTY(BlueprintReadWrite, SaveGame, meta=(ToolTip="【滚动缓冲区头指针】；bRollingBuffer模式下存储当前存档位置的变量，会被保存在【用户存档】中"))
	int RollingBufferPos = INDEX_NONE;

	friend FArchive& operator<< (FArchive& Ar, FSaveSystemSavingFeature& InStruct)
	{
		Ar << InStruct.RollingBufferPos;
		return Ar;
	}

	ESaveGameType ToSaveType()
	{
		const uint8 PackedValue = *((uint8*)this);
		if (bUseOfficial)
		{
			return OfficialSave;
		}
		if (PackedValue == 0b0010)
		{
			return NumberSave;
		}
		if (PackedValue == 0b0110)
		{
			return RollingSave;
		}
		if (PackedValue == 0b1000)
		{
			return CustomNameSave;
		}
		return InvalidType_ESaveGameType;
	}

	void FromSaveType(ESaveGameType InSaveType)
	{
		uint8* PackedValuePtr = (uint8*)this;
		switch (InSaveType)
		{
		case OfficialSave:
			*PackedValuePtr = 0b0001;
			break;
		case NumberSave:
			*PackedValuePtr = 0b0010;
			break;
		case RollingSave:
			*PackedValuePtr = 0b0110;
			break;
		case CustomNameSave:
			*PackedValuePtr = 0b1000;
			break;
		default:
			*PackedValuePtr = 0;
		}
	}
};