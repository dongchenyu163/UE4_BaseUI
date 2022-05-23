#pragma once

#include "CoreMinimal.h"
#include "ESaveGameType.h"
#include "FSaveSystemSavingFeature.h"
#include "FSaveLoadSystemSaveArea.generated.h"

USTRUCT(BlueprintType, meta=(ToolTip="定义一个【存储区域】，代表一类存档的【存储位置】【命名方式】等"))
struct FSaveLoadSystemSaveArea
{
	GENERATED_BODY()
public:
	FSaveLoadSystemSaveArea()
	{
		SaveGameFeatureRuntimeInfo.FromSaveType(NumberSave);
	}
	FSaveLoadSystemSaveArea(FString InFolderNameText, TEnumAsByte<ESaveGameType_Candidate> InSaveGameType, bool InIsSavingHeaderObj = true)
	{
		SaveGameFeatureRuntimeInfo.FromSaveType(InSaveGameType);
		SaveLocationFormatStr = InFolderNameText;
		SaveGameType = InSaveGameType;
		bSavingHeaderObj = InIsSavingHeaderObj;
	}

	UPROPERTY(EditAnywhere, BlueprintReadOnly, SkipSerialization, Config, Category="Save Area Info", meta=(ToolTip="存档文件的文件名前缀。"))
	TEnumAsByte<ESaveGameType_Candidate> SaveGameType = NumberSave;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, SkipSerialization, Config, Category="Save Area Info", meta=(ToolTip="本存储区域的根路径的字符串：从SaveGames文件夹开始计算。可以使用{UserID}和{UserName}宏"))
	FString SaveLocationFormatStr = "{UserID}/";
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, SkipSerialization, Config, Category="Save Area Info", DisplayName="Slot Name Format String", meta=(ToolTip="存档文件的命名规则，需要在这里指定扩展名。"))
	FString SaveSlotNameFormat = "{UserID}_{SlotID}.sav";

	UPROPERTY(EditAnywhere, BlueprintReadOnly, SkipSerialization, Config, Category="Save Area Info", meta=(ToolTip="当前存档区域的注释、解释说明，不参与程序的运行。"))
	FString CommentForThisArea = "";

	UPROPERTY(EditAnywhere, BlueprintReadOnly, SkipSerialization, Config, Category="Save Area Info", meta=(ToolTip="是否在保存的时候 保存【FDongSaveSystemSavingParam】传入的【USaveGameObjectHeaderBase】类型的【SaveObjHeader】变量？"))
	bool bSavingHeaderObj = true;
	
	UPROPERTY(BlueprintReadWrite, meta=(ToolTip="里面的RollingBufferPos变量会被序列化"))
	FSaveSystemSavingFeature SaveGameFeatureRuntimeInfo;
	
	friend FArchive& operator<< (FArchive& Ar, FSaveLoadSystemSaveArea& InStruct)
	{
		Ar << InStruct.SaveGameFeatureRuntimeInfo;
		return Ar;
	}
};